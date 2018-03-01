#ifndef LIBNDGPP_TUPLE_HPP
#define LIBNDGPP_TUPLE_HPP

namespace ndgpp
{
    template <class T, class Tuple>
    struct tuple_index;

    template <class T, class ... Ts>
    struct tuple_index<T, std::tuple<T, Ts...>>
    {
        static constexpr std::size_t value = 0;
    };

    /** Provides the index of the type in the tuple
     *
     *  @tparam T the type to index
     *  @tparam U the second type in the tuple
     *  @tparam Ts The remaining types in the tuple
     */
    template <class T, class U, class ... Ts>
    struct tuple_index<T, std::tuple<U, Ts...>>
    {
        /// The index of the type in the tuple
        static constexpr std::size_t value = 1 + tuple_index<T, std::tuple<Ts...>>::value;
    };

    template <class T, class Tuple>
    struct tuple_contains;

    template <class T>
    struct tuple_contains<T, std::tuple<>>
    {
        static constexpr bool value = false;
    };

    template <class T, class U>
    struct tuple_contains<T, std::tuple<U>>
    {
        static constexpr bool value = false;
    };

    template <class T>
    struct tuple_contains<T, std::tuple<T>>
    {
        static constexpr bool value = true;
    };

    template <class T, class ... Ts>
    struct tuple_contains<T, std::tuple<T, Ts...>>
    {
        static constexpr bool value = true;
    };

    /// If T is in the supplied tuple, provides the member constant value equal to true
    template <class T, class U, class ... Ts>
    struct tuple_contains<T, std::tuple<U, Ts...>>
    {
        static constexpr bool value = tuple_contains<T, std::tuple<Ts...>>::value;
    };
}

#endif
