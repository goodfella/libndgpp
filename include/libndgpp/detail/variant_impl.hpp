#ifndef LIBNDGPP_DETAIL_VARIANT_IMPL_HPP
#define LIBNDGPP_DETAIL_VARIANT_IMPL_HPP

#include <limits>
#include <tuple>

#include <libndgpp/variant_alternative.hpp>
#include <libndgpp/sfinae.hpp>
#include "variant_storage.hpp"

namespace ndgpp
{
    constexpr std::size_t variant_npos = std::numeric_limits<std::size_t>::max();

    namespace detail
    {
        template <class ... Ts>
        struct variant_impl
        {
            std::size_t index;

            template <class U, class X = ndgpp::disable_if_same_or_derived<variant_impl<Ts...>, std::decay_t<U>>>
            variant_impl(U&& u):
                index(ndgpp::tuple_index<std::decay_t<U>, std::tuple<Ts...>>::value)
            {
                new (std::addressof(storage)) ndgpp::detail::variant_storage<std::decay_t<U>>(std::forward<U>(u));
            }

            variant_impl(const variant_impl<Ts...>& other);
            variant_impl(variant_impl<Ts...>&& other);

            constexpr bool valueless_by_exception() const noexcept;

            ~variant_impl();

            const ndgpp::detail::variant_storage_base & storage_base() const noexcept;

            ndgpp::detail::variant_storage_base & storage_base() noexcept;

            template <std::size_t I, class ... Args>
            ndgpp::variant_alternative_t<I, variant_impl<Ts...>>& emplace(Args&& ... args);

            template <std::size_t I>
            ndgpp::detail::variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>& get() & noexcept;

            template <std::size_t I>
            ndgpp::detail::variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>&& get() && noexcept;

            template <std::size_t I>
            const ndgpp::detail::variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>& get() const & noexcept;

            std::aligned_union_t<0, ndgpp::detail::variant_storage_base, ndgpp::detail::variant_storage<Ts>...> storage;
        };

        template <class ... Ts>
        variant_impl<Ts...>::variant_impl(const variant_impl<Ts...>& other):
            index(variant_npos)
        {
            if (other.valueless_by_exception())
            {
                return;
            }

            other.storage_base().copy_construct(std::addressof(storage));
            this->index = other.index;
        }

        template <class ... Ts>
        variant_impl<Ts...>::variant_impl(variant_impl<Ts...>&& other):
            index(other.index)
        {
            other.index = variant_npos;
            other.storage_base().move_construct(std::addressof(storage));
        }

        template <class ... Ts>
        variant_impl<Ts...>::~variant_impl()
        {
            using variant_storage_base = ndgpp::detail::variant_storage_base;
            if (index != variant_npos)
            {
                this->storage_base().~variant_storage_base();
            }
        }

        template <class ... Ts>
        template <std::size_t I>
        variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>& variant_impl<Ts...>::get() & noexcept
        {
            using variant_storage_type = variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>;
            return *reinterpret_cast<variant_storage_type*>(std::addressof(storage));
        }

        template <class ... Ts>
        template <std::size_t I>
        variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>&& variant_impl<Ts...>::get() && noexcept
        {
            using variant_storage_type = variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>;
            return std::move(*reinterpret_cast<variant_storage_type*>(std::addressof(storage)));
        }

        template <class ... Ts>
        template <std::size_t I>
        const variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>& variant_impl<Ts...>::get() const & noexcept
        {
            using variant_storage_type = const variant_storage<std::tuple_element_t<I, std::tuple<Ts...>>>;
            return *reinterpret_cast<variant_storage_type*>(std::addressof(storage));
        }

        template <class ... Ts>
        template <std::size_t I, class ... Args>
        variant_alternative_t<I, variant_impl<Ts...>>& variant_impl<Ts...>::emplace(Args&& ... args)
        {
            if (! this->valueless_by_exception())
            {
                this->index = variant_npos;
                this->storage_base().~variant_storage_base();
            }

            using T = std::decay_t<std::tuple_element_t<I, std::tuple<Ts...>>>;

            new (std::addressof(storage)) ndgpp::detail::variant_storage<T>(std::forward<Args>(args)...);
            this->index = I;
            return this->get<I>().get();
        }

        template <class ... Ts>
        constexpr bool variant_impl<Ts...>::valueless_by_exception() const noexcept
        {
            return this->index == variant_npos;
        }

        template <class ... Ts>
        const ndgpp::detail::variant_storage_base & variant_impl<Ts...>::storage_base() const noexcept
        {
            return *reinterpret_cast<ndgpp::detail::variant_storage_base const *>(std::addressof(storage));
        }

        template <class ... Ts>
        ndgpp::detail::variant_storage_base & variant_impl<Ts...>::storage_base() noexcept
        {
            return *reinterpret_cast<ndgpp::detail::variant_storage_base *>(std::addressof(storage));
        }
    }
}

#endif
