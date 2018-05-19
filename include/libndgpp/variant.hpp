#ifndef LIBNDGPP_VARIANT_HPP
#define LIBNDGPP_VARIANT_HPP

#include <type_traits>
#include <functional>
#include <tuple>

#include "overload.hpp"
#include "source_location.hpp"
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
    struct monostate {};

    namespace detail
    {
        template <class Visitor, class ... Us, std::size_t ... I>
        constexpr decltype(auto)
        visit_types(Visitor&& visitor, const variant<Us...>& v, std::index_sequence<I...>);
    }

    /** Type safe union based on std::variant
     *
     *  @tparam Ts The types that may be stored in this variant. All
     *             types must be (possibly cv-qualified) non-array
     *             object types.
     *
     *  An instance of std::variant at any given time either holds a
     *  value of one of its alternative types, or in the case of error
     *  - no value (this state is hard to achieve, see
     *  ndgpp::valueless_by_exception).
     *
     *  As with unions, if a variant holds a value of some object type
     *  T, the object representation of T is allocated directly within
     *  the object representation of the variant itself. Variant is
     *  not allowed to allocate additional (dynamic) memory.
     *
     *  A variant is not permitted to hold references, arrays, or the
     *  type void. Empty variants are also ill-formed
     *  (ndgpp::variant<ndgpp::monostate> can be used instead).
     *
     *  Consistent with the behavior of unions during aggregate
     *  initialization, a default-constructed variant holds a value of
     *  its first alternative, unless that alternative is not
     *  default-constructible (in which case the variant is not
     *  default-constructible either). The helper class std::monostate
     *  can be used to make such variants default-constructible.
     */
    template <class ... Ts>
    class variant:
        public ndgpp::copy_control<Ts...>,
        public ndgpp::move_control<Ts...>
    {
        using first_type = std::tuple_element_t<0, std::tuple<Ts...>>;

        public:

        /** Default constructs a variant
         *
         *  Constructs a variant holding the value-initialized value
         *  of the first alternative (index() is zero). This
         *  constructor is constexpr if and only if the value
         *  initialization of the alternative type T_0 would satisfy
         *  the requirements for a constexpr function. This overload
         *  only participates in overload resolution if
         *  std::is_default_constructible_v<T_0> is true.
         */
        template <bool B = std::is_default_constructible<first_type>::value,
                  typename std::enable_if<B, int>::type = 0>
        constexpr variant() noexcept(std::is_nothrow_default_constructible<first_type>::value)
        {}

        /** Converting constructor
         *
         *  @tparam U The type the variant will be constructed with
         *
         */
        template <class U, class X = ndgpp::disable_if_same_or_derived<variant<Ts...>, std::decay_t<U>>>
        constexpr variant(U&& u) noexcept(std::is_nothrow_constructible<std::decay_t<U>>::value):
            impl_(std::forward<U>(u))
        {}

        /**
         * @breif Returns the alternative index the variant contains
         *        or ndgpp::variant_npos if the variant is
         *        valuess_by_exception.
         */
        constexpr std::size_t index() const noexcept;

        /// Returns true if the variant is valueless_by_exception
        constexpr bool valueless_by_exception() const noexcept;

        /** Creates a new value in-place, in an existing variant object
         *
         *  @tparam I The index of the type to create
         *  @tparam Args The types of the parameters used to construct type T_I
         *
         *  @param args The parameters used to construct type T_I
         *
         *  First, destroys the currently contained value (if
         *  any). Then direct-initializes the contained value as if
         *  constructing a value of type T_I with the arguments
         *  std::forward<Args>(args).... If an exception is thrown,
         *  *this may become valueless_by_exception. This overload
         *  only participates in overload resolution if
         *  std::is_constructible_v<T_I, Args...> is true. The
         *  behavior is undefined if I is not less than
         *  sizeof...(Types)
         */
        template <std::size_t I, class ... Args>
        variant_alternative_t<I, variant<Ts...>>& emplace(Args&& ... args);

        /** Returns true if the variant holds a ndgpp::monostate, false otherwise
         *
         *  This member function is only available if one of the
         *  variant's types is a ndgpp::monostate.
         */
        explicit
        operator bool() const noexcept;

        private:

        ndgpp::detail::variant_impl<Ts...> impl_;


        // Friend functions

        template <std::size_t I, class ... Us>
        friend constexpr std::add_pointer_t<const variant_alternative_t<I, variant<Us...>>>
        get_if(const variant<Us...> &) noexcept;

        template <std::size_t I, class ... Us>
        friend constexpr std::add_pointer_t<variant_alternative_t<I, variant<Us...>>>
        get_if(variant<Us...> &) noexcept;

        template <class Visitor, class ... Us>
        friend
        constexpr decltype(auto)
        visit(Visitor&& visitor, variant<Us...>& v);

        template <class Visitor, class ... Us>
        friend
        constexpr decltype(auto)
        visit(Visitor&& visitor, const variant<Us...>& v);
    };

    /** Returns true if the variant holds the given alternative
     *
     *  @tparam T The alternative type to check.
     *  @tparam Ts The variant's list of alternatives
     *
     *  @param v The variant to check
     *
     *  @note The call is ill-formed if T appears more than once
     */
    template <class T, class ... Ts>
    inline
    constexpr bool holds_alternative(const variant<Ts...>& v) noexcept;


    /** Applies the provided visitor to the variant parameter
     *
     *  @tparam Visitor The visitor's type
     *  @tparam Ts The variant's list of alternatives.
     *
     *  @param visitor The visitor
     *  @param v The variant
     *
     *  @return The return value of the selected visitor invocation
     *
     *  @throws ndgpp::bad_variant_access if the variant is ndgpp::valueless_by_exception
     */
    template <class Visitor, class ... Ts>
    constexpr decltype(auto) visit(Visitor&& visitor, const variant<Ts...>& v);

    /** Indexed based non-throwing variant accessor
     *
     *  @tparam I The index to look up
     *
     *  @param v The variant
     *
     *  @return if the variant contains the alternative at index I,
     *          then a pointer to the value at index I; otherwise,
     *          nullptr.
     *
     *  @throws nothing
     */
    template <std::size_t I, class ... Ts>
    constexpr std::add_pointer_t<const variant_alternative_t<I, variant<Ts...>>>
    get_if(const variant<Ts...> & v) noexcept;


    /** Indexed based non-throwing variant accessor
     *
     *  @tparam I The index to look up
     *
     *  @param v The variant
     *
     *  @return if the variant contains the alternative at index I,
     *          then a pointer to the value at index I; otherwise,
     *          nullptr.
     *
     *  @throws nothing
     */
    template <std::size_t I, class ... Ts>
    constexpr std::add_pointer_t<variant_alternative_t<I, variant<Ts...>>>
    get_if(variant<Ts...> & v) noexcept;

    template <class ... Ts>
    inline
    constexpr std::size_t variant<Ts...>::index() const noexcept
    {
        return impl_.index;
    }

    template <class ... Ts>
    inline
    constexpr bool variant<Ts...>::valueless_by_exception() const noexcept
    {
        return this->impl_.valueless_by_exception();
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
        static_assert(ndgpp::tuple_contains<ndgpp::monostate, std::tuple<Ts...>>::value,
                      "operator bool is only available with a variant that contains a ndgpp::monostate");

        return !ndgpp::holds_alternative<ndgpp::monostate>(*this);
    }

    template <class T, class ... Ts>
    inline
    constexpr bool holds_alternative(const variant<Ts...>& v) noexcept
    {
        static_assert(ndgpp::tuple_contains<T, std::tuple<Ts...>>::value,
                      "T is not an alternative for this variant");
        return v.index() == ndgpp::tuple_index<T, std::tuple<Ts...>>::value;
    }

    template <std::size_t I, class ... Us>
    constexpr std::add_pointer_t<variant_alternative_t<I, ndgpp::variant<Us...>>>
    get_if(variant<Us...> &&) noexcept = delete;

    template <std::size_t I, class ... Us>
    constexpr std::add_pointer_t<const variant_alternative_t<I, ndgpp::variant<Us...>>>
    get_if(const variant<Us...> &&) noexcept = delete;

    template <std::size_t I, class ... Ts>
    inline
    constexpr std::add_pointer_t<const variant_alternative_t<I, variant<Ts...>>>
    get_if(const variant<Ts...> & v) noexcept
    {
        if (v.index() != I)
        {
            return nullptr;
        }

        return &v.impl_.template get<I>().get();
    }

    template <std::size_t I, class ... Ts>
    inline
    constexpr std::add_pointer_t<variant_alternative_t<I, variant<Ts...>>>
    get_if(variant<Ts...> & v) noexcept
    {
        return ndgpp::get_if<I>(const_cast<const variant<Ts...>>(v));
        if (v.index() != I)
        {
            return nullptr;
        }

        return &v.impl_.template get<I>().get();
    }

    template <class Visitor, class ... Ts>
    inline
    constexpr decltype(auto) visit(Visitor&& visitor, const variant<Ts...>& v)
    {
        return v.impl_.visit(std::forward<Visitor>(visitor));
    }

    template <class Visitor, class ... Ts>
    constexpr decltype(auto) visit(Visitor&& visitor, variant<Ts...>& v)
    {
        return v.impl_.visit(std::forward<Visitor>(visitor));
    }

    template <class ... Fs>
    decltype(auto) make_visitor(Fs ... fs)
    {
        return ndgpp::overload<Fs...>(fs...);
    }
}

#endif
