#include <gtest/gtest.h>

#include <libndgpp/error.hpp>
#include <libndgpp/net/multicast_ipv4_address.hpp>

TEST(ctor, default_ctor)
{
    ndgpp::net::multicast_ipv4_address addr {};
    const constexpr ndgpp::net::multicast_ipv4_address::value_type expected {224,0,0,0};
    EXPECT_EQ(expected, addr.value());
}

TEST(ctor, value_type)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {224,168,5,15};
    const ndgpp::net::multicast_ipv4_address addr {ipv4_array};

    EXPECT_EQ(ipv4_array, addr);
}

TEST(ctor, value_type_invalid_too_high)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {240,168,5,15};
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {ipv4_array};};

    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, value_type_invalid_too_low)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {223,168,5,15};
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {ipv4_array};};

    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, uint32_invalid_too_high)
{
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {0xf0000000};};
    using expected_exception = ndgpp::error<std::invalid_argument>;

    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, uint32_invalid_too_low)
{
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {0xdf000000};};
    using expected_exception = ndgpp::error<std::invalid_argument>;

    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_valid_no_colon)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {224,168,5,15};
    const ndgpp::net::multicast_ipv4_address addr {"224.168.5.15"};
    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(ctor, string_valid_colon)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {239,168,5,15};
    const std::string expected_string ("239.168.5.15");
    const ndgpp::net::multicast_ipv4_address addr {expected_string};
    EXPECT_EQ(ipv4_array, addr.value());
    EXPECT_EQ(expected_string, addr.to_string());
}

TEST(ctor, string_valid_colon_with_value)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {239,255,255,255};
    const ndgpp::net::multicast_ipv4_address addr {"239.255.255.255:15"};
    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(ctor, string_invalid_too_low)
{
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"223.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_invalid_too_high)
{
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"240.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_first_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"256.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_second_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.256.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_third_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.255.256.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_fourth_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.255.255.256:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_invalid_delimiter)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.255.255.255;15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_first_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"-1.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_second_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.-1.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_third_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.255.-1.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_fourth_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {"224.255.255.-1:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, value_type)
{
    ndgpp::net::multicast_ipv4_address addr {};
    constexpr ndgpp::net::ipv4_array expected {224,2,3,4};
    addr = expected;
    EXPECT_EQ(expected, addr);
}

TEST(assignment, value_type_too_low)
{
    constexpr ndgpp::net::ipv4_array expected {223,2,3,4};
    ndgpp::net::multicast_ipv4_address addr;
    auto will_throw = [&addr] () {addr = expected;};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, value_type_too_high)
{
    constexpr ndgpp::net::ipv4_array expected {240,2,3,4};
    ndgpp::net::multicast_ipv4_address addr;
    auto will_throw = [&addr] () {addr = expected;};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, uint32)
{
    ndgpp::net::multicast_ipv4_address addr {};
    addr = 0xe0adbeaf;

    EXPECT_EQ(0xe0, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);
}

TEST(assignment, uint32_too_low)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = 0xdfadbeaf;};

    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, uint32_too_high)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = 0xf0adbeaf;};

    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, string)
{
    constexpr ndgpp::net::ipv4_array expected {224,2,3,4};
    ndgpp::net::multicast_ipv4_address addr;
    addr = "224.2.3.4";
    EXPECT_EQ(expected, addr.value());
}

TEST(assignment, string_too_low)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = "223.0.0.0";};

    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, string_too_high)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = "240.0.0.0";};

    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(cast, ipv4_address)
{
    ndgpp::net::multicast_ipv4_address maddr {"239.2.3.4"};
    ndgpp::net::ipv4_address addr {maddr};
    EXPECT_EQ(addr.value(), maddr.value());

    maddr = ndgpp::net::ipv4_array{224,2,3,4};
    addr = maddr;
    EXPECT_EQ(addr.value(), maddr.value());
}
