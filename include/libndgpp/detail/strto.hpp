#ifndef LIBNDGPP_DETAIL_STRTO_HPP
#define LIBNDGPP_DETAIL_STRTO_HPP

#include <climits>
#include <cstdlib>

namespace ndgpp
{
namespace detail
{

    template <class T>
    struct strtoi_traits;

    template <>
    struct strtoi_traits<signed long long>
    {
        static constexpr bool exact = true;
        static constexpr signed long long underflow_error_value = LLONG_MIN;
        static constexpr signed long long overflow_error_value = LLONG_MAX;
        static long long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtoll(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<signed long>
    {
        static constexpr bool exact = true;
        static constexpr signed long underflow_error_value = LONG_MIN;
        static constexpr signed long overflow_error_value = LONG_MAX;
        static long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtol(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<signed int>
    {
        static constexpr bool exact = false;
        static constexpr signed long long underflow_error_value = LONG_MIN;
        static constexpr signed long overflow_error_value = LONG_MAX;
        static long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtol(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<signed short>
    {
        static constexpr bool exact = false;
        static constexpr signed long long underflow_error_value = LONG_MIN;
        static constexpr signed long overflow_error_value = LONG_MAX;
        static long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtol(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<signed char>
    {
        static constexpr bool exact = false;
        static constexpr signed long long underflow_error_value = LONG_MIN;
        static constexpr signed long overflow_error_value = LONG_MAX;
        static long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtol(str, str_end, base);
        }
    };


    template <>
    struct strtoi_traits<unsigned long long>
    {
        static constexpr bool exact = true;
        static constexpr unsigned long long underflow_error_value = 0;
        static constexpr unsigned long long overflow_error_value = ULLONG_MAX;
        static unsigned long long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtoull(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<unsigned long>
    {
        static constexpr bool exact = true;
        static constexpr long long underflow_error_value = 0;
        static constexpr unsigned long overflow_error_value = ULONG_MAX;
        static unsigned long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtoul(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<unsigned int>
    {
        static constexpr bool exact = false;
        static constexpr long long underflow_error_value = 0;
        static constexpr unsigned long overflow_error_value = ULONG_MAX;
        static unsigned long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtoul(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<unsigned short>
    {
        static constexpr bool exact = false;
        static constexpr long long underflow_error_value = 0;
        static constexpr unsigned long overflow_error_value = ULONG_MAX;
        static unsigned long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtoul(str, str_end, base);
        }
    };

    template <>
    struct strtoi_traits<unsigned char>
    {
        static constexpr bool exact = false;
        static constexpr long long underflow_error_value = 0;
        static constexpr unsigned long overflow_error_value = ULONG_MAX;
        static unsigned long convert(char const * const str, char ** str_end, const int base)
        {
            return std::strtoul(str, str_end, base);
        }
    };
}
}

#endif
