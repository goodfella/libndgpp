#ifndef LIBNDGPP_NET_PORT_HPP
#define LIBNDGPP_NET_PORT_HPP

#include <cstdint>
#include <limits>
#include <libndgpp/bounded_integer.hpp>

namespace ndgpp
{
namespace net
{
    namespace detail
    {
        struct port_tag;
    }

    using port = ndgpp::bounded_integer<uint16_t,
                                        std::numeric_limits<uint16_t>::min(),
                                        std::numeric_limits<uint16_t>::max(),
                                        ndgpp::net::detail::port_tag>;
}
}

#endif
