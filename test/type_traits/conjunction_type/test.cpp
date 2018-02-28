#include <type_traits>
#include <limits>
#include <gtest/gtest.h>

#include <libndgpp/type_traits/conjunction_type.hpp>

struct test_false_type
{
    static constexpr bool value = false;
};

TEST(conjunction, false_check)
{
    {
        using conjunction_t = ndgpp::conjunction_type<std::true_type, std::false_type>;
        const auto conjunction = conjunction_t {};

        EXPECT_FALSE(conjunction_t::value);
        EXPECT_FALSE(conjunction.value);
    }

    {
        using conjunction_t = ndgpp::conjunction_type<std::false_type, std::true_type>;
        const auto conjunction = conjunction_t {};

        EXPECT_FALSE(conjunction_t::value);
        EXPECT_FALSE(conjunction.value);

        constexpr bool is_base = std::is_base_of<std::false_type, conjunction_t>::value;
        EXPECT_TRUE(is_base);
    }

    {
        using conjunction_t = ndgpp::conjunction_type<test_false_type, std::true_type>;
        const auto conjunction = conjunction_t {};

        EXPECT_FALSE(conjunction_t::value);
        EXPECT_FALSE(conjunction.value);
    }
}

TEST(conjunction, true_check)
{
    using conjunction_t = ndgpp::conjunction_type<std::true_type, std::true_type>;
    const auto conjunction = conjunction_t {};

    EXPECT_TRUE(conjunction_t::value);
    EXPECT_TRUE(conjunction.value);
}
