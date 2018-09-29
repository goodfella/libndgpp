#ifndef LIBNDGPP_TYPE_TRAITS_IS_CLASS_DEFINED_HPP
#define LIBNDGPP_TYPE_TRAITS_IS_CLASS_DEFINED_HPP

#include <type_traits>

namespace ndgpp
{
    template <class T, class Enable = void>
    struct is_class_defined
    {
        static constexpr bool value = false;
    };

    /// A type trait to determine if a class is defined
    template <class T>
    struct is_class_defined<T, std::enable_if_t<(sizeof(T) > 0)>>
    {
        static constexpr bool value = true;
    };
}

#endif
