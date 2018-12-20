#ifndef LIBNDGPP_NET_MULTICAST_IPV4_ADDRESS_HPP
#define LIBNDGPP_NET_MULTICAST_IPV4_ADDRESS_HPP

#include <stdexcept>

#include <libndgpp/error.hpp>
#include <libndgpp/net/basic_ipv4_address.hpp>
#include <libndgpp/net/ipv4_address.hpp>

namespace ndgpp {
namespace net {

    using multicast_ipv4_address = ndgpp::net::basic_ipv4_address<0xe0000000, 0xefffffff>;
    extern template class ndgpp::net::basic_ipv4_address<0xe0000000, 0xefffffff>;
}}

#endif
