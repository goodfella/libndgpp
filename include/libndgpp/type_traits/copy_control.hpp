#ifndef LIBNDGPP_COPY_CONTROL_HPP
#define LIBNDGPP_COPY_CONTROL_HPP

#include <libndgpp/type_traits/conjunction_type.hpp>

namespace ndgpp
{
    namespace detail
    {
        struct copyable_type {};
        struct non_copyable_type
        {
            non_copyable_type() = default;
            non_copyable_type(const non_copyable_type &) = delete;
            non_copyable_type & operator=(const non_copyable_type &) = delete;
        };
    }

    /** Conditionally defines a type that is either copyable or non-copyable
     *
     *  Defines a type that is both copy constructible and copy
     *  assignable if all the specified types are both copy
     *  constructible and copy assignable.

     *  @tparam Ts The list of types who copy semantics dictate
     *          whether or not copy_control_t is copy constructible
     *          and copy assignable.
     */
    template <class ... Ts>
    using copy_control = typename std::conditional<ndgpp::conjunction_type<std::is_copy_constructible<Ts>...>::value &&
                                                   ndgpp::conjunction_type<std::is_copy_assignable<Ts>...>::value,
                                                   ndgpp::detail::copyable_type,
                                                   ndgpp::detail::non_copyable_type>::type;
}

#endif
