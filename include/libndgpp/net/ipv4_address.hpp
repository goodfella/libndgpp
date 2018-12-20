#ifndef LIBNDGPP_NET_IPV4_ADDRESS_HPP
#define LIBNDGPP_NET_IPV4_ADDRESS_HPP

#include <libndgpp/net/basic_ipv4_address.hpp>

namespace ndgpp {
namespace net {

    using ipv4_address = ndgpp::net::basic_ipv4_address<>;
    extern template class ndgpp::net::basic_ipv4_address<>;
}}

#endif
