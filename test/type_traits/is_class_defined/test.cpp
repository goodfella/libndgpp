#include <type_traits>
#include <limits>
#include <gtest/gtest.h>

#include <libndgpp/type_traits/is_class_defined.hpp>

class undefined_class;
TEST(is_class_defined, undefined_class)
{
    constexpr bool value = ndgpp::is_class_defined<undefined_class>::value;
    EXPECT_FALSE(value);
}

class defined_class {};
TEST(is_class_defined, defined_class)
{
    constexpr bool value = ndgpp::is_class_defined<defined_class>::value;
    EXPECT_TRUE(value);
}
