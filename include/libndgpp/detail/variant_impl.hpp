#ifndef LIBNDGPP_DETAIL_VARIANT_IMPL_HPP
#define LIBNDGPP_DETAIL_VARIANT_IMPL_HPP

#include <limits>
#include <tuple>
#include <functional>
#include <type_traits>

#include <libndgpp/variant_alternative.hpp>
#include <libndgpp/type_traits/conjunction_type.hpp>
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

            static constexpr bool move_assign_noexcept =
                ndgpp::conjunction_type<std::is_nothrow_move_constructible<Ts> ...>::value &&
                ndgpp::conjunction_type<std::is_nothrow_move_assignable<Ts> ...>::value;

            variant_impl& operator= (variant_impl<Ts...>&& other) noexcept(move_assign_noexcept);

            static constexpr bool move_ctor_noexcept =
                ndgpp::conjunction_type<std::is_nothrow_move_constructible<Ts>...>::value;

            variant_impl(variant_impl<Ts...>&& other) noexcept(move_ctor_noexcept);

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

            void match(const std::function<void (const Ts&)>& ... branches) const &;
            void match(const std::function<void (Ts&&)>& ... branches) &&;

            void matcher() const &;
            void matcher() &&;

            template <class U, class ... Us>
            void matcher(const std::function<void (const U&)>& branch,
                         const std::function<void (const Us&)>& ... branches) const &;

            template <class U, class ... Us>
            void matcher(const std::function<void (U&&)>& branch,
                         const std::function<void (Us&&)>& ... branches) &&;

            std::aligned_union_t<0, ndgpp::detail::variant_storage_base, ndgpp::detail::variant_storage<Ts>...> storage;
        };

        template <class T, class ... Ts>
        struct variant_impl_emplace;

        template <class T, class ... Ts>
        struct variant_impl_emplace<T, variant_impl<Ts...>>
        {
            void operator() (T&& t)
            {
                variant.get().template emplace<ndgpp::tuple_index<T, std::tuple<Ts...>>::value>(std::move(t));
            }

            void operator() (const T& t)
            {
                variant.get().template emplace<ndgpp::tuple_index<T, std::tuple<Ts...>>::value>(t);
            }

            std::reference_wrapper<variant_impl<Ts...>> variant;
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
        variant_impl<Ts...>& variant_impl<Ts...>::operator=(variant_impl<Ts...>&& other) noexcept(variant_impl::move_assign_noexcept)
        {
            if (this == &other)
            {
                return *this;
            }
            else if (this->valueless_by_exception() && other.valueless_by_exception())
            {
                return *this;
            }
            else if (other.valueless_by_exception())
            {
                this->index = variant_npos;
                this->storage_base().~variant_storage_base();
            }
            else if (other.index == this->index)
            {
                other.storage_base().move_assign(this->storage_base().get_ptr());
            }
            else
            {
                using variant_type = variant_impl<Ts...>;
                std::move(other).match(variant_impl_emplace<Ts, variant_type>{std::ref(*this)}...);
            }

            return *this;
        }

        template <class ... Ts>
        variant_impl<Ts...>::variant_impl(variant_impl<Ts...>&& other) noexcept(variant_impl::move_ctor_noexcept):
            index(other.index)
        {
            if (other.valueless_by_exception())
            {
                return;
            }

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

            try
            {
                new (std::addressof(storage)) ndgpp::detail::variant_storage<T>(std::forward<Args>(args)...);
            }
            catch (...) {
                this->index = ndgpp::variant_npos;
                throw;
            }

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

        template <class ... Ts>
        inline
        void variant_impl<Ts...>::matcher() const &
        {}

        template <class ... Ts>
        inline
        void variant_impl<Ts...>::matcher() &&
        {}

        template <class ... Ts>
        template <class U, class ... Us>
        inline
        void variant_impl<Ts...>::matcher(const std::function<void (const U&)>& branch,
                                          const std::function<void (const Us&)>& ... branches) const &
        {
            if (ndgpp::tuple_index<U, std::tuple<Ts...>>::value == this->index)
            {
                branch(this->get<ndgpp::tuple_index<U, std::tuple<Ts...>>::value>().get());
            }
            else
            {
                matcher(branches...);
            }
        }

        template <class ... Ts>
        template <class U, class ... Us>
        inline
        void variant_impl<Ts...>::matcher(const std::function<void (U&&)>& branch,
                                          const std::function<void (Us&&)>& ... branches) &&
        {
            if (ndgpp::tuple_index<U, std::tuple<Ts...>>::value == this->index)
            {
                branch(std::move(*this).get<ndgpp::tuple_index<U, std::tuple<Ts...>>::value>().get());
            }
            else
            {
                std::move(*this).matcher(branches...);
            }
        }

        template <class ... Ts>
        inline
        void variant_impl<Ts...>::match(const std::function<void (const Ts&)>& ... branches) const &
        {
            matcher(branches...);
        }

        template <class ... Ts>
        inline
        void variant_impl<Ts...>::match(const std::function<void (Ts&&)>& ... branches) &&
        {
            std::move(*this).matcher(branches...);
        }
    }
}

#endif
