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
