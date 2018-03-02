#ifndef LIBNDGPP_COPY_CONTROL_HPP
#define LIBNDGPP_COPY_CONTROL_HPP

#include <libndgpp/type_traits/conjunction_type.hpp>

namespace ndgpp
{
    /// A copy constructible and copy assignable type
    struct copyable_type {};

    /// A type that is not copy constructible or copy assignable
    struct non_copyable_type
    {
        non_copyable_type() = default;
        non_copyable_type(const non_copyable_type &) = delete;
        non_copyable_type & operator=(const non_copyable_type &) = delete;
    };

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
                                                   ndgpp::copyable_type,
                                                   ndgpp::non_copyable_type>::type;
}

#endif
