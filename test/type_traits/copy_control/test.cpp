#include <type_traits>
#include <limits>
#include <gtest/gtest.h>

#include <libndgpp/type_traits/copy_control.hpp>

struct non_copyable
{
    non_copyable() = default;
    non_copyable(const non_copyable &) = delete;
    non_copyable & operator= (const non_copyable &) = delete;
};

struct copyable {};

TEST(copy_control, non_copyable_test)
{
    using copy_control = ndgpp::copy_control<non_copyable>;
    constexpr bool is_copy_constructible = std::is_copy_constructible<copy_control>::value;
    constexpr bool is_copy_assignable = std::is_copy_assignable<copy_control>::value;
    EXPECT_FALSE(is_copy_constructible);
    EXPECT_FALSE(is_copy_assignable);
}

TEST(copy_control, copyable_test)
{
    using copy_control = ndgpp::copy_control<copyable>;
    constexpr bool is_copy_constructible = std::is_copy_constructible<copy_control>::value;
    constexpr bool is_copy_assignable = std::is_copy_assignable<copy_control>::value;
    EXPECT_TRUE(is_copy_constructible);
    EXPECT_TRUE(is_copy_assignable);
}
