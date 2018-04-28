#ifndef LIBNDGPP_OVERLOAD_HPP
#define LIBNDGPP_OVERLOAD_HPP

namespace ndgpp
{
    template <class ... Fs>
    struct overload;

    template <class F, class ... Fs>
    struct overload<F, Fs...>: F, overload<Fs...>
    {
        overload(F f, Fs ... fs):
            F(f),
            overload<Fs...>(fs...)
        {}

        using F::operator();
        using overload<Fs...>::operator();
    };

    template <class F>
    struct overload<F>: F
    {
        overload(F f):
            F(f)
        {}

        using F::operator();
    };
}

#endif
