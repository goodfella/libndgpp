#ifndef LIBNDGPP_VARIANT_HPP
#define LIBNDGPP_VARIANT_HPP

#include <type_traits>
#include <functional>
#include <tuple>

#include "type_traits/conjunction_type.hpp"
#include "tuple.hpp"
#include "sfinae.hpp"
#include "type_traits/copy_control.hpp"
#include "detail/variant_impl.hpp"
#include "detail/variant_storage.hpp"

namespace ndgpp
{
    struct none_t {};
    static constexpr none_t none{};

    template <class ... Ts>
    class variant: public ndgpp::copy_control<Ts...>
    {
        public:

        template <class U, class X = ndgpp::disable_if_same_or_derived<variant<Ts...>, std::decay_t<U>>>
        variant(U&& u):
            impl_(std::forward<U>(u))
        {}

        std::size_t index() const noexcept;

        template <class T>
        bool holds_alternative() const noexcept;

        void match(const std::function<void (const Ts&)>& ... branches) const;

        explicit
        operator bool() const noexcept;

        private:

        void matcher() const;

        template <class U, class ... Us>
        void matcher(const std::function<void (const U&)>& branch,
                     const std::function<void (const Us&)>& ... branches) const;

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
    inline
    variant<Ts...>::operator bool() const noexcept
    {
        static_assert(ndgpp::tuple_contains<ndgpp::none_t, std::tuple<Ts...>>::value,
                      "operator bool is only available with a variant that contains a ndgpp::none_t");

        return !this->holds_alternative<ndgpp::none_t>();
    }

    template <class ... Ts>
    inline
    void variant<Ts...>::matcher() const
    {}

    template <class ... Ts>
    template <class U, class ... Us>
    inline
    void variant<Ts...>::matcher(const std::function<void (const U&)>& branch,
                                 const std::function<void (const Us&)>& ... branches) const
    {
        if (ndgpp::tuple_index<U, std::tuple<Ts...>>::value == this->index())
        {
            branch(static_cast<const ndgpp::detail::variant_storage<U>&>(impl_.storage_base()).get());
        }
        else
        {
            matcher(branches...);
        }
    }

    template <class ... Ts>
    inline
    void variant<Ts...>::match(const std::function<void (const Ts&)>& ... branches) const
    {
        matcher(branches...);
    }
}

#endif
