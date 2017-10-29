#ifndef LIBNDGPP_IOS_IOS_BASE_FLAG_SENTRY_HPP
#define LIBNDGPP_IOS_IOS_BASE_FLAG_SENTRY_HPP

#include <ios>
#include <functional>

namespace ndgpp
{
    /// Restores stores the flags of a std::ios_base object
    class ios_base_flag_sentry
    {
        public:

        /** Captures std::ios_base::flags for restoration during deconstruction
         *
         *  @param stream The std::ios_base object whose flags to restor
         */
        template <class S>
        ios_base_flag_sentry(const std::reference_wrapper<S> stream) noexcept;

        ios_base_flag_sentry(const ios_base_flag_sentry&) = delete;
        ios_base_flag_sentry& operator=(const ios_base_flag_sentry&) = delete;

        ios_base_flag_sentry(ios_base_flag_sentry&&) = delete;
        ios_base_flag_sentry& operator=(ios_base_flag_sentry&&) = delete;

        /// Retores the flags of the std:::ios_base object that's stored internally
        ~ios_base_flag_sentry();

        std::ios_base::fmtflags flags() const noexcept;
        const std::ios_base & stream() const noexcept;

        private:

        std::ios_base & stream() noexcept;

        std::reference_wrapper<std::ios_base> stream_;
        std::ios_base::fmtflags flags_;
    };

    template <class S>
    inline
    ios_base_flag_sentry::ios_base_flag_sentry(const std::reference_wrapper<S> stream) noexcept:
        stream_(stream.get()),
        flags_(stream.get().flags())
    {}

    inline
    ios_base_flag_sentry::~ios_base_flag_sentry()
    {
        this->stream().flags(this->flags());
    }

    inline
    std::ios_base::fmtflags ios_base_flag_sentry::flags() const noexcept
    {
        return this->flags_;
    }

    inline
    const std::ios_base & ios_base_flag_sentry::stream() const noexcept
    {
        return this->stream_;
    }

    inline
    std::ios_base & ios_base_flag_sentry::stream() noexcept
    {
        return this->stream_;
    }
}

#endif
