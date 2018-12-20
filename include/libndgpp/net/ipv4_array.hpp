#ifndef LIBNDGPP_NET_IPV4_ARRAY_HPP
#define LIBNDGPP_NET_IPV4_ARRAY_HPP


#include <cstdint>

#include <array>
#include <string>

namespace ndgpp {
namespace net {

    using ipv4_array = std::array<uint8_t, 4>;

    std::string to_string(const ipv4_array value);

    inline constexpr uint32_t to_uint32(const ipv4_array value)
    {
        return (static_cast<uint32_t>(value[0]) << 24 |
                static_cast<uint32_t>(value[1]) << 16 |
                static_cast<uint32_t>(value[2]) << 8 |
                static_cast<uint32_t>(value[3]));
    }

    ipv4_array make_ipv4_array(const std::string & value);

    inline constexpr ipv4_array make_ipv4_array(const uint32_t value) noexcept
    {
        return ndgpp::net::ipv4_array {static_cast<uint8_t>((value & 0xff000000) >> 24),
                static_cast<uint8_t>((value & 0x00ff0000) >> 16),
                static_cast<uint8_t>((value & 0x0000ff00) >> 8),
                static_cast<uint8_t>(value & 0x000000ff)};
    }
}}

#endif
