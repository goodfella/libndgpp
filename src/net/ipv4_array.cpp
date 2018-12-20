#include <cinttypes>
#include <cstdio>

#include <stdexcept>
#include <tuple>

#include <libndgpp/error.hpp>
#include <libndgpp/net/ipv4_array.hpp>
#include <libndgpp/strto.hpp>

std::string ndgpp::net::to_string(const ndgpp::net::ipv4_array value)
{
    // 3 characters per byte, 3 period characters, and 1 null terminator
    std::array<char, (std::tuple_size<ndgpp::net::ipv4_array>::value * 3) + 3 + 1> buffer;
    const int bytes_written = std::snprintf(buffer.data(),
                                            buffer.size(),
                                            "%" PRIu32 "." "%" PRIu32 "." "%" PRIu32 "." "%" PRIu32,
                                            value[0],
                                            value[1],
                                            value[2],
                                            value[3]);

    return std::string{buffer.begin(), buffer.begin() + bytes_written};
}

ndgpp::net::ipv4_array ndgpp::net::make_ipv4_array(const std::string & address)
{
    ndgpp::net::ipv4_array octets;

    ndgpp::strto_result<uint8_t> result = ndgpp::strtoi<uint8_t>(address.c_str(), 10, ".");
    if (!result)
    {
        throw ndgpp_error(std::invalid_argument, "first octet is invalid");
    }

    octets[0] = result.value();

    for (std::size_t i = 1; i < std::tuple_size<ndgpp::net::ipv4_array>::value - 1; ++i)
    {
        result = ndgpp::strtoi<uint8_t>(result.unparsed() + 1, 10, ".");
        if (!result)
        {
            throw ndgpp_error(std::invalid_argument, "one of the middle octets is invalid");
        }

        octets[i] = result.value();
    }

    result = ndgpp::strtoi<uint8_t>(result.unparsed() + 1, 10, ":");
    if (!result)
    {
        throw ndgpp_error(std::invalid_argument, "last octet is invalid");
    }

    octets[3] = result.value();

    return octets;
}
