# Introduction

A library containing C++ general primitives that I use.  Some things
that might be of use to others include but are not limited to:

- [ndgpp::bounded_integer](include/libndgpp/bounded_integer.hpp)
- [ndgpp::strto](include/libndgpp/strto.hpp)
- [ndgpp::safe_op](include/libndgpp/safe_operators.hpp)

The library also contains networking primitives that I use.  Some
things that might be of interest to others include but are not limited
to:

- [ndgpp::net::basic\_ipv4\_address](include/libndgpp/net/basic_ipv4_address.hpp)
- [ndgpp::net::ipv4_address](include/libndgpp/net/ipv4_address.hpp)
- [ndgpp::net::multicast\_ipv4\_address](include/libndgpp/net/multicast_ipv4_address.hpp)
- [ndgpp::net::port](include/libndgpp/net/port.hpp)

### General Primitives

#### ndgpp::bounded_integer

An integer type that allows for a compile time specifications of the
allowed value range.  This class faciliates proper construction from
an arbitraty integer type and has safe signed / unsigned comparisions.
It also facilitates integer construction from "string" types.

#### ndgpp::strto

A function overload set that facilitates converting strings to numeric
types.  These functions are convenient because they allow for compile
time specification of the allowed value range as well as a requiring a
template parameter to specify the target integer type.  The target
integer type template parameter facilitates string to integer
conversions in template functions because the template function does
not have to use a different function name for each target integer type.

#### ndgpp::safe_op

A namespace that contains several functions that perform safe
comparisons on integer types that differ in signedness.

#### ndgpp::network_byte_order

A integral type that stores an integral value in network byte order.

### Networking Primitives

#### ndgpp::net::basic\_ipv4\_address

This type represents an IPv4 address and allows for specifying the
range of values the IP address can take on.

#### ndgpp::net::ipv4_address

This type represents an IPv4 address that can take on any IPv4 address value

#### ndgpp::net::multicast\_ipv4\_address

This type represents a multicast IPv4 address that can only take on
address values that are in the IPv4 multicast address range.

#### ndgpp::net::port

This type represents a network port.
