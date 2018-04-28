#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#include <stdexcept>

#include <libndgpp/variant.hpp>
#include <libndgpp/type_traits/conjunction_type.hpp>
#include <libndgpp/type_traits/copy_control.hpp>

struct dtor_tracker
{
    dtor_tracker(bool& called):
        called_(called)
        {
            called_.get() = false;
        }

    ~dtor_tracker()
        {
            called_.get() = true;
        }

    std::reference_wrapper<bool> called_;
};

template <class T>
struct throws_on_conversion
{
    operator T () { throw std::logic_error("throws_on_conversion"); }
};


TEST(variant_alternative, not_helper_alias)
{
    using variant_type = ndgpp::variant<char, int ,short>;
    using type_0 = typename ndgpp::variant_alternative<0, variant_type>::type;
    using type_1 = typename ndgpp::variant_alternative<1, variant_type>::type;
    using type_2 = typename ndgpp::variant_alternative<2, variant_type>::type;

    constexpr auto test_0 = std::is_same<char, type_0>::value;
    constexpr auto test_1 = std::is_same<int, type_1>::value;
    constexpr auto test_2 = std::is_same<short, type_2>::value;

    EXPECT_TRUE(test_0);
    EXPECT_TRUE(test_1);
    EXPECT_TRUE(test_2);
}

TEST(variant_alternative, helper_alias)
{
    using variant_type = ndgpp::variant<char, int ,short>;
    using type_0 = ndgpp::variant_alternative_t<0, variant_type>;
    using type_1 = ndgpp::variant_alternative_t<1, variant_type>;
    using type_2 = ndgpp::variant_alternative_t<2, variant_type>;

    constexpr auto test_0 = std::is_same<char, type_0>::value;
    constexpr auto test_1 = std::is_same<int, type_1>::value;
    constexpr auto test_2 = std::is_same<short, type_2>::value;

    EXPECT_TRUE(test_0);
    EXPECT_TRUE(test_1);
    EXPECT_TRUE(test_2);
}

TEST(dtor, is_called)
{

    bool called = false;
    {
        ndgpp::variant<dtor_tracker, int> v{dtor_tracker{called}};
    }

    EXPECT_TRUE(called);
}

TEST(dtor, is_not_called)
{
    bool called = false;
    {
        ndgpp::variant<dtor_tracker, int> v{1};
    }

    EXPECT_FALSE(called);
}

TEST(dtor, valueless_by_exception)
{
    bool called = false;
    {
        ndgpp::variant<dtor_tracker> v {dtor_tracker{called}};
        try
        {
            v.emplace<0>(throws_on_conversion<dtor_tracker>{});
            FAIL();
        }
        catch (...)
        {
            ASSERT_TRUE(called);
            called = false;
        }

        ASSERT_TRUE(v.valueless_by_exception());
    }

    EXPECT_FALSE(called);
}

struct copy_tracker
{
    copy_tracker() = default;
    copy_tracker(copy_tracker && other):
        called(false)
    {}

    copy_tracker(const copy_tracker& other):
        called(true)
    {}

    copy_tracker & operator= (const copy_tracker & other)
    {
        if (this == &other)
        {
            return *this;
        }

        called = true;
        return *this;
    }

    copy_tracker & operator= (copy_tracker && other)
    {
        if (this == &other)
        {
            return *this;
        }

        called = false;
        return *this;
    }

    bool called {false} ;
};


TEST(copy_ctor, is_called)
{
    bool called {false};
    ndgpp::variant<copy_tracker> other{copy_tracker{}};
    ndgpp::variant<copy_tracker> copy{other};

    copy.match(
        [&called] (const copy_tracker& tracker) {
            called = tracker.called;
        }
    );

    EXPECT_TRUE(called);
}

TEST(is_copyable, non_copyable_type)
{
    using variant_type = ndgpp::variant<ndgpp::non_copyable_type>;
    constexpr bool copyable = ndgpp::conjunction_type<std::is_copy_constructible<variant_type>>::value;
    EXPECT_FALSE(copyable);
}

TEST(is_copyable, copyable_type)
{
    using variant_type = ndgpp::variant<ndgpp::copyable_type>;
    constexpr bool copyable = ndgpp::conjunction_type<std::is_copy_constructible<variant_type>>::value;

    EXPECT_TRUE(copyable);
}

TEST(is_moveable, non_moveable_type)
{
    using variant_type = ndgpp::variant<ndgpp::non_moveable_type>;
    constexpr bool moveable = ndgpp::conjunction_type<std::is_move_constructible<variant_type>,
                                                      std::is_move_assignable<variant_type>>::value;
    EXPECT_FALSE(moveable);
}

TEST(is_moveable, moveable_type)
{
    using variant_type = ndgpp::variant<ndgpp::moveable_type>;
    constexpr bool moveable = ndgpp::conjunction_type<std::is_move_constructible<variant_type>,
                                                      std::is_move_assignable<variant_type>>::value;
    EXPECT_TRUE(moveable);
}

struct move_tracker
{
    move_tracker() = default;

    move_tracker(const move_tracker&):
        called(false)
    {}

    move_tracker& operator=(const move_tracker & other)
    {
        if (this == &other) {
            return *this;
        }

        called = false;
        return *this;
    }

    move_tracker(move_tracker&& other):
        called(true)
    {}

    move_tracker& operator=(move_tracker && other)
    {
        if (this == &other)
        {
            return *this;
        }

        called = true;
        return *this;
    }

    bool called {false};
};

TEST(move_ctor, is_called)
{

    ndgpp::variant<bool, move_tracker> tmp{ndgpp::variant<bool, move_tracker>{move_tracker{}}};

    bool called {false};
    tmp.match(
        [&called] (const bool&) {
            FAIL();
        },
        [&called] (const move_tracker& tracker) {
            called = tracker.called;
        }
    );

    EXPECT_TRUE(called);
}

TEST(move_ctor, other_valueless_by_exception)
{
    ndgpp::variant<bool, int> v1{1};
    try
    {
        v1.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...)
    {}

    ASSERT_TRUE(v1.valueless_by_exception());

    ndgpp::variant<bool, int> v2{std::move(v1)};
    ASSERT_TRUE(v2.valueless_by_exception());
}

TEST(move_ctor, other_with_value)
{
    ndgpp::variant<bool, int> v1{ndgpp::variant<bool, int>{true}};
    EXPECT_EQ(0, v1.index());

    bool correct_value = false;
    v1.match(
        [&correct_value] (const bool v) {
            correct_value = v == true;
        },
        [&correct_value] (const int) {
            FAIL();
        }
    );

    EXPECT_TRUE(correct_value);
}

TEST(move_ctor, noexcept_specifier)
{
    {
        constexpr bool is_noexcept = std::is_nothrow_move_constructible<ndgpp::variant<int, double>>::value;
        EXPECT_TRUE(is_noexcept);
    }

    {
        struct move_construct_throws
        {
            move_construct_throws(move_construct_throws &&) { throw std::logic_error{__func__}; }
            move_construct_throws & operator= (move_construct_throws &&) = default;
        };

        constexpr bool is_noexcept = std::is_nothrow_move_constructible<ndgpp::variant<int, move_construct_throws>>::value;
        EXPECT_FALSE(is_noexcept);
    }
}

TEST(move_assign, different_types)
{
    {
        ndgpp::variant<int, double> v1{1};
        ASSERT_EQ(0, v1.index());

        const auto & rv = v1 = ndgpp::variant<int, double>{1.3};
        EXPECT_EQ(1, v1.index());
        EXPECT_EQ(&v1, &rv);

        bool correct_value = false;
        v1.match(
            [&correct_value] (const int&) {
                FAIL();
            },
            [&correct_value] (const double& v) {
                correct_value = (v == 1.3);
            }
        );

        EXPECT_TRUE(correct_value);
    }

    {
        ndgpp::variant<int, move_tracker> v1{1};
        v1 = ndgpp::variant<int, move_tracker>{move_tracker{}};

        bool move_called = false;
        v1.match(
            [&move_called] (const int&) {
                FAIL();
            },
            [&move_called] (const move_tracker& t) {
                move_called = t.called;
            }
        );

        EXPECT_TRUE(move_called);
    }
}

TEST(move_assign, same_types)
{
    {
        ndgpp::variant<int, double> v1{1};
        ASSERT_EQ(0, v1.index());

        const auto & rv = v1 = ndgpp::variant<int, double>{2};
        EXPECT_EQ(0, v1.index());
        EXPECT_EQ(&v1, &rv);

        bool correct_value = false;
        v1.match(
            [&correct_value] (const int& v) {
                correct_value = (v == 2);
            },
            [&correct_value] (const double&) {
                FAIL();
            }
        );

        EXPECT_TRUE(correct_value);
    }

    {
        ndgpp::variant<int, move_tracker> v1{move_tracker{}};
        v1 = ndgpp::variant<int, move_tracker>{move_tracker{}};

        bool move_called = false;
        v1.match(
            [&move_called] (const int&) {
                FAIL();
            },
            [&move_called] (const move_tracker& t) {
                move_called = t.called;
            }
        );

        EXPECT_TRUE(move_called);
    }
}

TEST(move_assign, both_valueless_by_exception)
{
    ndgpp::variant<double, int> v1{1};
    try
    {
        v1.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v1.valueless_by_exception());

    ndgpp::variant<double, int> v2{1};
    try
    {
        v2.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v2.valueless_by_exception());

    const auto & rv = v1 = std::move(v2);
    EXPECT_EQ(&v1, &rv);
    EXPECT_TRUE(v1.valueless_by_exception());
}

TEST(move_assign, other_valueless_by_exception)
{
    bool dtor_called = false;
    ndgpp::variant<double, dtor_tracker> v1{dtor_tracker{dtor_called}};

    ndgpp::variant<double, dtor_tracker> v2{1.0};
    try
    {
        bool tmp = false;
        v2.emplace<1>(throws_on_conversion<dtor_tracker>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v2.valueless_by_exception());

    const auto & rv = v1 = std::move(v2);
    EXPECT_EQ(&v1, &rv);
    EXPECT_TRUE(v1.valueless_by_exception());
    EXPECT_TRUE(dtor_called);
}

TEST(move_assign, this_valueless_by_exception)
{
    ndgpp::variant<double, int> v1{1.0};
    try
    {
        v1.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v1.valueless_by_exception());

    ndgpp::variant<double, int> v2{3.0};

    const auto & rv = v1 = std::move(v2);
    EXPECT_EQ(&v1, &rv);
    EXPECT_FALSE(v1.valueless_by_exception());
    EXPECT_EQ(0, v1.index());

    bool correct_value = false;
    v1.match(
        [&correct_value] (const double v) {
            correct_value = (v == 3.0);
        },
        [] (const int) {
            FAIL();
        }
    );

    EXPECT_TRUE(correct_value);
}

TEST(move_assign, noexcept_specifier)
{
    {
        constexpr bool is_noexcept = std::is_nothrow_move_assignable<ndgpp::variant<int, double>>::value;
        EXPECT_TRUE(is_noexcept);
    }

    {
        struct move_assign_throws
        {
            move_assign_throws(move_assign_throws &&) = default;
            move_assign_throws & operator= (move_assign_throws &&) { throw 42; }
        };

        constexpr bool is_noexcept = std::is_nothrow_move_assignable<ndgpp::variant<int, move_assign_throws>>::value;
        EXPECT_FALSE(is_noexcept);
    }
}

TEST(copy_assign, different_types)
{
    ndgpp::variant<bool, double> v1{true};
    ndgpp::variant<bool, double> v2{1.2};

    const auto & rv = v1 = v2;
    EXPECT_EQ(1, v1.index());
    EXPECT_EQ(&v1, &rv);

    bool proper_value = false;
    v1.match(
        [] (const bool v) {
            FAIL();
        },
        [&] (const double v) {
            proper_value = (v == 1.2);
        }
    );
    EXPECT_TRUE(proper_value);
}

TEST(copy_assign, same_types)
{
    {
        ndgpp::variant<bool, double> v1{true};
        ndgpp::variant<bool, double> v2{false};

        v1 = v2;
        EXPECT_EQ(0, v1.index());

        bool proper_value = false;
        v1.match(
            [&] (const bool v) {
                proper_value = (v == false);
            },
            [] (const double v) {
                FAIL();
            }
            );

        EXPECT_TRUE(proper_value);
    }

    {
        ndgpp::variant<bool, copy_tracker> v1{copy_tracker{}};
        ndgpp::variant<bool, copy_tracker> v2{copy_tracker{}};

        v1.match(
            [] (bool) {
                FAIL();
            },
            [&] (const copy_tracker& t) {
                ASSERT_FALSE(t.called);
            }
        );

        v2.match(
            [] (bool) {
                FAIL();
            },
            [&] (const copy_tracker& t) {
                ASSERT_FALSE(t.called);
            }
        );

        const auto & rv = v1 = v2;
        EXPECT_EQ(&v1, &rv);

        bool called = false;
        v1.match(
            [] (bool) {
                FAIL();
            },
            [&] (const copy_tracker& t) {
                called = t.called;
            }
        );

        EXPECT_TRUE(called);
    }
}

TEST(copy_assign, both_valueless_by_exception)
{
    ndgpp::variant<double, int> v1{1};
    try
    {
        v1.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v1.valueless_by_exception());

    ndgpp::variant<double, int> v2{1};
    try
    {
        v2.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v2.valueless_by_exception());

    const auto & rv = v1 = v2;
    EXPECT_EQ(&v1, &rv);
    EXPECT_TRUE(v1.valueless_by_exception());
}

TEST(copy_assign, this_valueless_by_exception)
{
    ndgpp::variant<double, int> v1{1};
    try
    {
        v1.emplace<1>(throws_on_conversion<int>{});
        FAIL();
    }
    catch (...) {}
    ASSERT_TRUE(v1.valueless_by_exception());

    ndgpp::variant<double, int> v2{1};

    const auto & rv = v1 = v2;
    EXPECT_EQ(&v1, &rv);

    bool proper_value = false;
    v1.match(
        [] (double) {
            FAIL();
        },
        [&] (int v) {
            proper_value = (v == 1);
        }
    );

    EXPECT_TRUE(proper_value);
}

TEST(member_function, index)
{
    {
        ndgpp::variant<int, double> v{1};
        EXPECT_EQ(0U, v.index());
    }

    {
        ndgpp::variant<int, double> v{1.0};
        EXPECT_EQ(1U, v.index());
    }
}

TEST(member_function, holds_alternative)
{
    {
        ndgpp::variant<int, double> v{1};
        EXPECT_TRUE(ndgpp::holds_alternative<int>(v));
        EXPECT_FALSE(ndgpp::holds_alternative<double>(v));
    }

    {
        ndgpp::variant<int, double> v{1.0};
        EXPECT_TRUE(ndgpp::holds_alternative<double>(v));
        EXPECT_FALSE(ndgpp::holds_alternative<int>(v));
    }
}

TEST(member_function, emplace_index_args)
{
    ndgpp::variant<double, int> v{1};
    const auto t = v.emplace<0>(1.5);
    EXPECT_EQ(1.5, t);

    bool proper_type = false;
    v.match(
        [&proper_type] (const double&) {
            proper_type = true;
        },
        [&proper_type] (const int&) {
            FAIL();
        }
    );

    EXPECT_TRUE(proper_type);
}

TEST(member_function, emplace_index_arg_with_exception)
{
    ndgpp::variant<double, int> v{1};
    const auto will_throw = [&v] () {v.emplace<1>(throws_on_conversion<int>{});};
    EXPECT_THROW(will_throw(), std::logic_error);
    EXPECT_TRUE(v.valueless_by_exception());
}

TEST(member_function, emplace_index_multi_member_struct)
{
    struct temp
    {
        int i;
        double d;
    };

    ndgpp::variant<double, temp> v{double{1.0}};
    const auto & rv = v.emplace<1>(1, 1.0);

    EXPECT_EQ(1, v.index());
    EXPECT_EQ(1, rv.i);
    EXPECT_EQ(1.0, rv.d);

    bool proper_i_value = false;
    bool proper_d_value = false;
    v.match(
        [] (double) {
            FAIL();
        },
        [&] (temp t) {
            proper_i_value = (t.i == 1);
            proper_d_value = (t.d == 1.0);
        }
    );

    EXPECT_TRUE(proper_i_value);
    EXPECT_TRUE(proper_d_value);
}

TEST(member_function, operator_bool)
{
    {
        ndgpp::variant<ndgpp::none_t, int> v{1};
        EXPECT_TRUE(static_cast<bool>(v));
    }

    {
        ndgpp::variant<ndgpp::none_t, int> v{ndgpp::none};
        EXPECT_FALSE(static_cast<bool>(v));
    }

    struct foo
    {
        foo(const foo&) = delete;
        foo& operator=(const foo&) = delete;

        foo(foo&&) = default;
        foo& operator=(foo&&) = default;
    };
}

TEST(const_visit, no_throw)
{
    constexpr int i = 1;
    using variant_t = ndgpp::variant<short, int, long long>;
    variant_t v{i};
    const auto func = [] (auto v) -> void{
        using type = decltype(v);
        constexpr bool correct_type = std::is_same<type, int>::value;
        EXPECT_TRUE(correct_type);
    };

    ndgpp::visit(func, const_cast<const variant_t &>(v));
}

TEST(const_visit, returning_visitor)
{
    constexpr int i = 1;
    using variant_t = ndgpp::variant<short, int, long long>;
    variant_t v{i};
    const auto func = [] (auto v) -> bool {
        using type = std::decay_t<decltype(v)>;
        constexpr bool correct_type = std::is_same<type, int>::value;
        EXPECT_TRUE(correct_type);
        return correct_type;
    };

    const auto ret = ndgpp::visit(func, const_cast<const variant_t &>(v));
    EXPECT_TRUE(ret);
}

TEST(const_visit, throws)
{
    using variant_t = ndgpp::variant<bool>;
    variant_t v {false};
    try
    {
        v.emplace<0>(throws_on_conversion<bool>{});
        // It's considered a test failure if the above statement does
        // not throw.
        FAIL();
    }
    catch (...)
    {}

    ASSERT_TRUE(v.valueless_by_exception());

    const auto func = [] (auto v) -> void{
        // If this function is ever called by visit, then it's
        // considered a failure because the visitor should not be
        // called with a valueless_by_exception variant
        FAIL();
    };

    const auto throws = [&] () {
        ndgpp::visit(func, const_cast<const variant_t &>(v));

        // ndgpp::visit should never return because it's expected to
        // throw an exception.
        FAIL();
    };

    EXPECT_THROW(throws(), ndgpp::bad_variant_access);
}

TEST(const_visit, make_visitor)
{
    constexpr int i = 1;
    using variant_t = ndgpp::variant<short, int>;
    variant_t v{i};


    const bool ret = ndgpp::visit(ndgpp::make_visitor(
                                      [](short){return false;},
                                      [](int){return true;}),
                                  const_cast<const variant_t &>(v));

    EXPECT_TRUE(ret);
}


TEST(visit, no_throw)
{
    constexpr int i = 1;
    using variant_t = ndgpp::variant<short, int, long long>;
    variant_t v{i};
    const auto func = [] (auto v) -> void{
        using type = decltype(v);
        constexpr bool correct_type = std::is_same<type, int>::value;
        EXPECT_TRUE(correct_type);
    };

    ndgpp::visit(func, v);
}

TEST(visit, returning_visitor)
{
    constexpr int i = 1;
    using variant_t = ndgpp::variant<short, int, long long>;
    variant_t v{i};
    const auto func = [] (auto v) -> bool {
        using type = std::decay_t<decltype(v)>;
        constexpr bool correct_type = std::is_same<type, int>::value;
        EXPECT_TRUE(correct_type);
        return correct_type;
    };

    const auto ret = ndgpp::visit(func, v);
    EXPECT_TRUE(ret);
}

TEST(visit, throws)
{
    using variant_t = ndgpp::variant<bool>;
    variant_t v {false};
    try
    {
        v.emplace<0>(throws_on_conversion<bool>{});
        // It's considered a test failure if the above statement does
        // not throw.
        FAIL();
    }
    catch (...)
    {}

    ASSERT_TRUE(v.valueless_by_exception());

    const auto func = [] (auto v) -> void{
        // If this function is ever called by visit, then it's
        // considered a failure because the visitor should not be
        // called with a valueless_by_exception variant
        FAIL();
    };

    const auto throws = [&] () {
        ndgpp::visit(func, v);

        // ndgpp::visit should never return because it's expected to
        // throw an exception.
        FAIL();
    };

    EXPECT_THROW(throws(), ndgpp::bad_variant_access);
}

TEST(visit, make_visitor)
{
    constexpr int i = 1;
    using variant_t = ndgpp::variant<short, int>;
    variant_t v{i};


    const bool ret = ndgpp::visit(ndgpp::make_visitor(
                                      [](short){return false;},
                                      [](int){return true;}),
                                  v);

    EXPECT_TRUE(ret);
}
