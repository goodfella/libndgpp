#include <gtest/gtest.h>

#include <array>
#include <cstring>
#include <sstream>

#include <libndgpp/network_byte_order.hpp>
#include <libndgpp/strto.hpp>

using host_types = ::testing::Types<uint16_t, uint32_t, uint64_t>;

template <class T>
struct values;

template <>
struct values<uint16_t>
{
    static constexpr uint16_t scalar = 0xabcd;
    static constexpr std::array<uint8_t, 2> array = {0xab, 0xcd};
};

constexpr uint16_t values<uint16_t>::scalar;
constexpr std::array<uint8_t, 2> values<uint16_t>::array;

template <>
struct values<uint32_t>
{
    static constexpr uint32_t scalar = 0xabcdefbc;
    static constexpr std::array<uint8_t, 4> array = {0xab, 0xcd, 0xef, 0xbc};
};

constexpr uint32_t values<uint32_t>::scalar;
constexpr std::array<uint8_t, 4> values<uint32_t>::array;

template <>
struct values<uint64_t>
{
    static constexpr uint64_t scalar = 0xabcdefbcabcdefbc;
    static constexpr std::array<uint8_t, 8> array = {0xab, 0xcd, 0xef, 0xbc, 0xab, 0xcd, 0xef, 0xbc};
};

constexpr uint64_t values<uint64_t>::scalar;
constexpr std::array<uint8_t, 8> values<uint64_t>::array;

template <class T>
class member_test: public ::testing::Test
{
    public:

    using value_type = T;
    using array_type = std::array<uint8_t, sizeof(T)>;
};

TYPED_TEST_CASE(member_test, host_types);

TYPED_TEST(member_test, value_type_ctor)
{
    using value_type = typename TestFixture::value_type;
    const ndgpp::network_byte_order<value_type> nbo(values<value_type>::scalar);

    EXPECT_EQ(values<value_type>::scalar, static_cast<value_type>(nbo));

    typename TestFixture::array_type buf;
    std::memcpy(buf.data(), &nbo, nbo.size());
    EXPECT_EQ(values<value_type>::array, buf);
}

TYPED_TEST(member_test, value_type_assignment)
{
    using value_type = typename TestFixture::value_type;

    ndgpp::network_byte_order<value_type> nbo(values<value_type>::scalar);
    ndgpp::network_byte_order<value_type> & nbo2 = nbo = values<value_type>::scalar;

    EXPECT_EQ(values<value_type>::scalar, static_cast<value_type>(nbo));
    EXPECT_EQ(std::addressof(nbo), std::addressof(nbo2));

    typename TestFixture::array_type buf;
    std::memcpy(buf.data(), &nbo, nbo.size());
    EXPECT_EQ(values<value_type>::array, buf);
}

template <class T>
class operator_test: public ::testing::Test
{
    public:

    using value_type = T;

    operator_test():
        nb1{values<T>::scalar},
        nb2{values<T>::scalar + 1},
        nb3{values<T>::scalar + 2}
    {}

    ndgpp::network_byte_order<T> nb1;
    ndgpp::network_byte_order<T> nb2;
    ndgpp::network_byte_order<T> nb3;

    const T hb1 = values<T>::scalar;
    const T hb2 = values<T>::scalar + 1;
    const T hb3 = values<T>::scalar + 2;
};

TYPED_TEST_CASE(operator_test, host_types);

TYPED_TEST(operator_test, equality)
{
    EXPECT_EQ(this->nb1, this->nb1);
    EXPECT_FALSE(this->nb1 == this->nb2);

    EXPECT_EQ(this->nb1, this->hb1);
    EXPECT_FALSE(this->nb1 == this->hb2);
}

TYPED_TEST(operator_test, not_equal)
{
    EXPECT_TRUE(this->nb1 != this->nb2);
    EXPECT_FALSE(this->nb1 != this->nb1);

    EXPECT_TRUE(this->nb1 != this->hb2);
    EXPECT_FALSE(this->nb1 != this->hb1);
}

TYPED_TEST(operator_test, less_than)
{
    EXPECT_LT(this->nb1, this->nb2);
    EXPECT_FALSE(this->nb2 < this->nb1);

    EXPECT_LT(this->nb1, this->hb2);
    EXPECT_FALSE(this->nb2 < this->hb1);
}

TYPED_TEST(operator_test, greater_than)
{
    EXPECT_GT(this->nb2, this->nb1);
    EXPECT_FALSE(this->nb1 > this->nb2);

    EXPECT_GT(this->nb2, this->nb1);
    EXPECT_FALSE(this->nb1 > this->hb2);
}

TYPED_TEST(operator_test, less_than_equal)
{
    EXPECT_LE(this->nb1, this->nb1);
    EXPECT_LE(this->nb1, this->nb2);
    EXPECT_FALSE(this->nb2 <= this->nb1);

    EXPECT_LE(this->nb1, this->hb1);
    EXPECT_LE(this->nb1, this->hb2);
    EXPECT_FALSE(this->nb2 <= this->hb1);
}

TYPED_TEST(operator_test, greater_than_equal)
{
    EXPECT_GE(this->nb1, this->nb1);
    EXPECT_GE(this->nb2, this->nb1);
    EXPECT_FALSE(this->nb1 >= this->nb2);

    EXPECT_GE(this->nb1, this->hb1);
    EXPECT_GE(this->nb2, this->hb1);
    EXPECT_FALSE(this->nb1 >= this->hb2);
}

TYPED_TEST(operator_test, insertion)
{
    using value_type = typename TestFixture::value_type;

    std::stringstream ss;
    ss << this->nb1;

    const auto val = ndgpp::strtoi<value_type>(ss.str().c_str());
    ASSERT_TRUE(static_cast<bool>(val));

    EXPECT_EQ(val.value(), this->nb1);
}
