#include <type_traits>
#include <limits>
#include <gtest/gtest.h>

#include <libndgpp/type_traits/char_numeric_type.hpp>

TEST(unsigned_char_type, test)
{
    using type = ndgpp::char_numeric_type<unsigned char>::type;
    constexpr bool is_same = std::is_same<type, unsigned short>::value;
    EXPECT_TRUE(is_same);
}

TEST(signed_char_type, test)
{
    using type = ndgpp::char_numeric_type<signed char>::type;
    constexpr bool is_same = std::is_same<type, signed short>::value;
    EXPECT_TRUE(is_same);
}

TEST(char_type, test)
{
    using type = ndgpp::char_numeric_type<char>::type;
    constexpr bool is_signed = std::numeric_limits<char>::is_signed;
    if (is_signed)
    {
        constexpr bool is_same = std::is_same<type, signed short>::value;
        EXPECT_TRUE(is_same);
    }
    else
    {
        constexpr bool is_same = std::is_same<type, unsigned short>::value;
        EXPECT_TRUE(is_same);
    }
}
