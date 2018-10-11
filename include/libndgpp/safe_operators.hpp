#ifndef LIBNDGPP_SAFE_OPERATORS_HPP
#define LIBNDGPP_SAFE_OPERATORS_HPP

#include <type_traits>

#include <libndgpp/detail/safe_operators.hpp>

namespace ndgpp
{
namespace safe_op
{
    /** Sign safe less than comparison for two integers
     *
     *  @tparam Lhs The type of the left operand
     *  @tparam Rhs The type of the right operand
     *
     *  @param lhs The left operand
     *  @param rhs The right operand
     */
    template <class Lhs, class Rhs>
    inline auto lt(const Lhs lhs, const Rhs rhs) ->
        std::enable_if_t<std::is_integral<Lhs>::value &&
                         std::is_integral<Rhs>::value,
                         bool>
    {
        using safe_ops = ndgpp::detail::integral_safe_ops<std::is_signed<Lhs>::value,
                                                          std::is_signed<Rhs>::value>;
        return safe_ops::lt(lhs, rhs);
    }

    /** Sign safe greater than comparison for two integers
     *
     *  @tparam Lhs The type of the left operand
     *  @tparam Rhs The type of the right operand
     *
     *  @param lhs The left operand
     *  @param rhs The right operand
     */
    template <class Lhs, class Rhs>
    inline auto gt(const Lhs lhs, const Rhs rhs) ->
        std::enable_if_t<std::is_integral<Lhs>::value &&
                         std::is_integral<Rhs>::value,
                         bool>
    {
        using safe_ops = ndgpp::detail::integral_safe_ops<std::is_signed<Lhs>::value,
                                                          std::is_signed<Rhs>::value>;
        return safe_ops::gt(lhs, rhs);
    }

    /** Sign safe less than equal to comparison for two integers
     *
     *  @tparam Lhs The type of the left operand
     *  @tparam Rhs The type of the right operand
     *
     *  @param lhs The left operand
     *  @param rhs The right operand
     */
    template <class Lhs, class Rhs>
    inline auto lte(const Lhs lhs, const Rhs rhs) ->
        std::enable_if_t<std::is_integral<Lhs>::value &&
                         std::is_integral<Rhs>::value,
                         bool>
    {
        using safe_ops = ndgpp::detail::integral_safe_ops<std::is_signed<Lhs>::value,
                                                          std::is_signed<Rhs>::value>;
        return safe_ops::lte(lhs, rhs);
    }

    /** Sign safe greater than equal to comparison for two integers
     *
     *  @tparam Lhs The type of the left operand
     *  @tparam Rhs The type of the right operand
     *
     *  @param lhs The left operand
     *  @param rhs The right operand
     */
    template <class Lhs, class Rhs>
    inline auto gte(const Lhs lhs, const Rhs rhs) ->
        std::enable_if_t<std::is_integral<Lhs>::value &&
                         std::is_integral<Rhs>::value,
                         bool>
    {
        using safe_ops = ndgpp::detail::integral_safe_ops<std::is_signed<Lhs>::value,
                                                          std::is_signed<Rhs>::value>;
        return safe_ops::gte(lhs, rhs);
    }
}
}

#endif
