#ifndef LIBNDGPP_DETAIL_VARIANT_IMPL_HPP
#define LIBNDGPP_DETAIL_VARIANT_IMPL_HPP

#include <limits>
#include <tuple>
#include <functional>
#include <type_traits>
#include <utility>
#include <exception>

#include <libndgpp/variant_alternative.hpp>
#include <libndgpp/type_traits/conjunction_type.hpp>
#include <libndgpp/sfinae.hpp>
#include <libndgpp/source_location.hpp>
#include "variant_storage.hpp"

namespace ndgpp
{
    constexpr std::size_t variant_npos = std::numeric_limits<std::size_t>::max();

    struct bad_variant_access: public std::runtime_error
    {
        bad_variant_access(const char * what):
            std::runtime_error(what)
        {}
    };

    namespace detail
    {
        template <class ... Ts>
        struct variant_impl
        {
            std::size_t index;
            using first_type = std::tuple_element_t<0, std::tuple<Ts...>>;

            constexpr variant_impl():
                index(0)
            {
                new (std::addressof(storage)) ndgpp::detail::variant_storage<std::decay_t<first_type>>();
            }

            template <class U, class X = ndgpp::disable_if_same_or_derived<variant_impl<Ts...>, std::decay_t<U>>>
            constexpr variant_impl(U&& u):
                index(ndgpp::tuple_index<std::decay_t<U>, std::tuple<Ts...>>::value)
            {
                new (std::addressof(storage)) ndgpp::detail::variant_storage<std::decay_t<U>>(std::forward<U>(u));
            }

            // Copy and move constructors

            variant_impl(const variant_impl<Ts...>& other);

            static constexpr bool move_ctor_noexcept =
                ndgpp::conjunction_type<std::is_nothrow_move_constructible<Ts>...>::value;

            variant_impl(variant_impl<Ts...>&& other) noexcept(move_ctor_noexcept);

            // Copy and move assignment operators

            variant_impl & operator= (const variant_impl & other);

            static constexpr bool move_assign_noexcept =
                ndgpp::conjunction_type<std::is_nothrow_move_constructible<Ts> ...>::value &&
                ndgpp::conjunction_type<std::is_nothrow_move_assignable<Ts> ...>::value;

            variant_impl& operator= (variant_impl<Ts...>&& other) noexcept(move_assign_noexcept);

            // Misc member functions

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

            template <class Visitor>
            std::result_of_t<Visitor(variant_alternative_t<0, variant_impl<Ts...>>)>
            visit(Visitor&& visitor, std::integer_sequence<std::size_t>) const &;

            template <class Visitor, std::size_t ... I>
            decltype(auto)
            visit(Visitor&& visitor, std::index_sequence<I...>) const &;

            template <class Visitor>
            decltype(auto)
            visit(Visitor&& visitor) const &;


            template <class Visitor>
            std::result_of_t<Visitor(variant_alternative_t<0, variant_impl<Ts...>>)>
            visit(Visitor&& visitor, std::integer_sequence<std::size_t>) &&;

            template <class Visitor, std::size_t ... I>
            decltype(auto)
            visit(Visitor&& visitor, std::index_sequence<I...>) &&;

            template <class Visitor>
            decltype(auto)
            visit(Visitor&& visitor) &&;

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
        variant_impl<Ts...> & variant_impl<Ts...>::operator= (const variant_impl& other)
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
                this->index = ndgpp::variant_npos;
                this->storage_base().~variant_storage_base();
            }
            else if (other.index == this->index)
            {
                other.storage_base().copy_assign(this->storage_base().get_ptr());
            }
            else
            {
                const auto visitor = [this](const auto & v) {
                    using T = std::decay_t<decltype(v)>;
                    this->template emplace<ndgpp::tuple_index<T, std::tuple<Ts...>>::value>(v);
                };

                other.visit(visitor);
            }

            return *this;
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
                const auto visitor = [this](auto && v) {
                    using T = std::decay_t<decltype(v)>;
                    this->template emplace<ndgpp::tuple_index<T, std::tuple<Ts...>>::value>(std::move(v));
                };

                std::move(other).visit(visitor);
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
        template <class Visitor>
        std::result_of_t<Visitor(variant_alternative_t<0, variant_impl<Ts...>>)>
        variant_impl<Ts...>::visit(Visitor&& visitor, std::integer_sequence<std::size_t>) const &
        {
            throw bad_variant_access{NDGPP_SOURCE_LOCATION_STR};
        }

        template <class ... Ts>
        template <class Visitor, std::size_t ... I>
        decltype(auto)
        variant_impl<Ts...>::visit(Visitor && visitor, std::index_sequence<I...>) const &
        {
            constexpr std::size_t index = sizeof...(I) - 1;
            if (this->index == index)
            {
                // The variant contains the alternative at index I, so
                // visit it.
                return std::forward<Visitor>(visitor)(this->get<index>().get());
            }
            else
            {
                // The variant does not contain the alternative at
                // index I, so try the next index.  The "next index"
                // is generated by creating a std::index_sequence with
                // one less std::size_t template parameter.
                return this->visit(std::forward<Visitor>(visitor), std::make_index_sequence<index>());
            }
        }

        template <class ... Ts>
        template <class Visitor>
        decltype(auto)
        variant_impl<Ts...>::visit(Visitor && visitor) const &
        {
            return this->visit(std::forward<Visitor>(visitor), std::make_index_sequence<sizeof...(Ts)>());
        }



        template <class ... Ts>
        template <class Visitor>
        std::result_of_t<Visitor(variant_alternative_t<0, variant_impl<Ts...>>)>
        variant_impl<Ts...>::visit(Visitor&& visitor, std::integer_sequence<std::size_t>) &&
        {
            throw bad_variant_access{NDGPP_SOURCE_LOCATION_STR};
        }

        template <class ... Ts>
        template <class Visitor, std::size_t ... I>
        decltype(auto)
        variant_impl<Ts...>::visit(Visitor && visitor, std::index_sequence<I...>) &&
        {
            constexpr std::size_t index = sizeof...(I) - 1;
            if (this->index == index)
            {
                // The variant contains the alternative at index I, so
                // visit it.
                return std::forward<Visitor>(visitor)(std::move(*this).get<index>().get());
            }
            else
            {
                // The variant does not contain the alternative at
                // index I, so try the next index.  The "next index"
                // is generated by creating a std::index_sequence with
                // one less std::size_t template parameter.
                return std::move(*this).visit(std::forward<Visitor>(visitor), std::make_index_sequence<index>());
            }
        }

        template <class ... Ts>
        template <class Visitor>
        decltype(auto)
        variant_impl<Ts...>::visit(Visitor && visitor) &&
        {
            return std::move(*this).visit(std::forward<Visitor>(visitor), std::make_index_sequence<sizeof...(Ts)>());
        }
    }
}

#endif
