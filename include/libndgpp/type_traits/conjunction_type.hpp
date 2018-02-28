#ifndef LIBNDGPP_TYPE_TRAITS_CONJUNCTION_TYPE_HPP
#define LIBNDGPP_TYPE_TRAITS_CONJUNCTION_TYPE_HPP

#include <type_traits>

namespace ndgpp
{
    template <class ...>
    struct conjunction_type : std::true_type {};

    template <class B>
    struct conjunction_type <B> : B {};

    /** A type that represents the conjunction of the specified types value member
     *
     *  conjunction_type is a subclass of either std::true_type or the
     *  first type whose value member results in false.
     *
     *  @tparam B The class whose value member will be evaluated
     *  @tparam Bs the remaining types to evaluate
     */
    template <class B, class ... Bs>
    struct conjunction_type <B, Bs...> : std::conditional<bool(B::value), conjunction_type<Bs...>, B>::type {};
}

#endif
