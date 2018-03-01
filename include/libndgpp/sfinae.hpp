#ifndef LIBNDGPP_SFINAE_HPP
#define LIBNDGPP_SFINAE_HPP

namespace ndgpp
{
    /// Disables a function if A is the same or a derived type of B
    template<typename A, typename B>
    using disable_if_same_or_derived =
    typename std::enable_if<
        !std::is_base_of<A,typename
                         std::remove_reference<B>::type
                         >::value
        >::type;
}

#endif
