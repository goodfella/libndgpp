#include <stdexcept>
#include <iostream>

#include <libndgpp/error.hpp>
#include <libndgpp/strto.hpp>
#include <libndgpp/net/ipv4_address.hpp>

static ndgpp::net::ipv4_address::value_type parse_string(const std::string& address)
{
    ndgpp::net::ipv4_address::value_type octets;

    ndgpp::strto_result<uint8_t> result = ndgpp::strtoi<uint8_t>(address.c_str(), 10, ".");
    if (!result)
    {
        throw ndgpp_error(std::invalid_argument, "first octet is invalid");
    }

    octets[0] = result.value();

    for (std::size_t i = 1; i < std::tuple_size<ndgpp::net::ipv4_address::value_type>::value - 1; ++i)
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

ndgpp::net::ipv4_address::ipv4_address(const std::string & address):
    value_{parse_string(address)}
{}

ndgpp::net::ipv4_address & ndgpp::net::ipv4_address::operator= (const std::string& address)
{
    this->value_ = parse_string(address);
    return *this;
}
