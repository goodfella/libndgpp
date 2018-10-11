#include <gtest/gtest.h>

#include <libndgpp/bounded_integer.hpp>

TEST(ctor, min_ctor)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    constexpr bounded_integer b (ndgpp::bounded_integer_min);
    EXPECT_EQ(bounded_integer::min(), b.get());
}

TEST(ctor, max_ctor)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    constexpr bounded_integer b(ndgpp::bounded_integer_max);
    EXPECT_EQ(bounded_integer::max(), b.get());
}

TEST(ctor, min_value_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(1);

    EXPECT_EQ(1, bounded_integer::min());
    EXPECT_EQ(1, b.get());
}

TEST(ctor, max_value_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(2);

    EXPECT_EQ(2, bounded_integer::max());
    EXPECT_EQ(2, b.get());
}

TEST(ctor, underflow_value_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;

    auto throws = [] () {
        bounded_integer b(-1);
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_underflow>);
}

TEST(ctor, overflow_value_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;

    auto throws = [] () {
        bounded_integer b(3);
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_overflow>);
}

TEST(ctor, min_other_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(1U);

    EXPECT_EQ(1, b.get());
}

TEST(ctor, max_other_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(2U);

    EXPECT_EQ(2, b.get());
}

TEST(ctor, underflow_other_type)
{
    using bounded_integer = ndgpp::bounded_integer<unsigned int, 1, 2>;

    auto throws = [] () {
        bounded_integer b(-1);
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_underflow>);
}

TEST(ctor, overflow_other_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;

    auto throws = [] () {
        bounded_integer b(3U);
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_overflow>);
}

TEST(ctor, min_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(std::string {"1"});

    EXPECT_EQ(1, b.get());
}

TEST(ctor, max_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(std::string {"2"});

    EXPECT_EQ(2, b.get());
}

TEST(ctor, offseted_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(std::string {"a2"}, 1);

    EXPECT_EQ(2, b.get());
}

TEST(ctor, octal_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 8>;
    bounded_integer b(std::string {"010"});

    EXPECT_EQ(8, b.get());
}

TEST(ctor, hex_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 16>;
    bounded_integer b(std::string {"0x10"});

    EXPECT_EQ(16, b.get());
}

TEST(ctor, underflow_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    auto throws = [] () {
        bounded_integer b(std::string {"-1"});
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_underflow>);
}

TEST(ctor, overflow_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    auto throws = [] () {
        bounded_integer b(std::string {"3"});
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_overflow>);
}

TEST(ctor, invalid_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    auto throws = [] () {
        bounded_integer b(std::string {"a"});
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_invalid>);
}

TEST(ctor, offseted_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(std::string {"a2"}, 1);

    EXPECT_EQ(2, b.get());
}

TEST(ctor, octal_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 8>;
    bounded_integer b(std::string {"010"});

    EXPECT_EQ(8, b.get());
}

TEST(ctor, hex_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 16>;
    bounded_integer b(std::string {"0x10"});

    EXPECT_EQ(16, b.get());
}

TEST(ctor, min_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b("1");

    EXPECT_EQ(1, b.get());
}

TEST(ctor, max_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b("2");

    EXPECT_EQ(2, b.get());
}

TEST(ctor, underflow_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    auto throws = [] () {
        bounded_integer b("-1");
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_underflow>);
}

TEST(ctor, overflow_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    auto throws = [] () {
        bounded_integer b("3");
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_overflow>);
}

TEST(ctor, invalid_char_string_type)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    auto throws = [] () {
        bounded_integer b("a");
    };

    EXPECT_THROW(throws(), ndgpp::error<ndgpp::bounded_integer_invalid>);
}

TEST(ctor, char_string_char_string_delimiter)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b("1:2", 0, 0, ":");
    EXPECT_EQ(1, b.get());
}

TEST(ctor, char_string_iter_delimiter)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    const std::vector<char> d = {':', ','};
    bounded_integer b("1:2", 0, 0, d.cbegin(), d.cend());
    EXPECT_EQ(1, b.get());
}

TEST(ctor, string_char_string_delimiter)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(std::string("1:2"), 0, 0, ":");
    EXPECT_EQ(1, b.get());
}

TEST(ctor, string_iter_delimiter)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    const std::vector<char> d = {':', ','};
    bounded_integer b(std::string("1:2"), 0, 0, d.cbegin(), d.cend());
    EXPECT_EQ(1, b.get());
}

TEST(ctor, integral_constant)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    constexpr bounded_integer b(std::integral_constant<int, 2>{});
    EXPECT_EQ(2, b.get());

    std::array<char, b.get()> array;
    EXPECT_EQ(2U, array.size());
}

TEST(operators_test, less_than_bounded_integers)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1("1");
    bounded_integer b2("2");

    EXPECT_TRUE(b1 < b2);
    EXPECT_FALSE(b2 < b1);
}

TEST(operators_test, less_than_bounded_integer_numeric)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1("1");

    EXPECT_TRUE(b1 < 2);
    EXPECT_FALSE(b1 < 0);
}

TEST(operators_test, less_than_numeric_bounded_integer)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1("2");

    EXPECT_TRUE(1 < b1);
    EXPECT_FALSE(3 < b1);
}

TEST(operators_test, equal_to_bounded_integers)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1("1");
    bounded_integer b2("1");

    EXPECT_TRUE(b1 == b2);
}

TEST(operators_test, equal_to_bounded_integer_numeric)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1("1");

    EXPECT_TRUE(b1 == 1);
}

TEST(operators_test, equal_to_numeric_bounded_integer)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1("1");

    EXPECT_TRUE(1 == b1);
}

TEST(hash_test, equals_value_type_hash)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b(1);

    std::hash<int> int_hasher;
    std::hash<bounded_integer> b_hasher;

    EXPECT_EQ(int_hasher(b.get()), b_hasher(b));
}

TEST(swap_test, correctness)
{
    using bounded_integer = ndgpp::bounded_integer<int, 1, 2>;
    bounded_integer b1(1);
    bounded_integer b2(2);

    std::swap(b1, b2);

    EXPECT_EQ(2, b1.get());
    EXPECT_EQ(1, b2.get());
}
