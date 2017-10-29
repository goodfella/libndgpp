#ifndef LIBNDGPP_UTILITY_HPP
#define LIBNDGPP_UTILITY_HPP

namespace ndgpp
{
    /// Causes a compiler error upon instantiation so the type can be examined
    template <class T>
    class print_type_t;

    /// Helper function to generate a print_type_t compilation error
    template<class T>
    inline decltype(auto) print_type(T&& t)
    {
        return print_type_t<T>{}
    }
}

#endif
