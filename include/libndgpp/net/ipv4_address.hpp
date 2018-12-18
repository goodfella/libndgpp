#ifndef LIBNDGPP_NET_IPV4_ADDRESS_HPP
#define LIBNDGPP_NET_IPV4_ADDRESS_HPP

#include <cinttypes>
#include <cstdint>
#include <cstdio>

#include <array>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include <libndgpp/net/ipv4_array.hpp>

namespace ndgpp { namespace net {

    /** IPv4 address value type
     *
     *  @par Move Semantics
     *  Move operations behave just like copy operations
     */
    class ipv4_address final
    {
        public:

        using value_type = ndgpp::net::ipv4_array;

        /// Constructs an address equal to 0.0.0.0
        constexpr ipv4_address() noexcept;

        /// Constructs an address equal to the array of octets passed in
        constexpr ipv4_address(const value_type octets) noexcept;

        /** Constructs an address from an unsigned 32 bit integer
         *
         *  @param An unsigned 32 bit integer in network byte order
         */
        explicit
        constexpr ipv4_address(const uint32_t value) noexcept;

        /** Constructs an address from a string
         *
         *  @param value A string representing an IP address in dotted
         *               quad notation.  The string can be optionally
         *               terminated by a colon.
         */
        explicit
        ipv4_address(const std::string & value);

        constexpr ipv4_address(const ipv4_address & other) noexcept;
        constexpr ipv4_address(ipv4_address && other) noexcept;

        ipv4_address & operator=(const ipv4_address & other) noexcept;
        ipv4_address & operator=(ipv4_address && other) noexcept;

        /// Assignes the address based on the value_type value
        ipv4_address & operator= (const value_type value) noexcept;

        /// Assignes the address based on the passed in unsigned 32 bit network byte order value
        ipv4_address & operator= (const uint32_t value) noexcept;

        /// Assignes the address based on the passed in dotted quad string value
        ipv4_address & operator= (const std::string& value);

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

        /// Provides access to a given octet in the address
        uint8_t & operator [](const std::size_t index) noexcept;

        /// Swaps this with the passed in address
        void swap(ipv4_address & other) noexcept;

        private:

        value_type value_ = {};
    };

    inline constexpr ipv4_address::ipv4_address() noexcept = default;

    inline constexpr ipv4_address::ipv4_address(const ndgpp::net::ipv4_address &) noexcept = default;
    inline ipv4_address & ndgpp::net::ipv4_address::operator=(const ndgpp::net::ipv4_address &) noexcept = default;

    inline constexpr ipv4_address::ipv4_address(ndgpp::net::ipv4_address &&) noexcept = default;
    inline ipv4_address & ndgpp::net::ipv4_address::operator=(ndgpp::net::ipv4_address &&) noexcept = default;

    inline constexpr ipv4_address::ipv4_address(const value_type value) noexcept:
        value_{value}
    {}

    inline constexpr ipv4_address::ipv4_address(const uint32_t value) noexcept:
        value_{static_cast<uint8_t>((value & 0xff000000) >> 24U),
               static_cast<uint8_t>((value & 0x00ff0000) >> 16U),
               static_cast<uint8_t>((value & 0x0000ff00) >> 8U),
               static_cast<uint8_t>((value & 0x000000ff))}
    {}

    inline ipv4_address & ipv4_address::operator= (const value_type value) noexcept
    {
        this->value_ = value;
        return *this;
    }

    inline ipv4_address & ipv4_address::operator= (const uint32_t value) noexcept
    {
        this->value_[0] = static_cast<uint8_t>((value & 0xff000000) >> 24U);
        this->value_[1] = static_cast<uint8_t>((value & 0x00ff0000) >> 16U);
        this->value_[2] = static_cast<uint8_t>((value & 0x0000ff00) >> 8U);
        this->value_[3] = static_cast<uint8_t>((value & 0x000000ff));
        return *this;
    }

    inline constexpr ipv4_address::value_type ipv4_address::value() const noexcept
    {
        return this->value_;
    }

    inline constexpr uint32_t ipv4_address::to_uint32() const noexcept
    {
        return
            (static_cast<uint32_t>(this->value_[0]) << 24) |
            (static_cast<uint32_t>(this->value_[1]) << 16) |
            (static_cast<uint32_t>(this->value_[2]) << 8) |
            (static_cast<uint32_t>(this->value_[3]));
    }

    inline std::string ipv4_address::to_string() const
    {
        // 3 characters per byte, 3 period characters, and 1 null terminator
        std::array<char, (std::tuple_size<value_type>::value * 3) + 3 + 1> buffer;
        const int bytes_written = std::snprintf(buffer.data(),
                                                buffer.size(),
                                                "%" PRIu32 "." "%" PRIu32 "." "%" PRIu32 "." "%" PRIu32,
                                                this->value_[0],
                                                this->value_[1],
                                                this->value_[2],
                                                this->value_[3]);

        return std::string{buffer.begin(), buffer.begin() + bytes_written};
    }

    inline constexpr uint8_t ipv4_address::operator [](const std::size_t index) const noexcept
    {
        return this->value_[index];
    }

    inline uint8_t & ipv4_address::operator [](const std::size_t index) noexcept
    {
        return this->value_[index];
    }

    inline void ipv4_address::swap(ipv4_address & other) noexcept
    {
        std::swap(this->value_, other.value_);
    }

    inline void swap(ipv4_address & lhs, ipv4_address & rhs) noexcept
    {
        lhs.swap(rhs);
    }

    inline std::ostream & operator <<(std::ostream & stream, const ipv4_address address)
    {
        stream << static_cast<uint16_t>(address[0]);
        for (std::size_t i = 1; i < std::tuple_size<ipv4_address::value_type>::value; ++i)
        {
            stream.put('.');
            stream << static_cast<uint16_t>(address[i]);
        }

        return stream;
    }

    inline bool operator ==(const ipv4_address lhs, const ipv4_address rhs)
    {
        return lhs.value() == rhs.value();
    }

    inline bool operator !=(const ipv4_address lhs, const ipv4_address rhs)
    {
        return !(lhs == rhs);
    }

    inline bool operator <(const ipv4_address lhs, const ipv4_address rhs)
    {
        return lhs.value() < rhs.value();
    }

    inline bool operator >=(const ipv4_address lhs, const ipv4_address rhs)
    {
        return !(lhs < rhs);
    }

    inline bool operator >(const ipv4_address lhs, const ipv4_address rhs)
    {
        return rhs < lhs;
    }

    inline bool operator <=(const ipv4_address lhs, const ipv4_address rhs)
    {
        return !(rhs < lhs);
    }
}}

#endif
