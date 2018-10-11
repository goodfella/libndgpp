#include <limits>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include <libndgpp/strto.hpp>

/*
 * 1. type conversion {valid, invalid, overflow, underflow}
 * 2. change min and max
 * 3. oct, dec, and hex bases
 */

template <class T>
class conversion_test: public ::testing::Test
{
    public:

    using value_type = std::decay_t<T>;
    static constexpr bool is_signed = std::numeric_limits<value_type>::is_signed;
    using promoted_type = std::conditional<is_signed,
                                           signed long long,
                                           unsigned long long>;

    conversion_test()
        {
            if (std::is_same<T, signed char>::value)
            {
                overflow_value << static_cast<signed short>(std::numeric_limits<value_type>::max()) << '0';
            }
            else if (std::is_same<T, unsigned char>::value)
            {
                overflow_value << static_cast<unsigned short>(std::numeric_limits<value_type>::max()) << '0';
            }
            else
            {
                overflow_value << std::numeric_limits<value_type>::max() << '0';
            }

            if (is_signed)
            {
                if (std::is_same<T, signed char>::value)
                {
                    underflow_value << static_cast<signed short>(std::numeric_limits<value_type>::min()) << '0';
                }
                else if (std::is_same<T, unsigned char>::value)
                {
                    underflow_value << static_cast<unsigned short>(std::numeric_limits<value_type>::min()) << '0';
                }
                else
                {
                    underflow_value << std::numeric_limits<value_type>::min() << '0';
                }
            }
            else
            {
                underflow_value << "-1";
            }
        }

    std::ostringstream overflow_value;
    std::ostringstream underflow_value;
};

using conversion_test_types = ::testing::Types<signed long long,
                                               signed long,
                                               signed int,
                                               signed short,
                                               signed char,
                                               unsigned long long,
                                               unsigned long,
                                               unsigned int,
                                               unsigned short,
                                               unsigned char>;

TYPED_TEST_CASE(conversion_test, conversion_test_types);

TYPED_TEST(conversion_test, valid)
{
    const auto result = ndgpp::strtoi<typename TestFixture::value_type>("1");

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    EXPECT_EQ(static_cast<typename TestFixture::value_type>(1), result.value());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());
}

TYPED_TEST(conversion_test, invalid)
{
    const auto result = ndgpp::strtoi<typename TestFixture::value_type>("z");

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_TRUE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());

    EXPECT_THROW(result.value(), ndgpp::error<std::logic_error>);
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('z', *result.unparsed());
}

TYPED_TEST(conversion_test, underflow)
{
    const std::string val = this->underflow_value.str();
    const auto result = ndgpp::strtoi<typename TestFixture::value_type>(val.c_str());

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_TRUE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());

    if (std::numeric_limits<typename TestFixture::value_type>::is_signed)
    {
        EXPECT_EQ('\0', *result.unparsed());
    }
    else
    {
        EXPECT_EQ('-', *result.unparsed());
    }

    EXPECT_THROW(result.value(), ndgpp::error<std::logic_error>);
}

TYPED_TEST(conversion_test, overflow)
{
    const std::string val = this->overflow_value.str();
    const auto result = ndgpp::strtoi<typename TestFixture::value_type>(val.c_str());

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_TRUE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_THROW(result.value(), ndgpp::error<std::logic_error>);
}

TYPED_TEST(conversion_test, restricted_min)
{
    const auto result = ndgpp::strtoi<typename TestFixture::value_type, 1>("0");

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_TRUE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_THROW(result.value(), ndgpp::error<std::logic_error>);
}

TYPED_TEST(conversion_test, restricted_max)
{
    const auto result = ndgpp::strtoi<typename TestFixture::value_type, 1, 2>("3");

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_TRUE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_THROW(result.value(), ndgpp::error<std::logic_error>);
}

TEST(default_base_test, octal)
{
    const auto result = ndgpp::strtoi<unsigned int>("010");

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_EQ(8U, result.value());
}

TEST(default_base_test, decimal)
{
    const auto result = ndgpp::strtoi<unsigned int>("10");

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_EQ(10U, result.value());
}

TEST(default_base_test, hex)
{
    const auto result = ndgpp::strtoi<unsigned int>("0x10");

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_EQ(16U, result.value());
}

TEST(specific_base_test, octal)
{
    const auto result = ndgpp::strtoi<unsigned int>("10", 8);

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_EQ(8U, result.value());
}

TEST(specific_base_test, decimal)
{
    const auto result = ndgpp::strtoi<unsigned int>("10", 10);

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_EQ(10U, result.value());
}

TEST(specific_base_test, hex)
{
    const auto result = ndgpp::strtoi<unsigned int>("10", 16);

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());

    EXPECT_EQ(16U, result.value());
}

TEST(delimiter_test, single_char)
{
    const auto result = ndgpp::strtoi<unsigned int>("10:1", 0, ":");
    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ(':', *result.unparsed());

    EXPECT_EQ(10U, result.value());
}

TEST(delimiter_test, multiple_char)
{
    const auto result = ndgpp::strtoi<unsigned int>("10-1", 0, ":-");
    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('-', *result.unparsed());

    EXPECT_EQ(10U, result.value());
}

TEST(delimiter_test, container)
{
    std::vector<char> delims = {':', '-'};
    const auto result = ndgpp::strtoi<unsigned int>("10-1", 0, delims.cbegin(), delims.cend());

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_FALSE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('-', *result.unparsed());

    EXPECT_EQ(10U, result.value());
}

TEST(input_string_test, empty_string)
{
    const auto result = ndgpp::strtoi<unsigned int>("");

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_TRUE(result.invalid());
    EXPECT_FALSE(result.underflow());
    EXPECT_FALSE(result.overflow());
    ASSERT_NE(nullptr, result.unparsed());
    EXPECT_EQ('\0', *result.unparsed());
}
