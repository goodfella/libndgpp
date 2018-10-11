#ifndef LIBNDGPP_DETAIL_SAFE_OPERATORS_HPP
#define LIBNDGPP_DETAIL_SAFE_OPERATORS_HPP

#include <type_traits>

namespace ndgpp
{
namespace detail
{
    template <bool Lhs_Signed, bool Rhs_Signed>
    struct integral_safe_ops;

    template <>
    struct integral_safe_ops<true, true>
    {
        template <class Lhs, class Rhs>
        static constexpr bool lt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs and Rhs are not signed");
            return lhs < rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool gt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs and Rhs are not signed");
            return lhs > rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool lte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs and Rhs are not signed");
            return lhs <= rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool gte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs and Rhs are not signed");
            return lhs >= rhs;
        }
    };

    template <>
    struct integral_safe_ops<true, false>
    {
        template <class Lhs, class Rhs>
        static constexpr bool lt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Rhs is not unsigned");

            return lhs < 0 || static_cast<std::make_unsigned_t<Lhs>>(lhs) < rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool gt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Rhs is not unsigned");

            return lhs >= 0 && static_cast<std::make_unsigned_t<Lhs>>(lhs) > rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool lte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Rhs is not unsigned");

            return lhs < 0 || static_cast<std::make_unsigned_t<Lhs>>(lhs) <= rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool gte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_signed<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Rhs is not unsigned");

            return lhs >= 0 && static_cast<std::make_unsigned_t<Lhs>>(lhs) >= rhs;
        }
    };

    template <>
    struct integral_safe_ops<false, true>
    {
        template <class Lhs, class Rhs>
        static constexpr bool lt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs is not unsigned");

            return rhs < 0 || lhs < static_cast<typename std::make_unsigned_t<Rhs>>(rhs);
        }

        template <class Lhs, class Rhs>
        static constexpr bool gt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs is not unsigned");

            return rhs < 0 || lhs > static_cast<typename std::make_unsigned_t<Rhs>>(rhs);
        }

        template <class Lhs, class Rhs>
        static constexpr bool lte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs is not unsigned");

            return rhs >= 0 && lhs <= static_cast<typename std::make_unsigned_t<Rhs>>(rhs);
        }

        template <class Lhs, class Rhs>
        static constexpr bool gte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_signed<Rhs>::value,
                          "Lhs is not unsigned");

            return rhs < 0 || lhs >= static_cast<typename std::make_unsigned_t<Rhs>>(rhs);
        }
    };

    template <>
    struct integral_safe_ops<false, false>
    {
        template <class Lhs, class Rhs>
        static constexpr bool lt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Lhs and Rhs are not unsigned");

            return lhs < rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool gt(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Lhs and Rhs are not unsigned");

            return lhs > rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool lte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Lhs and Rhs are not unsigned");

            return lhs <= rhs;
        }

        template <class Lhs, class Rhs>
        static constexpr bool gte(const Lhs lhs, const Rhs rhs) noexcept
        {
            static_assert(std::is_unsigned<Lhs>::value && std::is_unsigned<Rhs>::value,
                          "Lhs and Rhs are not unsigned");

            return lhs >= rhs;
        }
    };
}
}

#endif
