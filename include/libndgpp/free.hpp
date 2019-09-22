#ifndef LIBNDGPP_FREE_HPP
#define LIBNDGPP_FREE_HPP

#include <cstdlib>

namespace ndgpp
{
    struct free
    {
        void operator() (void * p)
        {
            std::free(p);
        }
    };
}

#endif
