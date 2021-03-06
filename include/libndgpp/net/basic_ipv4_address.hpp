#ifndef LIBNDGPP_NET_BASIC_IPV4_ADDRESS_HPP
#define LIBNDGPP_NET_BASIC_IPV4_ADDRESS_HPP

#include <cstdint>
#include <array>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <libndgpp/error.hpp>
#include <libndgpp/net/ipv4_array.hpp>

namespace ndgpp {
namespace net {

    template <uint32_t Min, uint32_t Max>
    struct basic_ipv4_address_validator
    {
        constexpr
        bool
        operator () (uint32_t value)
        {
            if (value < Min || value > Max)
            {
                throw ndgpp_error(std::out_of_range, "supplied address out of range");
            }

            return true;
        }
    };

    template <>
    struct basic_ipv4_address_validator<0, 0xffffffff>
    {
        constexpr
        bool
        operator () (uint32_t value) noexcept
        {
            return true;
        }
    };

    /** IPv4 address value type
     *
     *  @tparam Min The minimum address value in network byte order
     *  @tparam Max The maximum address value in network byte order
     *
     *  @par Move Semantics
     *  Move operations behave just like copy operations
     */
    template <uint32_t Min = 0, uint32_t Max = 0xffffffff>
    class basic_ipv4_address final
    {
        public:

        using value_type = ndgpp::net::ipv4_array;

        /// The minimum address value in network byte order
        static constexpr uint32_t min = Min;

        /// The maximum address value in network byte order
        static constexpr uint32_t max = Max;

        using constrained_bool_type = std::conditional_t<Min == 0x0 && Max == 0xffffffff,
                                                         std::false_type,
                                                         std::true_type>;

        static constexpr bool constrained = constrained_bool_type::value;

        /// Constructs an address assigned to Min
        constexpr basic_ipv4_address() noexcept;

        template <uint32_t MinO, uint32_t MaxO>
        constexpr basic_ipv4_address(const basic_ipv4_address<MinO, MaxO> & other) noexcept(MinO >= Min && MaxO <= Max);

        /// Constructs an address assigned to the value provided
        constexpr basic_ipv4_address(const value_type value) noexcept(!constrained);

        /** Constructs an address from an unsigned 32 bit integer
         *
         *  The parameter's value is mapped to the address octet like
         *  so:
         *
         *   address[0] = (value >> 24) & 0xff000000;
         *   address[1] = (value >> 16) & 0x00ff0000;
         *   address[2] = (value >> 8)  & 0x0000ff00;
         *   address[3] = value         & 0x000000ff;
         *
         *  @param An unsigned 32 bit integer
         */
        explicit
        constexpr basic_ipv4_address(const uint32_t value) noexcept(!constrained);

        /** Constructs an address from a string
         *
         *  @param value A string representing an IP address in dotted
         *               quad notation.  The string can be optionally
         *               terminated by a colon.
         */
        explicit
        basic_ipv4_address(const std::string & value);

        basic_ipv4_address & operator = (const basic_ipv4_address & value) noexcept;

        /// Assignes the address based on the value_type value
        basic_ipv4_address & operator = (const value_type value) noexcept(!constrained);

        /// Assignes the address based on the passed in unsigned 32 bit value
        basic_ipv4_address & operator = (const uint32_t value) noexcept (!constrained);

        /// Assignes the address based on the passed in dotted quad string value
        basic_ipv4_address & operator = (const std::string & value);

        void swap(basic_ipv4_address & other) noexcept;

        constexpr uint8_t operator[] (const std::size_t index) const noexcept;

        constexpr ndgpp::net::ipv4_array value() const noexcept;

        /** Returns the address as an unsigned 32 bit integer
         *
         *  The address is mapped to the value like so:
         *
         *  addr[0] << 24 | addr[1] << 16 | addr[2] << 8 | addr[3]
         */
        constexpr uint32_t to_uint32() const noexcept;

        /// Returns the address as a dotted quad string
        std::string to_string() const;

        private:

        ndgpp::net::ipv4_array value_ = {ndgpp::net::make_ipv4_array(Min)};
    };

    template <uint32_t Min, uint32_t Max>
    constexpr basic_ipv4_address<Min, Max>::basic_ipv4_address() noexcept = default;

    template <uint32_t Min, uint32_t Max>
    template <uint32_t MinO, uint32_t MaxO>
    constexpr basic_ipv4_address<Min, Max>::basic_ipv4_address(const basic_ipv4_address<MinO, MaxO> & addr) noexcept(MinO >= Min && MaxO <= Max):
        value_ {addr.value()}
    {
        if (!(MinO >= Min && MaxO <= Max))
        {
            basic_ipv4_address_validator<Min, Max> {} (this->to_uint32());
        }
    }

    template <uint32_t Min, uint32_t Max>
    constexpr basic_ipv4_address<Min, Max>::basic_ipv4_address(const ndgpp::net::ipv4_array value) noexcept(!basic_ipv4_address::constrained):
         value_ {value}
    {
        basic_ipv4_address_validator<Min, Max> {} (this->to_uint32());
    }

    template <uint32_t Min, uint32_t Max>
    constexpr basic_ipv4_address<Min, Max>::basic_ipv4_address(const uint32_t value) noexcept(!basic_ipv4_address::constrained):
         value_ {ndgpp::net::make_ipv4_array(value)}
    {
        basic_ipv4_address_validator<Min, Max> {} (value);
    }

    template <uint32_t Min, uint32_t Max>
    basic_ipv4_address<Min, Max>::basic_ipv4_address(const std::string & value):
        value_ {ndgpp::net::make_ipv4_array(value)}
    {
        basic_ipv4_address_validator<Min, Max> {} (this->to_uint32());
    }

    template <uint32_t Min, uint32_t Max>
    basic_ipv4_address<Min, Max> &
    basic_ipv4_address<Min, Max>::operator = (const basic_ipv4_address &) noexcept = default;

    template <uint32_t Min, uint32_t Max>
    basic_ipv4_address<Min, Max> &
    basic_ipv4_address<Min, Max>::operator = (const ndgpp::net::ipv4_array rhs) noexcept(!basic_ipv4_address::constrained)
    {
        this->value_ = rhs;
        basic_ipv4_address_validator<Min, Max> {} (this->to_uint32());
        return *this;
    }

    template <uint32_t Min, uint32_t Max>
    basic_ipv4_address<Min, Max> &
    basic_ipv4_address<Min, Max>::operator = (const uint32_t rhs) noexcept(!basic_ipv4_address::constrained)
    {
        this->value_ = ndgpp::net::make_ipv4_array(rhs);
        basic_ipv4_address_validator<Min, Max> {} (rhs);
        return *this;
    }

    template <uint32_t Min, uint32_t Max>
    basic_ipv4_address<Min, Max> &
    basic_ipv4_address<Min, Max>::operator = (const std::string & rhs)
    {
        this->value_ = ndgpp::net::make_ipv4_array(rhs);
        basic_ipv4_address_validator<Min, Max> {} (this->to_uint32());
        return *this;
    }

    template <uint32_t Min, uint32_t Max>
    void basic_ipv4_address<Min, Max>::swap(basic_ipv4_address & other) noexcept
    {
        std::swap(this->value_, other.value_);
    }

    template <uint32_t Min, uint32_t Max>
    inline constexpr uint8_t basic_ipv4_address<Min, Max>::operator [] (const std::size_t index) const noexcept
    {
        return this->value_[index];
    }

    template <uint32_t Min, uint32_t Max>
    inline constexpr ndgpp::net::ipv4_array
    basic_ipv4_address<Min, Max>::value() const noexcept
    {
        return this->value_;
    }

    template <uint32_t Min, uint32_t Max>
    inline constexpr uint32_t basic_ipv4_address<Min, Max>::to_uint32() const noexcept
    {
        return ndgpp::net::to_uint32(this->value_);
    }

    template <uint32_t Min, uint32_t Max>
    inline std::string basic_ipv4_address<Min, Max>::to_string() const
    {
        return ndgpp::net::to_string(this->value_);
    }

    template <uint32_t Min, uint32_t Max>
    void swap(basic_ipv4_address<Min, Max> & lhs,
              basic_ipv4_address<Min, Max> & rhs)
    {
        lhs.swap(rhs);
    }

    template <uint32_t Min, uint32_t Max>
    inline std::ostream & operator <<(std::ostream & stream, const basic_ipv4_address<Min, Max> address)
    {
        stream << static_cast<uint16_t>(address[0]);
        for (std::size_t i = 1; i < std::tuple_size<typename ndgpp::net::basic_ipv4_address<Min, Max>::value_type>::value; ++i)
        {
            stream.put('.');
            stream << static_cast<uint16_t>(address[i]);
        }

        return stream;
    }

    template <uint32_t Min, uint32_t Max>
    inline bool operator ==(const basic_ipv4_address<Min, Max> lhs, const basic_ipv4_address<Min, Max> rhs)
    {
        return lhs.value() == rhs.value();
    }

    template <uint32_t Min, uint32_t Max>
    inline bool operator !=(const basic_ipv4_address<Min, Max> lhs, const basic_ipv4_address<Min, Max> rhs)
    {
        return !(lhs == rhs);
    }

    template <uint32_t Min, uint32_t Max>
    inline bool operator <(const basic_ipv4_address<Min, Max> lhs, const basic_ipv4_address<Min, Max> rhs)
    {
        return lhs.value() < rhs.value();
    }

    template <uint32_t Min, uint32_t Max>
    inline bool operator >=(const basic_ipv4_address<Min, Max> lhs, const basic_ipv4_address<Min, Max> rhs)
    {
        return !(lhs < rhs);
    }

    template <uint32_t Min, uint32_t Max>
    inline bool operator >(const basic_ipv4_address<Min, Max> lhs, const basic_ipv4_address<Min, Max> rhs)
    {
        return rhs < lhs;
    }

    template <uint32_t Min, uint32_t Max>
    inline bool operator <=(const basic_ipv4_address<Min, Max> lhs, const basic_ipv4_address<Min, Max> rhs)
    {
        return !(rhs < lhs);
    }
}}

#endif
