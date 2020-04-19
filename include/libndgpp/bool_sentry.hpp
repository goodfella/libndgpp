#ifndef LIBNDGPP_BOOL_SENTRY_HPP
#define LIBNDGPP_BOOL_SENTRY_HPP

namespace ndgpp
{
    class bool_sentry
    {
        public:

        bool_sentry(bool & value) noexcept;

        bool_sentry(const bool_sentry &) = delete;
        bool_sentry(bool_sentry &&) = delete;

        bool_sentry & operator = (const bool_sentry &) = delete;
        bool_sentry & operator = (bool_sentry &&) = delete;

        ~bool_sentry() noexcept;

        private:

        bool * value_ptr_;
        bool original_value_;
    };
}

#endif
