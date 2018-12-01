#ifndef LIBNDGPP_STRTO_HPP
#define LIBNDGPP_STRTO_HPP

#include <cctype>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include <libndgpp/error.hpp>
#include <libndgpp/strto_result.hpp>
#include <libndgpp/detail/strto.hpp>

namespace ndgpp
{
    struct cstring_param_t {};
    constexpr cstring_param_t cstring_param {};

    /** @defgroup strtoi strtoi
     *
     *  Converts a string to an integer type given the Min and Max
     *  limits specified.  If an unsigned type is desired, and the
     *  string represents a negative number, an error is returned.
     *
     *  The conversion is done using the strto family of functions.
     *  If the conversion encounters a non-numeric character given the
     *  conversion base and that character is a delimiter character,
     *  then an error is not returned.
     *
     *  @tparam T The type of integer to convert the string to
     *  @tparam DelimIter The delimiter iterator type
     *  @tparam Min The minimum valued allowed
     *  @tparam Max The maximum value allowed
     *
     *  @param str The string to convert
     *
     *  @param base The base of the conversion see the std::strto family
     *              of functions for more information
     *
     *  @param delim_first The first delimiter
     *  @param delim_last The last delimiter
     *
     *  @return A ndgpp::strto_result object
     *
     *  @{
     */
    template <class T,
              class DelimIter,
              T Min = std::numeric_limits<std::decay_t<T>>::min(),
              T Max = std::numeric_limits<std::decay_t<T>>::max()>
    auto strtoi(char const * const str, int base, DelimIter delim_first, DelimIter delim_last);

    template <class T,
              unsigned N,
              T Min = std::numeric_limits<std::decay_t<T>>::min(),
              T Max = std::numeric_limits<std::decay_t<T>>::max()>
    auto strtoi(char const * const str, int base, const char (&delims) [N]);

    template <class T,
              T Min = std::numeric_limits<std::decay_t<T>>::min(),
              T Max = std::numeric_limits<std::decay_t<T>>::max()>
    inline auto strtoi(char const * const str, int base = 0)
    {
        return strtoi<T, 1U, Min, Max>(str, base, "");
    }

    template <class T,
              T Min = std::numeric_limits<std::decay_t<T>>::min(),
              T Max = std::numeric_limits<std::decay_t<T>>::max()>
    inline auto strtoi(char const * const str, int base, char const * const delims, ndgpp::cstring_param_t)
    {
        return strtoi<T, char const *, Min, Max>(str,
                                                 base,
                                                 delims,
                                                 delims + std::strlen(delims));
    }

    template <class T,
              unsigned N,
              T Min = std::numeric_limits<std::decay_t<T>>::min(),
              T Max = std::numeric_limits<std::decay_t<T>>::max()>
    inline auto strtoi(char const * const str, int base, const char (&delims) [N])
    {
        return strtoi<T, char const*, Min, Max>(str,
                                                base,
                                                delims,
                                                delims + (N - 1));
    }

    template <class T,
              class DelimIter,
              T Min = std::numeric_limits<std::decay_t<T>>::min(),
              T Max = std::numeric_limits<std::decay_t<T>>::max()>
    auto strtoi(char const * const str, int base, DelimIter delim_first, DelimIter delim_last)
    {
        using decayed_type = std::decay_t<T>;

        char * str_end = nullptr;

        if (str[0] == '\0')
        {
                return strto_result<T> {strto_result<T>::invalid_value, str};
        }

        std::size_t offset = 0;
        if (! std::numeric_limits<decayed_type>::is_signed)
        {
            for (int i = 0; str[i] != '\0'; ++i)
            {
                if (! std::isspace(str[i]))
                {
                    offset = i;
                    break;
                }
            }

            if (str[offset] == '-')
            {
                return strto_result<T> {strto_result<T>::underflowed, str};
            }
        }

        const auto value = detail::strtoi_traits<decayed_type>::convert(str + offset, &str_end, base);

        if (str_end != nullptr && *str_end != '\0')
        {
            // The full string was not parsed, check if the first
            // character not parsed is a delimiter
            const auto result = std::find(delim_first, delim_last, *str_end);
            if (result == delim_last)
            {
                return strto_result<T> {strto_result<T>::invalid_value, str_end};
            }
        }

        if (errno == ERANGE)
        {
            // The parsed value is too large or too small for the type
            if (value == detail::strtoi_traits<decayed_type>::underflow_error_value)
            {
                return strto_result<T> {strto_result<T>::underflowed, str_end};
            }

            if (value == detail::strtoi_traits<decayed_type>::overflow_error_value)
            {
                return strto_result<T> {strto_result<T>::overflowed, str_end};
            }
        }

        if (Min > std::numeric_limits<decayed_type>::min() && value < Min)
        {
            return strto_result<T> {strto_result<T>::underflowed, str_end};
        }

        if (Max < std::numeric_limits<decayed_type>::max() && value > Max)
        {
            return strto_result<T> {strto_result<T>::overflowed, str_end};
        }

        if (detail::strtoi_traits<decayed_type>::exact)
        {
            return strto_result<T> {static_cast<decayed_type>(value), str_end};
        }

        if (value < std::numeric_limits<decayed_type>::min())
        {
            return strto_result<T> {strto_result<T>::underflowed, str_end};
        }

        if (value > std::numeric_limits<decayed_type>::max())
        {
            return strto_result<T> {strto_result<T>::overflowed, str_end};
        }

        return strto_result<T> {static_cast<decayed_type>(value), str_end};
    }

    /// @}
}

#endif
