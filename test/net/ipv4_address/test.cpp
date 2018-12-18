#include <gtest/gtest.h>

#include <libndgpp/error.hpp>
#include <libndgpp/net/ipv4_address.hpp>

TEST(ctor, default_ctor)
{
    ndgpp::net::ipv4_address addr {};
    const constexpr ndgpp::net::ipv4_address::value_type expected {};
    EXPECT_EQ(expected, addr.value());
}

TEST(ctor, value_type)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const ndgpp::net::ipv4_address addr {ipv4_array};

    EXPECT_EQ(ipv4_array, addr);
}

TEST(ctor, uint32)
{
    const ndgpp::net::ipv4_address addr {0xdeadbeaf};
    EXPECT_EQ(0xde, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);

    EXPECT_EQ(0xdeadbeaf, addr.to_uint32());
}

TEST(ctor, string_valid_no_colon)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const ndgpp::net::ipv4_address addr {"172.168.5.15"};
    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(ctor, string_valid_colon)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const std::string expected_string ("172.168.5.15");
    const ndgpp::net::ipv4_address addr {expected_string};
    EXPECT_EQ(ipv4_array, addr.value());
    EXPECT_EQ(expected_string, addr.to_string());
}

TEST(ctor, string_valid_colon_with_value)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {255,255,255,255};
    const ndgpp::net::ipv4_address addr {"255.255.255.255:15"};
    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(ctor, string_first_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"256.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_second_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.256.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_third_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.255.256.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_fourth_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.255.255.256:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_invalid_delimiter)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.255.255.255;15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_first_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"-1.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_second_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.-1.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_third_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.255.-1.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_fourth_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::ipv4_address addr {"255.255.255.-1:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, value_type)
{
    ndgpp::net::ipv4_address addr {};
    constexpr ndgpp::net::ipv4_array expected {1,2,3,4};
    addr = expected;
    EXPECT_EQ(expected, addr);
}

TEST(assignment, uint32)
{
    ndgpp::net::ipv4_address addr {};
    addr = 0xdeadbeaf;

    EXPECT_EQ(0xde, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);
}

TEST(assignment, string)
{
    constexpr ndgpp::net::ipv4_array expected {1,2,3,4};
    ndgpp::net::ipv4_address addr;
    addr = "1.2.3.4";
    EXPECT_EQ(expected, addr.value());
}

TEST(index_operator, set)
{
    ndgpp::net::ipv4_address addr {"1.2.3.4"};
    addr[0] = 192;
    EXPECT_EQ(192, addr[0]);
}
