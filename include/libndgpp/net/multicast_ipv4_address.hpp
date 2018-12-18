#ifndef LIBNDGPP_NET_MULTICAST_IPV4_ADDRESS_HPP
#define LIBNDGPP_NET_MULTICAST_IPV4_ADDRESS_HPP

#include <stdexcept>

#include <libndgpp/error.hpp>
#include <libndgpp/net/ipv4_address.hpp>

namespace ndgpp {
namespace net {

    /// IPv4 multicast address type
    class multicast_ipv4_address final
    {
        public:

        using value_type = ndgpp::net::ipv4_array;

        /// Constructs an address equal to 0.0.0.0
        constexpr multicast_ipv4_address() noexcept;

        explicit
        multicast_ipv4_address(const ndgpp::net::ipv4_address addr);

        /// Constructs an address equal to the array of octets passed in
        multicast_ipv4_address(const value_type octets);

        /** Constructs an address from an unsigned 32 bit integer
         *
         *  @param An unsigned 32 bit integer in network byte order
         */
        explicit
        constexpr multicast_ipv4_address(const uint32_t value);

        /** Constructs an address from a string
         *
         *  @param value A string representing an IP address in dotted
         *               quad notation.  The string can be optionally
         *               terminated by a colon.
         */
        explicit
        multicast_ipv4_address(const std::string & value);

        constexpr multicast_ipv4_address(const multicast_ipv4_address & other) noexcept;
        constexpr multicast_ipv4_address(multicast_ipv4_address && other) noexcept;

        multicast_ipv4_address & operator=(const multicast_ipv4_address & other) noexcept;
        multicast_ipv4_address & operator=(multicast_ipv4_address && other) noexcept;

        /// Assignes the address based on the value_type value
        multicast_ipv4_address & operator= (const value_type value);

        /// Assignes the address based on the passed in unsigned 32 bit network byte order value
        multicast_ipv4_address & operator= (const uint32_t value);

        /// Assignes the address based on the passed in dotted quad string value
        multicast_ipv4_address & operator= (const std::string& value);

        /** Returns the address value_type
         *
         *  @note Index 0 is the most significant byte
         */
        constexpr value_type value() const noexcept;

        /// Returns the address in network byte order as an unsigned 32 bit integer
        constexpr uint32_t to_uint32() const noexcept;

        /// Returns the address as a dotted quad string
        std::string to_string() const;

        /// Provides access to a given octet in the address
        constexpr uint8_t operator [](const std::size_t index) const noexcept;

        /// Swaps this with the passed in address
        void swap(multicast_ipv4_address & other) noexcept;

        constexpr operator ndgpp::net::ipv4_address() const noexcept;

        private:

        constexpr bool validate() const noexcept;

        ndgpp::net::ipv4_address address_ = ndgpp::net::ipv4_array{224,0,0,0};
    };

    inline constexpr multicast_ipv4_address::multicast_ipv4_address() noexcept = default;

    inline constexpr multicast_ipv4_address::multicast_ipv4_address(const ndgpp::net::multicast_ipv4_address &) noexcept = default;
    inline constexpr multicast_ipv4_address::multicast_ipv4_address(ndgpp::net::multicast_ipv4_address &&) noexcept = default;

    inline multicast_ipv4_address & ndgpp::net::multicast_ipv4_address::operator=(const ndgpp::net::multicast_ipv4_address &) noexcept = default;
    inline multicast_ipv4_address & ndgpp::net::multicast_ipv4_address::operator=(ndgpp::net::multicast_ipv4_address &&) noexcept = default;

    inline constexpr bool multicast_ipv4_address::validate() const noexcept
    {
        if (this->address_[0] < 224)
        {
            return false;
        }
        else if (this->address_[0] > 239)
        {
            return false;
        }

        return true;
    }

    inline multicast_ipv4_address::multicast_ipv4_address(const ndgpp::net::ipv4_address value):
        address_{value}
    {
        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value_type");
        }
    }

    inline multicast_ipv4_address::multicast_ipv4_address(const value_type value):
        address_{value}
    {
        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value_type");
        }
    }

    inline constexpr multicast_ipv4_address::multicast_ipv4_address(const uint32_t value):
        address_(value)
    {
        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value_type");
        }
    }

    inline ndgpp::net::multicast_ipv4_address::multicast_ipv4_address(const std::string & address):
        address_{address}
    {
        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value_type");
        }
    }

    inline ndgpp::net::multicast_ipv4_address & ndgpp::net::multicast_ipv4_address::operator= (const std::string& address)
    {
        this->address_ = address;
        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value");
        }

        return *this;
    }

    inline multicast_ipv4_address & multicast_ipv4_address::operator= (const value_type value)
    {
        this->address_ = value;

        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value");
        }

        return *this;
    }

    inline multicast_ipv4_address & multicast_ipv4_address::operator= (const uint32_t value)
    {
        this->address_ = value;

        if (!this->validate())
        {
            throw ndgpp_error(std::invalid_argument, "bad value");
        }

        return *this;
    }

    inline constexpr multicast_ipv4_address::value_type multicast_ipv4_address::value() const noexcept
    {
        return this->address_.value();
    }

    inline constexpr uint32_t multicast_ipv4_address::to_uint32() const noexcept
    {
        return this->address_.to_uint32();
    }

    inline std::string multicast_ipv4_address::to_string() const
    {
        return this->address_.to_string();
    }

    inline constexpr uint8_t multicast_ipv4_address::operator [](const std::size_t index) const noexcept
    {
        return this->address_[index];
    }

    inline void multicast_ipv4_address::swap(multicast_ipv4_address & other) noexcept
    {
        std::swap(this->address_, other.address_);
    }

    inline void swap(multicast_ipv4_address & lhs, multicast_ipv4_address & rhs) noexcept
    {
        lhs.swap(rhs);
    }

    inline std::ostream & operator <<(std::ostream & stream, const multicast_ipv4_address address)
    {
        stream << static_cast<uint16_t>(address[0]);
        for (std::size_t i = 1; i < std::tuple_size<multicast_ipv4_address::value_type>::value; ++i)
        {
            stream.put('.');
            stream << static_cast<uint16_t>(address[i]);
        }

        return stream;
    }

    inline bool operator ==(const multicast_ipv4_address lhs, const multicast_ipv4_address rhs)
    {
        return lhs.value() == rhs.value();
    }

    inline bool operator !=(const multicast_ipv4_address lhs, const multicast_ipv4_address rhs)
    {
        return !(lhs == rhs);
    }

    inline bool operator <(const multicast_ipv4_address lhs, const multicast_ipv4_address rhs)
    {
        return lhs.value() < rhs.value();
    }

    inline bool operator >=(const multicast_ipv4_address lhs, const multicast_ipv4_address rhs)
    {
        return !(lhs < rhs);
    }

    inline bool operator >(const multicast_ipv4_address lhs, const multicast_ipv4_address rhs)
    {
        return rhs < lhs;
    }

    inline bool operator <=(const multicast_ipv4_address lhs, const multicast_ipv4_address rhs)
    {
        return !(rhs < lhs);
    }

    inline constexpr ndgpp::net::multicast_ipv4_address::operator ndgpp::net::ipv4_address() const noexcept {
        return this->address_;
    }
}}

#endif
