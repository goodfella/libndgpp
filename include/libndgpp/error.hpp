#ifndef LIBNDGPP_ERROR_HPP
#define LIBNDGPP_ERROR_HPP

#include <type_traits>
#include <exception>
#include <stdexcept>
#include <functional>

#include "source_location.hpp"

namespace ndgpp
{
    class exception: public std::exception
    {
        public:

        exception(const ndgpp::source_location& location);

        const ndgpp::source_location& source_location() const noexcept;

        private:

        ndgpp::source_location source_location_;
    };

    ndgpp::exception::exception(const ndgpp::source_location& location):
        source_location_(location)
    {}

    inline const ndgpp::source_location& ndgpp::exception::source_location() const noexcept
    {
        return source_location_;
    }


    template <class InnerException>
    class error: public ndgpp::exception
    {
        public:

        using inner_exception_type = InnerException;

        template <class ... Args>
        error(const ndgpp::source_location& sourceLocation,
              Args&& ... args):
            ndgpp::exception(sourceLocation),
            inner_exception_(std::forward<Args>(args)...)
            {}

        const inner_exception_type& inner_exception() const noexcept;

        char const * what() const noexcept;

        private:

        inner_exception_type inner_exception_;
    };

    template <class InnerException>
    inline char const * ndgpp::error<InnerException>::what() const noexcept
    {
        return inner_exception_.what();
    }
}

#define NDGPP_ERROR(inner, ...) ndgpp::error<inner>(NDGPP_SOURCE_LOCATION, ##__VA_ARGS__)

#endif
