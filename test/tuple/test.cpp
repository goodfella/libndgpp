#include <gtest/gtest.h>

#include <libndgpp/tuple.hpp>

TEST(tuple_contains, is_true)
{
    {
        constexpr auto contains = ndgpp::tuple_contains<bool, std::tuple<bool, int>>::value;
        EXPECT_TRUE(contains);
    }
}

TEST(tuple_contains, is_false)
{
    {
        constexpr auto contains = ndgpp::tuple_contains<bool, std::tuple<>>::value;
        EXPECT_FALSE(contains);

    }

    {
        constexpr auto contains = ndgpp::tuple_contains<bool, std::tuple<int, double>>::value;
        EXPECT_FALSE(contains);
    }
}

TEST(tuple_index, first_type)
{
    using tuple_type = std::tuple<int, double, bool>;
    constexpr auto index = ndgpp::tuple_index<int, tuple_type>::value;
    EXPECT_EQ(0U, index);
}

TEST(tuple_index, middle_type)
{
    using tuple_type = std::tuple<int, double, bool>;
    constexpr auto index = ndgpp::tuple_index<double, tuple_type>::value;
    EXPECT_EQ(1U, index);
}

TEST(tuple_index, last_type)
{
    using tuple_type = std::tuple<int, double, bool>;
    constexpr auto index = ndgpp::tuple_index<bool, tuple_type>::value;
    EXPECT_EQ(2U, index);
}
