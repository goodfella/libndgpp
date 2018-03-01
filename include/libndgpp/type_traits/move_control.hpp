#ifndef LIBNDGPP_MOVE_CONTROL_HPP
#define LIBNDGPP_MOVE_CONTROL_HPP

#include <libndgpp/type_traits/conjunction_type.hpp>

namespace ndgpp
{
    namespace detail
    {
        struct moveable_type {};
        struct non_moveable_type
        {
            non_moveable_type() = default;
            non_moveable_type(const non_moveable_type&) = delete;
            non_moveable_type& operator=(const non_moveable_type&) = delete;
        };
    }

    /** Conditionally defines a type that is either movable or non-movable
     *
     *  Defines a type that is both move constructible and move
     *  assignable if all the specified types are both move
     *  constructible and move assignable.

     *  @tparam Ts The list of types whose move semantics dictate
     *          whether or not move_control is move constructible
     *          and move assignable.
     */
    template <class ... Ts>
    using move_control = std::conditional_t<ndgpp::conjunction_type<std::is_move_constructible<Ts>...>::value &&
                                            ndgpp::conjunction_type<std::is_move_assignable<Ts>...>::value,
                                            ndgpp::detail::moveable_type,
                                            ndgpp::detail::non_moveable_type>;
}

#endif
