#include <gtest/gtest.h>

#include <libndgpp/safe_operators.hpp>

TEST(lt_test, same_types_unequal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::lt(1, 3));
    EXPECT_FALSE(ndgpp::safe_op::lt(3, 1));

    EXPECT_TRUE(ndgpp::safe_op::lt(1U, 3U));
    EXPECT_FALSE(ndgpp::safe_op::lt(3U, 1U));
}

TEST(lt_test, negative_value)
{
    EXPECT_TRUE(ndgpp::safe_op::lt(-1, 0U));
}

TEST(lt_test, same_types_equal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::lt(0, 0));
    EXPECT_FALSE(ndgpp::safe_op::lt(0U, 0U));
}

TEST(lt_test, different_types_unequal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::lt(1, 3U));
    EXPECT_TRUE(ndgpp::safe_op::lt(1U, 3));

    EXPECT_FALSE(ndgpp::safe_op::lt(3U, 1));
    EXPECT_FALSE(ndgpp::safe_op::lt(3, 1U));
}

TEST(lt_test, different_types_equal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::lt(0, 0U));
    EXPECT_FALSE(ndgpp::safe_op::lt(0U, 0));
}

TEST(lte_test, same_types_unequal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::lte(1, 3));
    EXPECT_FALSE(ndgpp::safe_op::lte(3, 1));

    EXPECT_TRUE(ndgpp::safe_op::lte(1U, 3U));
    EXPECT_FALSE(ndgpp::safe_op::lte(3U, 1U));
}

TEST(lte_test, negative_value)
{
    EXPECT_TRUE(ndgpp::safe_op::lte(-1, 0U));
    EXPECT_FALSE(ndgpp::safe_op::lte(0U, -1));
}

TEST(lte_test, same_types_equal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::lte(0, 0));
    EXPECT_TRUE(ndgpp::safe_op::lte(0U, 0U));
}

TEST(lte_test, different_types_unequal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::lte(1, 3U));
    EXPECT_TRUE(ndgpp::safe_op::lte(1U, 3));

    EXPECT_FALSE(ndgpp::safe_op::lte(3U, 1));
    EXPECT_FALSE(ndgpp::safe_op::lte(3, 1U));
}

TEST(lte_test, different_types_equal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::lte(0, 0U));
    EXPECT_TRUE(ndgpp::safe_op::lte(0U, 0));
}

TEST(gt_test, same_types_unequal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::gt(1, 3));
    EXPECT_TRUE(ndgpp::safe_op::gt(3, 1));

    EXPECT_FALSE(ndgpp::safe_op::gt(1U, 3U));
    EXPECT_TRUE(ndgpp::safe_op::gt(3U, 1U));
}

TEST(gt_test, negative_value)
{
    EXPECT_TRUE(ndgpp::safe_op::gt(0U, -1));
    EXPECT_FALSE(ndgpp::safe_op::gt(-1, 0U));
}

TEST(gt_test, same_types_equal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::gt(0, 0));
    EXPECT_FALSE(ndgpp::safe_op::gt(0U, 0U));
}

TEST(gt_test, different_types_unequal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::gt(1, 3U));
    EXPECT_FALSE(ndgpp::safe_op::gt(1U, 3));

    EXPECT_TRUE(ndgpp::safe_op::gt(3U, 1));
    EXPECT_TRUE(ndgpp::safe_op::gt(3, 1U));
}

TEST(gt_test, different_types_equal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::gt(0, 0U));
    EXPECT_FALSE(ndgpp::safe_op::gt(0U, 0));
}

TEST(gte_test, same_types_unequal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::gte(1, 3));
    EXPECT_TRUE(ndgpp::safe_op::gte(3, 1));

    EXPECT_FALSE(ndgpp::safe_op::gte(1U, 3U));
    EXPECT_TRUE(ndgpp::safe_op::gte(3U, 1U));
}

TEST(gte_test, negative_value)
{
    EXPECT_TRUE(ndgpp::safe_op::gte(0U, -1));
    EXPECT_FALSE(ndgpp::safe_op::gte(-1, 0U));
}

TEST(gte_test, same_types_equal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::gte(0, 0));
    EXPECT_TRUE(ndgpp::safe_op::gte(0U, 0U));
}

TEST(gte_test, different_types_unequal_values)
{
    EXPECT_FALSE(ndgpp::safe_op::gte(1, 3U));
    EXPECT_FALSE(ndgpp::safe_op::gte(1U, 3));

    EXPECT_TRUE(ndgpp::safe_op::gte(3U, 1));
    EXPECT_TRUE(ndgpp::safe_op::gte(3, 1U));
}

TEST(gte_test, different_types_equal_values)
{
    EXPECT_TRUE(ndgpp::safe_op::gte(0, 0U));
    EXPECT_TRUE(ndgpp::safe_op::gte(0U, 0));
}
