#ifndef LIBNDGPP_BOUNDED_INTEGER_HPP
#define LIBNDGPP_BOUNDED_INTEGER_HPP

#include <cstdint>
#include <cstring>

#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include <libndgpp/error.hpp>
#include <libndgpp/safe_operators.hpp>
#include <libndgpp/strto.hpp>

namespace ndgpp
{
    struct bounded_integer_error: public std::invalid_argument
    {
        bounded_integer_error(const char * what):
            std::invalid_argument(what)
        {}
    };

    struct bounded_integer_underflow final: public bounded_integer_error
    {
        bounded_integer_underflow():
            bounded_integer_error("ndgpp::bounded_integer underflow")
        {}
    };

    struct bounded_integer_overflow final: public bounded_integer_error
    {
        bounded_integer_overflow():
            bounded_integer_error("ndgpp::bounded_integer overflow")
        {}
    };

    struct bounded_integer_invalid final: public bounded_integer_error
    {
        bounded_integer_invalid():
            bounded_integer_error("ndgpp::bounded_integer invalid value")
        {}
    };

    struct bounded_integer_min_t {};
    constexpr bounded_integer_min_t bounded_integer_min {};

    struct bounded_integer_max_t {};
    constexpr bounded_integer_max_t bounded_integer_max {};

    /** An integer type that has a bounded value range
     *
     *  @tparam T The underlying integer type
     *
     *  @tparam Min The minimal value this type is allowed to represent
     *
     *  @tparam Max The maximum value this type is allowed to represent
     *
     *  @tparam Tag Allows for differentiating between two
     *          bounded_integer types with the same ranges.
     */
    template <class T, T Min, T Max, class Tag = void>
    class bounded_integer final
    {
        public:

        static_assert(std::is_integral<T>::value, "T must be a integral type");

        /// The underlying integer type
        using value_type = std::decay_t<T>;

        /// The tag type
        using tag_type = Tag;

        /// Returns the minimum value this bounded_integer can represent
        static constexpr value_type min() noexcept {return Min;}

        /// Returns the maximum value this bounded_integer can represent
        static constexpr value_type max() noexcept {return Max;}

        /// Constructs a bounded integer with its minimum allowed value
        constexpr bounded_integer (bounded_integer_min_t) noexcept;

        /// Constructs a bounded integer with its maximum allowed value
        constexpr bounded_integer (bounded_integer_max_t) noexcept;

        /** Constructs a bounded_integer given a compile time constant
         *
         *  @tparam U The constant's integral type
         *  @tparam Val The constant's value
         *
         *  @param v The std::integral_constant instance
         */
        template <class U, U Val>
        explicit constexpr bounded_integer(std::integral_constant<U, Val> v) noexcept;

        /** Constructs a bounded_integer
         *
         *  @param value The value to assign the bounded_integer to
         */
        template <class U>
        explicit bounded_integer(const U value);

        /** Constructs a bounded integer from a C string
         *
         *  @tparam DelimIter The delimiter iterator type
         *
         *  @param str The value represented as a string
         *  @param pos The offset in the string to start parsing for a value
         *  @param base The base to use for the conversion see std::strtoul
         *  @param first The first element in the iterator range
         *  @param last The last element in the iterator range
         */
        template <class DelimIter>
        bounded_integer(char const * const str,
                        const std::size_t pos,
                        const int base,
                        const DelimIter begin,
                        const DelimIter end);

        /** Constructs a bounded integer from a C string
         *
         *  @param str The value represented as a string
         *  @param pos The offset in the string to start parsing for a value
         *  @param base The base to use for the conversion see std::strtoul
         *  @param delimiters A string containing the set of delimiters
         */
        explicit
        bounded_integer(char const * const str,
                        const std::size_t pos = 0,
                        const int base = 0,
                        char const * const delimiters = "");

        /** Constructs a bounded integer from a std::string
         *
         *  @tparam DelimIter The delimiter iterator type
         *
         *  @param str The value represented as string
         *  @param pos The offset in the string to start parsing for a value
         *  @param base The base to use for the conversion see std::strtoul
         *  @param first The first element in the iterator range
         *  @param last The last element in the iterator range
         */
        template <class DelimIter>
        bounded_integer(const std::string& str,
                        const std::size_t pos,
                        const int base,
                        const DelimIter begin,
                        const DelimIter end);

        /** Constructs a bounded integer from a std::string
         *
         *  @param str The value represented as a string
         *  @param pos The offset in the string to start parsing for a value
         *  @param base The base to use for the conversion see std::strtoul
         *  @param delimiters A string containing the set of delimiters
         */
        explicit
        bounded_integer(const std::string& str,
                        const std::size_t pos = 0,
                        const int base = 0,
                        char const * const delimiters = "");

        bounded_integer(const bounded_integer&) noexcept;
        bounded_integer& operator=(const bounded_integer&) noexcept;

        bounded_integer(bounded_integer&&) noexcept;
        bounded_integer& operator=( bounded_integer&&) noexcept;

        /// Returns the underlying value
        constexpr value_type get() const noexcept;

        /// Swaps two bounded_integer values
        void swap(bounded_integer& other) noexcept;

        private:

        value_type value_ = Min;
    };

    template <class T, T Min, T Max, class Tag>
    bounded_integer<T, Min, Max, Tag>::bounded_integer(const bounded_integer&) noexcept = default;

    template <class T, T Min, T Max, class Tag>
    bounded_integer<T, Min, Max, Tag>&
    bounded_integer<T, Min, Max, Tag>::bounded_integer::operator=(const bounded_integer&) noexcept = default;

    template <class T, T Min, T Max, class Tag>
    bounded_integer<T, Min, Max, Tag>::bounded_integer::bounded_integer(bounded_integer&&) noexcept = default;

    template <class T, T Min, T Max, class Tag>
    bounded_integer<T, Min, Max, Tag>&
    bounded_integer<T, Min, Max, Tag>::bounded_integer::operator=(bounded_integer&&) noexcept = default;

    template <class T, T Min, T Max, class Tag>
    constexpr bounded_integer<T, Min, Max, Tag>::bounded_integer(bounded_integer_min_t) noexcept:
        value_(Min)
    {}

    template <class T, T Min, T Max, class Tag>
    constexpr bounded_integer<T, Min, Max, Tag>::bounded_integer(bounded_integer_max_t) noexcept:
        value_(Max)
    {}

    template <class T, T Min, T Max, class Tag>
    template <class U, U Val>
    constexpr bounded_integer<T, Min, Max, Tag>::bounded_integer(std::integral_constant<U, Val>) noexcept:
        value_(Val)
    {
        static_assert(Val >= Min && Val <= Max,
                      "integral constant out of bounded_integer range");
    }

    template <class T, T Min, T Max, class Tag>
    template <class U>
    bounded_integer<T, Min, Max, Tag>::bounded_integer(const U value)
    {
        if (ndgpp::safe_op::lt(value, Min))
        {
            throw ndgpp::error<bounded_integer_underflow>(ndgpp_source_location);
        }
        else if (ndgpp::safe_op::gt(value, Max))
        {
            throw ndgpp::error<bounded_integer_overflow>(ndgpp_source_location);
        }

        value_ = value;
    }

    template <class T, T Min, T Max, class Tag>
    template <class DelimIter>
    bounded_integer<T, Min, Max, Tag>::bounded_integer(char const * const str,
                                                       const std::size_t pos,
                                                       const int base,
                                                       const DelimIter begin,
                                                       const DelimIter end)
    {
        const ndgpp::strto_result<value_type> result =
            ndgpp::strtoi<value_type, DelimIter, Min, Max>(str + pos, base, begin, end);

        if (result)
        {
            value_ = result.value();
            return;
        }

        if (result.overflow())
        {
            throw ndgpp::error<bounded_integer_overflow>(ndgpp_source_location);
        }

        if (result.underflow())
        {
            throw ndgpp::error<bounded_integer_underflow>(ndgpp_source_location);
        }

        if (result.invalid())
        {
            throw ndgpp::error<bounded_integer_invalid>(ndgpp_source_location);
        }
    }

    template <class T, T Min, T Max, class Tag>
    bounded_integer<T, Min, Max, Tag>::bounded_integer(char const * const str,
                                                       const std::size_t pos,
                                                       const int base,
                                                       char const * const delimiters):
        bounded_integer(str, pos, base, delimiters, delimiters + std::strlen(delimiters))
    {}

    template <class T, T Min, T Max, class Tag>
    template <class DelimIter>
    bounded_integer<T, Min, Max, Tag>::bounded_integer(const std::string& str,
                                                       const std::size_t pos,
                                                       const int base,
                                                       const DelimIter begin,
                                                       const DelimIter end):
        bounded_integer(str.c_str(), pos, base, begin, end)
    {}

    template <class T, T Min, T Max, class Tag>
    bounded_integer<T, Min, Max, Tag>::bounded_integer(const std::string& str,
                                                       const std::size_t pos,
                                                       const int base,
                                                       char const * const delimiters):
        bounded_integer(str.c_str(), pos, base, delimiters, delimiters + std::strlen(delimiters))
    {}

    template <class T, T Min, T Max, class Tag>
    inline constexpr std::decay_t<T> bounded_integer<T, Min, Max, Tag>::get() const noexcept
    {
        return this->value_;
    }

    template <class T, T Min, T Max, class Tag>
    inline void bounded_integer<T, Min, Max, Tag>::swap(bounded_integer& other) noexcept
    {
        std::swap(this->value_, other.value_);
    }

    template <class T, T Min, T Max, class Tag>
    inline bool operator < (const bounded_integer<T, Min, Max, Tag> lhs,
                            const bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::lt(lhs.get(), rhs.get());
    }

    template <class T, T Min, T Max, class Tag, class U>
    inline bool operator < (const bounded_integer<T, Min, Max, Tag> lhs,
                            const U rhs) noexcept
    {
        return ndgpp::safe_op::lt(lhs.get(), rhs);
    }

    template <class U, class T, T Min, T Max, class Tag>
    inline bool operator < (const U lhs,
                            bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::lt(lhs, rhs.get());
    }

    template <class T, T Min, T Max, class Tag>
    inline bool operator == (const bounded_integer<T, Min, Max, Tag> lhs,
                             const bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return lhs.get() == rhs.get();
    }

    template <class T, T Min, T Max, class Tag, class U>
    inline bool operator == (const bounded_integer<T, Min, Max, Tag> lhs,
                             const U rhs) noexcept
    {
        return lhs.get() == rhs;
    }

    template <class U, class T, T Min, T Max, class Tag>
    inline bool operator == (const U lhs,
                             bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return lhs == rhs.get();
    }

    template <class T, T Min, T Max, class Tag>
    inline bool operator != (const bounded_integer<T, Min, Max, Tag> lhs,
                             const bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return lhs.get() != rhs.get();
    }

    template <class T, T Min, T Max, class Tag, class U>
    inline bool operator != (const bounded_integer<T, Min, Max, Tag> lhs,
                             const U rhs) noexcept
    {
        return lhs.get() != rhs;
    }

    template <class U, class T, T Min, T Max, class Tag>
    inline bool operator != (const U lhs,
                             bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return lhs != rhs.get();
    }

    template <class T, T Min, T Max, class Tag>
    inline bool operator > (const bounded_integer<T, Min, Max, Tag> lhs,
                            const bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::gt(lhs.get(), rhs.get());
    }

    template <class T, T Min, T Max, class Tag, class U>
    inline bool operator > (const bounded_integer<T, Min, Max, Tag> lhs,
                            const U rhs) noexcept
    {
        return ndgpp::safe_op::gt(lhs.get(), rhs);
    }

    template <class U, class T, T Min, T Max, class Tag>
    inline bool operator > (const U lhs,
                            bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::gt(lhs, rhs.get());
    }

    template <class T, T Min, T Max, class Tag>
    inline bool operator >= (const bounded_integer<T, Min, Max, Tag> lhs,
                             const bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::gte(lhs.get(), rhs.get());
    }

    template <class T, T Min, T Max, class Tag, class U>
    inline bool operator >= (const bounded_integer<T, Min, Max, Tag> lhs,
                             const U rhs) noexcept
    {
        return ndgpp::safe_op::gte(lhs.get(), rhs);
    }

    template <class U, class T, T Min, T Max, class Tag>
    inline bool operator >= (const U lhs,
                             bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::gte(lhs, rhs.get());
    }

    template <class T, T Min, T Max, class Tag>
    inline bool operator <= (const bounded_integer<T, Min, Max, Tag> lhs,
                             const bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::lte(lhs.get(), rhs.get());
    }

    template <class T, T Min, T Max, class Tag, class U>
    inline bool operator <= (const bounded_integer<T, Min, Max, Tag> lhs,
                             const U rhs) noexcept
    {
        return ndgpp::safe_op::lte(lhs.get(), rhs);
    }

    template <class U, class T, T Min, T Max, class Tag>
    inline bool operator <= (const U lhs,
                             bounded_integer<T, Min, Max, Tag> rhs) noexcept
    {
        return ndgpp::safe_op::lte(lhs, rhs.get());
    }

    template <class T, T Min, T Max, class Tag>
    std::ostream& operator << (std::ostream& out, const bounded_integer<T, Min, Max, Tag> rhs)
    {
        out << rhs.get();
        return out;
    }
}

namespace std
{
    template <class T, T Min, T Max, class Tag>
    struct hash<ndgpp::bounded_integer<T, Min, Max, Tag>> final
    {
        using bounded_integer_type = ndgpp::bounded_integer<T, Min, Max, Tag>;
        using argument_type = bounded_integer_type;
        using result_type = typename std::hash<T>::result_type;

        result_type operator() (argument_type val) const noexcept
        {
            return hasher(val.get());
        }

        std::hash<T> hasher;
    };
}

#endif
