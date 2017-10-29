namespace ndgpp { namespace detail {

    inline constexpr decltype(auto) accumulate()
    {
        return 0;
    }

    template <class T, class ... Args>
    inline constexpr decltype(auto) accumulate(T&& t, Args&& ... args)
    {
        return std::forward<T>(t) + detail::accumulate(std::forward<Args>(args)...);
    }
}

    template <class ... Args>
    inline constexpr decltype(auto) accumulate(Args&& ... args)
    {
        return detail::accumulate(std::forward<Args>(args)...);
    }
}
