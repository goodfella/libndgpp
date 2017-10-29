#ifndef LIBNDGPP_ALGORITHM_ACCUMULATE_HPP
#define LIBNDGPP_ALGORITHM_ACCUMULATE_HPP

#include <cstddef>
#include <cstdint>

namespace ndgpp
{
    /** Accumulates the value of each parameter
     *
     *  @tparam Args The types of each parameter to accumulate
     *
     *  @param args The parameters to accummulate
     *
     *  @return The sum of args
     */
    template <class ... Args>
    constexpr decltype(auto) accumulate(Args&& ... args);
}

#include "detail/accumulate.hpp"

#endif
