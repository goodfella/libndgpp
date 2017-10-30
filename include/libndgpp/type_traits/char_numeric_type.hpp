#ifndef LIBNDGPP_TYPE_TRAITS_CHAR_NUMERIC_TYPE_HPP
#define LIBNDGPP_TYPE_TRAITS_CHAR_NUMERIC_TYPE_HPP

namespace ndgpp
{
    /// Type trait class that returns a numeric type suitable for a char
    template <class T>
    struct char_numeric_type;

    template <>
    struct char_numeric_type<signed char>
    {
        using type = signed short;
    };

    template <>
    struct char_numeric_type<unsigned char>
    {
        using type = unsigned short;
    };

    template <>
    struct char_numeric_type<char>
    {
        using type = typename std::conditional<std::is_signed<char>::value,
                                               signed short,
                                               unsigned short>::type;
    };
}

#endif
