#include <sstream>
#include <utility>
#include <gtest/gtest.h>

#include <libndgpp/error.hpp>
#include <libndgpp/net/basic_ipv4_address.hpp>

TEST(ctor, default_ctor)
{
    ndgpp::net::basic_ipv4_address<> addr {};
    const constexpr ndgpp::net::basic_ipv4_address<>::value_type expected {};
    EXPECT_EQ(expected, addr.value());
}

TEST(ctor, value_type)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const ndgpp::net::basic_ipv4_address<> addr {ipv4_array};

    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(constrained_ctor, value_type)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const ndgpp::net::basic_ipv4_address<0xac000000> addr {ipv4_array};

    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(constrained_ctor, value_type_to_low)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {223,168,5,15};

    auto throws = [ipv4_array] () {ndgpp::net::basic_ipv4_address<0xe0000000> addr {ipv4_array};};

    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(constrained_ctor, value_type_to_high)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {224,0,0,2};

    auto throws = [ipv4_array] () {ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr {ipv4_array};};

    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(ctor, uint32)
{
    const ndgpp::net::basic_ipv4_address<> addr {0xdeadbeaf};
    EXPECT_EQ(0xde, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);

    EXPECT_EQ(0xdeadbeaf, addr.to_uint32());
}

TEST(constrained_ctor, uint32)
{
    const ndgpp::net::basic_ipv4_address<0xde000000> addr {0xdeadbeaf};
    EXPECT_EQ(0xde, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);

    EXPECT_EQ(0xdeadbeaf, addr.to_uint32());
}

TEST(constrained_ctor, uint32_to_low)
{
    auto throws = [] () {ndgpp::net::basic_ipv4_address<0xe0000000> addr {0xdfffffff};};

    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(constrained_ctor, uint32_to_high)
{
    auto throws = [] () {ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr {0xe0000002};};

    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(ctor, string_valid_no_colon)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const ndgpp::net::basic_ipv4_address<> addr {"172.168.5.15"};
    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(ctor, string_valid_colon)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {172,168,5,15};
    const std::string expected_string ("172.168.5.15");
    const ndgpp::net::basic_ipv4_address<> addr {expected_string};
    EXPECT_EQ(ipv4_array, addr.value());
    EXPECT_EQ(expected_string, addr.to_string());
}

TEST(ctor, string_valid_colon_with_value)
{
    constexpr ndgpp::net::ipv4_array ipv4_array {255,255,255,255};
    const ndgpp::net::basic_ipv4_address<> addr {"255.255.255.255:15"};
    EXPECT_EQ(ipv4_array, addr.value());
}

TEST(ctor, string_first_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"256.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_second_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.256.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_third_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.255.256.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_fourth_octet_overflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.255.255.256:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_invalid_delimiter)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.255.255.255;15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_first_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"-1.255.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_second_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.-1.255.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_third_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.255.-1.255:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(ctor, string_fourth_octet_underflow)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<> addr {"255.255.255.-1:15"};};
    using expected_exception = ndgpp::error<std::invalid_argument>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(copy_ctor, same_type)
{
    constexpr ndgpp::net::basic_ipv4_address<> addr1 {0xe0000000};
    constexpr ndgpp::net::basic_ipv4_address<> addr2 {addr1};

    EXPECT_EQ(addr1, addr2);
}

TEST(copy_ctor, different_type)
{
    constexpr ndgpp::net::basic_ipv4_address<> addr1 {0xe0000000};
    constexpr ndgpp::net::basic_ipv4_address<0xe0000000> addr2 {addr1};

    EXPECT_EQ(addr1.value(), addr2.value());
}

TEST(copy_ctor, different_type_too_low)
{
    constexpr ndgpp::net::basic_ipv4_address<> addr1 {0xe0000000};

    auto throws = [addr1] () {
        ndgpp::net::basic_ipv4_address<0xe1000000> addr2 {addr1};
    };

    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(copy_ctor, different_type_too_high)
{
    constexpr ndgpp::net::basic_ipv4_address<> addr1 {0xe0000002};

    auto throws = [addr1] () {
        ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr2 {addr1};
    };

    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(constrained_ctor, string_too_low)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<0xe0000000> addr {"223.255.255.255"};};
    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(constrained_ctor, string_too_high)
{
    const auto will_throw = [] () {const ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr {"224.0.0.2"};};
    using expected_exception = ndgpp::error<std::out_of_range>;
    EXPECT_THROW(will_throw(), expected_exception);
}

TEST(assignment, value_type)
{
    ndgpp::net::basic_ipv4_address<> addr {};
    constexpr ndgpp::net::ipv4_array expected {1,2,3,4};
    addr = expected;
    EXPECT_EQ(expected, addr.value());
}

TEST(constrained_assignment, value_type)
{
    ndgpp::net::basic_ipv4_address<0xe0000000> addr {};
    constexpr ndgpp::net::ipv4_array expected {224,2,3,4};
    addr = expected;
    EXPECT_EQ(expected, addr.value());
}

TEST(constrained_assignment, value_type_too_low)
{
    ndgpp::net::basic_ipv4_address<0xe0000000> addr {};
    auto throws = [&addr] () {addr = ndgpp::net::ipv4_array {223, 0, 0, 0};};
    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(constrained_assignment, value_type_too_high)
{
    ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr {};
    auto throws = [&addr] () {addr = ndgpp::net::ipv4_array {224, 0, 0, 2};};
    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(assignment, uint32)
{
    ndgpp::net::basic_ipv4_address<> addr {};
    addr = 0xdeadbeaf;

    EXPECT_EQ(0xde, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);
}

TEST(constrained_assignment, uint32)
{
    ndgpp::net::basic_ipv4_address<0xde000000> addr {};
    addr = 0xdeadbeaf;

    EXPECT_EQ(0xde, addr.value()[0]);
    EXPECT_EQ(0xad, addr.value()[1]);
    EXPECT_EQ(0xbe, addr.value()[2]);
    EXPECT_EQ(0xaf, addr.value()[3]);
}

TEST(constrained_assignment, uint32_too_low)
{
    ndgpp::net::basic_ipv4_address<0xe0000000> addr {};

    auto throws = [&addr] () {addr = 0xdfffffff;};
    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(constrained_assignment, uint32_too_high)
{
    ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr {};

    auto throws = [&addr] () {addr = 0xe0000002;};
    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(assignment, string)
{
    constexpr ndgpp::net::ipv4_array expected {1,2,3,4};
    ndgpp::net::basic_ipv4_address<> addr;
    addr = "1.2.3.4";
    EXPECT_EQ(expected, addr.value());
}

TEST(constrained_assignment, string)
{
    constexpr ndgpp::net::ipv4_array expected {224,2,3,4};
    ndgpp::net::basic_ipv4_address<0xe0000000> addr;
    addr = "224.2.3.4";
    EXPECT_EQ(expected, addr.value());
}

TEST(constrained_assignment, string_too_low)
{
    ndgpp::net::basic_ipv4_address<0xe0000000> addr;
    auto throws = [&addr] () {addr = "223.0.0.0";};
    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}


TEST(constrained_assignment, string_too_high)
{
    ndgpp::net::basic_ipv4_address<0xe0000000, 0xe0000001> addr;
    auto throws = [&addr] () {addr = "223.0.0.2";};
    EXPECT_THROW(throws(), ndgpp::error<std::out_of_range>);
}

TEST(constrained, test)
{
    static_assert(ndgpp::net::basic_ipv4_address<0x10000000>::constrained,
                  "bug in ndgpp::net::basic_ipv4_address constraint checking");

    static_assert(ndgpp::net::basic_ipv4_address<0x00000000, 0xfffffffe>::constrained,
                  "bug in ndgpp::net::basic_ipv4_address constraint checking");

    static_assert(ndgpp::net::basic_ipv4_address<0x10000000, 0xfffffffe>::constrained,
                  "bug in ndgpp::net::basic_ipv4_address constraint checking");
}

TEST(to_type, uint32)
{
    constexpr ndgpp::net::basic_ipv4_address<> addr {ndgpp::net::ipv4_array{224,0,0,0}};
    EXPECT_EQ(0xe0000000, addr.to_uint32());
}

TEST(to_type, string)
{
    const std::string expected {"224.0.0.0"};
    const ndgpp::net::basic_ipv4_address<> addr {0xe0000000};
    EXPECT_EQ(expected, addr.to_string());
}

TEST(swap, test)
{
    ndgpp::net::basic_ipv4_address<> addr1 {0xe0000000};
    ndgpp::net::basic_ipv4_address<> addr2 {0xdf000000};

    std::swap(addr1, addr2);

    EXPECT_EQ(0xdf000000, addr1.to_uint32());
    EXPECT_EQ(0xe0000000, addr2.to_uint32());
}

TEST(comparison, test)
{
    ndgpp::net::basic_ipv4_address<> small {0xdf000000};
    ndgpp::net::basic_ipv4_address<> large {0xe0000000};

    EXPECT_EQ(small, small);
    EXPECT_LT(small, large);
    EXPECT_TRUE(small <= large);
    EXPECT_TRUE(small <= small);

    EXPECT_GT(large, small);
    EXPECT_TRUE(large >= small);
    EXPECT_TRUE(large >= large);
}

TEST(ostream, test)
{
    const std::string expected {"224.1.2.3"};
    const ndgpp::net::basic_ipv4_address<> addr {expected};
    std::stringstream ss;
    ss << addr;
    EXPECT_EQ(expected, ss.str());
}
