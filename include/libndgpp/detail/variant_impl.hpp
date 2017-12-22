#ifndef LIBNDGPP_DETAIL_VARIANT_IMPL_HPP
#define LIBNDGPP_DETAIL_VARIANT_IMPL_HPP

#include <limits>
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

            ~variant_impl();

            const ndgpp::detail::variant_storage_base & storage_base() const noexcept;

            ndgpp::detail::variant_storage_base & storage_base() noexcept;

            std::aligned_union_t<0, ndgpp::detail::variant_storage_base, ndgpp::detail::variant_storage<Ts>...> storage;
        };

        template <class ... Ts>
        variant_impl<Ts...>::variant_impl(const variant_impl<Ts...>& other):
            index(other.index)
        {
            other.storage_base().copy_construct(std::addressof(storage));
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
