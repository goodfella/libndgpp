#include <gtest/gtest.h>

#include <libndgpp/error.hpp>
#include <libndgpp/net/multicast_ipv4_address.hpp>

TEST(ctor, ipv4_address_too_low)
{
    constexpr ndgpp::net::ipv4_address addr {ndgpp::net::ipv4_array {223,255,255,255}};
    auto will_throw = [&addr] () {const ndgpp::net::multicast_ipv4_address maddr {addr};};
    using expected_exception = ndgpp::error<std::out_of_range>;

    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, ipv4_address_too_high)
{
    constexpr ndgpp::net::ipv4_address addr {ndgpp::net::ipv4_array {240,0,0,0}};
    auto will_throw = [&addr] () {const ndgpp::net::multicast_ipv4_address maddr {addr};};
    using expected_exception = ndgpp::error<std::out_of_range>;

    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, value_type_invalid_too_high)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {240,0,0,0};
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {ipv4_array};};

    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, value_type_invalid_too_low)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {223,255,255,255};
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {ipv4_array};};

    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, uint32_invalid_too_high)
{
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {0xf0000000};};
    using expected_exception = ndgpp::error<std::out_of_range>;

    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, uint32_invalid_too_low)
{
    auto will_throw = [] () {const ndgpp::net::multicast_ipv4_address addr {0xdf000000};};
    using expected_exception = ndgpp::error<std::out_of_range>;

    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, value_type_too_low)
{
    constexpr ndgpp::net::ipv4_array expected {223,255,255,255};
    ndgpp::net::multicast_ipv4_address addr;
    auto will_throw = [&addr] () {addr = expected;};
    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, value_type_too_high)
{
    constexpr ndgpp::net::ipv4_array expected {240,0,0,0};
    ndgpp::net::multicast_ipv4_address addr;
    auto will_throw = [&addr] () {addr = expected;};
    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, uint32_too_low)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = 0xdfffffff;};

    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, uint32_too_high)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = 0xf0000000;};

    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, string_too_low)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = "223.255.255.255";};

    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, string_too_high)
{
    ndgpp::net::multicast_ipv4_address addr {};
    auto will_throw = [&addr] () {addr = "240.0.0.0";};

    using expected_exception = ndgpp::error<std::out_of_range>;
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
