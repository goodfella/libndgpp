#ifndef LIBNDGPP_VARIANT_ALTERNATIVE_HPP
#define LIBNDGPP_VARIANT_ALTERNATIVE_HPP

#include <tuple>

namespace ndgpp
{
    template <class ... Ts>
    class variant;

    template <std::size_t I, class T>
    struct variant_alternative;

    template <std::size_t I, class ... Ts>
    struct variant_alternative<I, variant<Ts...>>
    {
        using type = std::tuple_element_t<I, std::tuple<Ts...>>;
    };

    template <std::size_t I, class T>
    using variant_alternative_t = typename variant_alternative<I, T>::type;
}

#endif
