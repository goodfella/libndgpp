#include <libndgpp/bool_sentry.hpp>

ndgpp::bool_sentry::bool_sentry(bool & value) noexcept:
    value_ptr_(&value),
    original_value_(value)
{}

ndgpp::bool_sentry::~bool_sentry() noexcept
{
    *this->value_ptr_ = this->original_value_;
}
