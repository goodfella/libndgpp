#ifndef LIBNDGPP_STRTO_RESULT_HPP
#define LIBNDGPP_STRTO_RESULT_HPP

#include <type_traits>

namespace ndgpp
{
    /** Represents the result of a strto conversion
     *
     *  @tparam T The integer type of the conversion
     */
    template <class T>
    class strto_result final
    {
        public:

        using value_type = std::decay_t<T>;

        struct invalid_t {};
        struct underflow_t {};
        struct overflow_t {};

        static constexpr invalid_t invalid_value = {};
        static constexpr underflow_t underflowed = {};
        static constexpr overflow_t overflowed = {};

        strto_result(const T value, char const * const unparsed);
        strto_result(invalid_t, char const * const unparsed);
        strto_result(underflow_t, char const * const unparsed);
        strto_result(overflow_t, char const * const unparsed);

        explicit operator bool() const noexcept;

        bool invalid() const noexcept;
        bool underflow() const noexcept;
        bool overflow() const noexcept;

        value_type value() const;

        char const * unparsed() const noexcept;

        private:

        enum class error_type
        {
            none,
            invalid,
            underflow,
            overflow,
        };

        T value_ = {};
        error_type error_;
        char const * unparsed_;
    };

    template <class T>
    strto_result<T>::strto_result(const T value, char const * const unparsed):
        value_(value),
        error_(error_type::none),
        unparsed_(unparsed)
    {}

    template <class T>
    strto_result<T>::strto_result(invalid_t, char const * const unparsed):
        error_(error_type::invalid),
        unparsed_(unparsed)
    {}

    template <class T>
    strto_result<T>::strto_result(underflow_t, char const * const unparsed):
        error_(error_type::underflow),
        unparsed_(unparsed)
    {}

    template <class T>
    strto_result<T>::strto_result(overflow_t, char const * const unparsed):
        error_(error_type::overflow),
        unparsed_(unparsed)
    {}

    template <class T>
    strto_result<T>::operator bool() const noexcept
    {
        return this->error_ == error_type::none;
    }

    template <class T>
    bool strto_result<T>::invalid() const noexcept
    {
        return this->error_ == error_type::invalid;
    }

    template <class T>
    bool strto_result<T>::underflow() const noexcept
    {
        return this->error_ == error_type::underflow;
    }

    template <class T>
    bool strto_result<T>::overflow() const noexcept
    {
        return this->error_ == error_type::overflow;
    }

    template <class T>
    std::decay_t<T> strto_result<T>::value() const
    {
        if (!(*this))
        {
            throw ndgpp_error(std::logic_error,
                              "strto_result value not set");
        }

        return this->value_;
    }

    template <class T>
    char const * strto_result<T>::unparsed() const noexcept
    {
        return this->unparsed_;
    }
}

#endif
