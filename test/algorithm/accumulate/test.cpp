#include <gtest/gtest.h>

#include <libndgpp/algorithm/accumulate.hpp>

TEST(noargs, test)
{
    EXPECT_EQ(0, ndgpp::accumulate());
}

TEST(sumzero, test)
{
    EXPECT_EQ(0, ndgpp::accumulate(1, -1));
}

TEST(boolsum, test)
{
    EXPECT_EQ(2, ndgpp::accumulate(true, true));
}

TEST(negative, test)
{
    EXPECT_EQ(-2, ndgpp::accumulate(-1, -1));
}
