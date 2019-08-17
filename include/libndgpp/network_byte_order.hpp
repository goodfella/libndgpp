#ifndef LIBNDGPP_NETWORK_BYTE_ORDER_HPP
#define LIBNDGPP_NETWORK_BYTE_ORDER_HPP

#include <algorithm>
#include <array>
#include <cstring>
#include <type_traits>
#include <utility>
#include <ostream>

#include <libndgpp/network_byte_order_ops.hpp>

namespace ndgpp
{
    /** Stores a native type value in network byte order
     *
     *  The ndgpp::network_byte_order class is a regular type that
     *  will seemlessly represent a native type value in network byte
     *  order.
     *
     *  @tparam T The native type i.e. uint16_t, uint32_t, uint64_t
     */
    template <class T>
    class network_byte_order
    {
        static_assert(std::is_integral<T>::value, "T is not an integral type");
        static_assert(!std::is_signed<T>::value, "T is not unsigned");

        public:

        using value_type = T;

        constexpr
        network_byte_order() noexcept;

        /** Constructs a network_byte_order instance with the specified value
         *
         *  @param value An instance of type T in host byte order
         */
        explicit
        constexpr
        network_byte_order(const T value) noexcept;

        constexpr
        network_byte_order(const network_byte_order & other) noexcept;

        network_byte_order(network_byte_order && other) noexcept;


        /** Assigns this to the value of rhs
         *
         *  @param rhs An instance of type T in host byte order
         */
        network_byte_order & operator= (const T rhs) noexcept;
        network_byte_order & operator= (const network_byte_order & rhs) noexcept;
        network_byte_order & operator= (network_byte_order && rhs) noexcept;


        /// Returns the stored value in host byte order
        constexpr
        operator value_type () const noexcept;

        /** Returns the address of the underlying value
         *
         *  This is useful for sending the value over a socket:
         *
         *  \code
         *  const int ret = send(&v, v.size());
         */
        value_type const * operator &() const noexcept;
        value_type * operator &() noexcept;

        /// Returns the size of the underlying value
        constexpr std::size_t size() const noexcept;

        void swap(network_byte_order<T> & other) noexcept;

        private:

        value_type value_;

        friend bool operator== <> (const network_byte_order<T>, const network_byte_order<T>);
        friend bool operator!= <> (const network_byte_order<T>, const network_byte_order<T>);
        friend bool operator< <> (const network_byte_order<T>, const network_byte_order<T>);
        friend bool operator> <> (const network_byte_order<T>, const network_byte_order<T>);
        friend bool operator<= <> (const network_byte_order<T>, const network_byte_order<T>);
        friend bool operator>= <> (const network_byte_order<T>, const network_byte_order<T>);
    };

    template <class T>
    inline
    bool operator== (const network_byte_order<T> lhs,
                     const network_byte_order<T> rhs)
    {
        return lhs.value_ == rhs.value_;
    }

    template <class T>
    inline
    bool operator!= (const network_byte_order<T> lhs,
                     const network_byte_order<T> rhs)
    {
        return !(lhs == rhs);
    }

    template <class T>
    inline
    bool operator< (const network_byte_order<T> lhs,
                    const network_byte_order<T> rhs)
    {
        return lhs.value_ < rhs.value_;
    }

    template <class T>
    inline
    bool operator> (const network_byte_order<T> lhs,
                    const network_byte_order<T> rhs)
    {
        return rhs < lhs;
    }

    template <class T>
    inline
    bool operator<= (const network_byte_order<T> lhs,
                     const network_byte_order<T> rhs)
    {
        return !(rhs < lhs);
    }

    template <class T>
    inline
    bool operator>= (const network_byte_order<T> lhs,
                     const network_byte_order<T> rhs)
    {
        return !(lhs < rhs);
    }

    inline uint16_t host_to_network(const uint16_t val) noexcept
    {
        alignas(std::alignment_of<uint16_t>::value) std::array<uint8_t, 2> buf;
        buf[0] = static_cast<uint8_t>((val & 0xff00) >> 8);
        buf[1] = static_cast<uint8_t>((val & 0x00ff));
        return *reinterpret_cast<uint16_t*>(buf.data());
    }

    inline uint32_t host_to_network(const uint32_t val) noexcept
    {
        alignas(std::alignment_of<uint32_t>::value) std::array<uint8_t, 4> buf;
        buf[0] = static_cast<uint8_t>((val & 0xff000000) >> 24);
        buf[1] = static_cast<uint8_t>((val & 0x00ff0000) >> 16);
        buf[2] = static_cast<uint8_t>((val & 0x0000ff00) >> 8);
        buf[3] = static_cast<uint8_t>((val & 0x000000ff));
        return *reinterpret_cast<uint32_t*>(buf.data());
    }

    inline uint64_t host_to_network(const uint64_t val) noexcept
    {
        alignas(std::alignment_of<uint64_t>::value) std::array<uint8_t, 8> buf;
        buf[0] = static_cast<uint8_t>((val & 0xff00000000000000) >> 56);
        buf[1] = static_cast<uint8_t>((val & 0x00ff000000000000) >> 48);
        buf[2] = static_cast<uint8_t>((val & 0x0000ff0000000000) >> 40);
        buf[3] = static_cast<uint8_t>((val & 0x000000ff00000000) >> 32);
        buf[4] = static_cast<uint8_t>((val & 0x00000000ff000000) >> 24);
        buf[5] = static_cast<uint8_t>((val & 0x0000000000ff0000) >> 16);
        buf[6] = static_cast<uint8_t>((val & 0x000000000000ff00) >> 8);
        buf[7] = static_cast<uint8_t>((val & 0x00000000000000ff));
        return *reinterpret_cast<uint64_t*>(buf.data());
    }

    inline uint16_t network_to_host(const uint16_t val)
    {
        std::array<uint8_t, 2> buf;
        std::memcpy(buf.data(), &val, sizeof(val));
        return
            static_cast<uint16_t>(buf[0]) << 8 |
            static_cast<uint16_t>(buf[1]);
        
    }

    inline uint32_t network_to_host(const uint32_t val) noexcept
    {
        std::array<uint8_t, 4> buf;
        std::memcpy(buf.data(), &val, sizeof(val));
        return
            static_cast<uint32_t>(buf[0]) << 24 |
            static_cast<uint32_t>(buf[1]) << 16 |
            static_cast<uint32_t>(buf[2]) << 8  |
            static_cast<uint32_t>(buf[3]);
    }

    inline uint64_t network_to_host(const uint64_t val) noexcept
    {
        std::array<uint8_t, 8> buf;
        std::memcpy(buf.data(), &val, sizeof(val));
        return
            static_cast<uint64_t>(buf[0]) << 56 |
            static_cast<uint64_t>(buf[1]) << 48 |
            static_cast<uint64_t>(buf[2]) << 40 |
            static_cast<uint64_t>(buf[3]) << 32 |
            static_cast<uint64_t>(buf[4]) << 24 |
            static_cast<uint64_t>(buf[5]) << 16 |
            static_cast<uint64_t>(buf[6]) << 8  |
            static_cast<uint64_t>(buf[7]);
    }

    template <class T>
    void swap(network_byte_order<T> & lhs, network_byte_order<T> & rhs)
    {
        lhs.swap(rhs);
    }

    template <class T>
    inline
    std::ostream & operator <<(std::ostream & out, const network_byte_order<T> val)
    {
        out << static_cast<T>(val);
        return out;
    }
}

template <class T>
inline
constexpr
ndgpp::network_byte_order<T>::network_byte_order() noexcept = default;

template <class T>
inline
constexpr
ndgpp::network_byte_order<T>::network_byte_order(const ndgpp::network_byte_order<T>& other) noexcept = default;

template <class T>
inline
ndgpp::network_byte_order<T>::network_byte_order(ndgpp::network_byte_order<T>&& other) noexcept = default;

template <class T>
inline
ndgpp::network_byte_order<T> & ndgpp::network_byte_order<T>::operator= (const ndgpp::network_byte_order<T>& other) noexcept = default;

template <class T>
inline
ndgpp::network_byte_order<T> & ndgpp::network_byte_order<T>::operator= (ndgpp::network_byte_order<T>&& other) noexcept = default;

template <class T>
inline
constexpr
ndgpp::network_byte_order<T>::network_byte_order(const T value) noexcept:
    value_(ndgpp::host_to_network(value))
{}

template <class T>
inline
ndgpp::network_byte_order<T> & ndgpp::network_byte_order<T>::operator= (const T value) noexcept
{
    this->value_ = ndgpp::host_to_network(value);
    return *this;
}

template <class T>
inline
constexpr
ndgpp::network_byte_order<T>::operator T() const noexcept
{
    return ndgpp::network_to_host(this->value_);
}

template <class T>
inline
T const * ndgpp::network_byte_order<T>::operator &() const noexcept
{
    return &this->value_;
}

template <class T>
inline
T * ndgpp::network_byte_order<T>::operator &() noexcept
{
    return &this->value_;
}

template <class T>
inline
constexpr
std::size_t ndgpp::network_byte_order<T>::size() const noexcept
{
    return sizeof(T);
}

template <class T>
inline
void ndgpp::network_byte_order<T>::swap(ndgpp::network_byte_order<T> & other) noexcept
{
    std::swap(this->value_, other.value_);
}

#endif
