#include <sstream>
#include <functional>

#include <gtest/gtest.h>

#include <libndgpp/ios/ios_base_flag_sentry.hpp>

TEST(ctor, test)
{
    std::stringstream ss;
    ndgpp::ios_base_flag_sentry sentry(std::ref(ss));

    EXPECT_EQ(ss.flags(), sentry.flags());
}

TEST(dtor, test)
{
    std::stringstream ss;
    const auto flags = ss.flags();
    {
        ndgpp::ios_base_flag_sentry sentry(std::ref(ss));
        ss.flags(std::ios_base::showbase | std::ios_base::skipws);
        EXPECT_NE(flags, ss.flags());
    }

    EXPECT_EQ(flags, ss.flags());
}
