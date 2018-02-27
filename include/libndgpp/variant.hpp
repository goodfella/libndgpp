#ifndef LIBNDGPP_VARIANT_HPP
#define LIBNDGPP_VARIANT_HPP

#include <type_traits>
#include <functional>
#include <tuple>

#include "type_traits/conjunction_type.hpp"
#include "tuple.hpp"
#include "sfinae.hpp"
#include "type_traits/copy_control.hpp"
#include "type_traits/move_control.hpp"
#include "variant_alternative.hpp"
#include "detail/variant_impl.hpp"
#include "detail/variant_storage.hpp"

namespace ndgpp
{
    struct none_t {};
    static constexpr none_t none{};

    template <class ... Ts>
    class variant:
        public ndgpp::copy_control<Ts...>,
        public ndgpp::move_control<Ts...>
    {
        public:

        template <class U, class X = ndgpp::disable_if_same_or_derived<variant<Ts...>, std::decay_t<U>>>
        variant(U&& u):
            impl_(std::forward<U>(u))
        {}

        std::size_t index() const noexcept;

        template <class T>
        bool holds_alternative() const noexcept;

        template <std::size_t I, class ... Args>
        variant_alternative_t<I, variant<Ts...>>& emplace(Args&& ... args);

        void match(const std::function<void (const Ts&)>& ... branches) const;

        explicit
        operator bool() const noexcept;

        private:

        ndgpp::detail::variant_impl<Ts...> impl_;
    };

    template <class ... Ts>
    inline
    std::size_t variant<Ts...>::index() const noexcept
    {
        return impl_.index;
    }

    template <class ... Ts>
    template <class T>
    inline
    bool variant<Ts...>::holds_alternative() const noexcept
    {
        static_assert(ndgpp::tuple_contains<T, std::tuple<Ts...>>::value,
                      "T is not an alternative for this variant");
        return this->index() == ndgpp::tuple_index<T, std::tuple<Ts...>>::value;
    }

    template <class ... Ts>
    template <std::size_t I, class ... Args>
    inline
    variant_alternative_t<I, variant<Ts...>>& variant<Ts...>::emplace(Args&& ... args)
    {
        return this->impl_.emplace<I, Args...>(std::forward<Args>(args)...);
    }

    template <class ... Ts>
    inline
    variant<Ts...>::operator bool() const noexcept
    {
        static_assert(ndgpp::tuple_contains<ndgpp::none_t, std::tuple<Ts...>>::value,
                      "operator bool is only available with a variant that contains a ndgpp::none_t");

        return !this->holds_alternative<ndgpp::none_t>();
    }

    template <class ... Ts>
    inline
    void variant<Ts...>::match(const std::function<void (const Ts&)>& ... branches) const
    {
        impl_.match(branches...);
    }

    template <class ... Ts>
    inline
    void match(const variant<Ts...>& v, const std::function<void (const Ts&)>& ... branches)
    {
        v.match(branches...);
    }
}

#endif
