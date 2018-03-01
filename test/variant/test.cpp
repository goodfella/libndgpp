#include <gtest/gtest.h>
#include <functional>
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

TEST(move_ctor, is_called)
{
    struct tracker
    {
        tracker() = default;
        tracker(const tracker&) = default;
        tracker& operator=(const tracker&) = default;

        tracker(tracker&& other)
        {
            called = true;
        }

        bool called {false};
    };

    ndgpp::variant<bool, tracker> tmp{ndgpp::variant<bool, tracker>{tracker{}}};

    bool called {false};
    tmp.match(
        [&called] (const bool&) {
            called = false;
        },
        [&called] (const tracker& tracker) {
            called = tracker.called;
        }
    );

    EXPECT_TRUE(called);
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
