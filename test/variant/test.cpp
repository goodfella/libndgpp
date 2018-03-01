#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#include <libndgpp/variant.hpp>

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
    operator T () { throw 42; }
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

TEST(copy_ctor, is_called)
{
    struct tracker
    {
        tracker() = default;
        tracker(const tracker& other):
            called(true)
        {}

        bool called {false} ;
    };

    bool called {false};
    ndgpp::variant<tracker> other{tracker{}};
    ndgpp::variant<tracker> copy{other};

    copy.match(
        [&called] (const tracker& tracker) {
            called = tracker.called;
        }
    );

    EXPECT_TRUE(called);
}

struct move_tracker
{
    move_tracker() = default;

    move_tracker(const move_tracker&):
        called(false)
    {}

    move_tracker& operator=(const move_tracker&)
    {
        called = false;
    }

    move_tracker(move_tracker&& other):
        called(true)
    {}

    move_tracker& operator=(move_tracker&&)
    {
        called = true;
    }

    bool called {false};
};

TEST(move_ctor, is_called)
{

    ndgpp::variant<bool, move_tracker> tmp{ndgpp::variant<bool, move_tracker>{move_tracker{}}};

    bool called {false};
    tmp.match(
        [&called] (const bool&) {
            called = false;
        },
        [&called] (const move_tracker& tracker) {
            called = tracker.called;
        }
    );

    EXPECT_TRUE(called);
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
                correct_value = false;
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
                move_called = false;
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
                correct_value = false;
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
                move_called = false;
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
    }
    catch (...) {}
    ASSERT_TRUE(v1.valueless_by_exception());

    ndgpp::variant<double, int> v2{1};
    try
    {
        v2.emplace<1>(throws_on_conversion<int>{});
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
        v2.emplace<1>(throws_on_conversion<dtor_tracker>{});
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
        [] (const int) {}
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
        EXPECT_TRUE(v.holds_alternative<int>());
        EXPECT_FALSE(v.holds_alternative<double>());
    }

    {
        ndgpp::variant<int, double> v{1.0};
        EXPECT_TRUE(v.holds_alternative<double>());
        EXPECT_FALSE(v.holds_alternative<int>());
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
            proper_type = false;
        }
    );

    EXPECT_TRUE(proper_type);
}

TEST(member_function, emplace_index_arg_with_exception)
{
    ndgpp::variant<double, int> v{1};
    const auto will_throw = [&v] () {v.emplace<1>(throws_on_conversion<int>{});};
    EXPECT_THROW(will_throw(), int);
    EXPECT_TRUE(v.valueless_by_exception());
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
