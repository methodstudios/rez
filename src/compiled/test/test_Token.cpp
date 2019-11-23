#include <gtest/gtest.h>

#include "../version/Token.hpp"

#include <limits>
#include <utility>
#include <tuple>

//
// Numeric Token
//

// initialization
class NumericTokenThrowTest : public ::testing::TestWithParam<const char*> {};
TEST_P(NumericTokenThrowTest, InitializationThrow)
{
    EXPECT_THROW(NumericToken{GetParam()}, std::invalid_argument);
}
INSTANTIATE_TEST_SUITE_P(InitializationThrow, NumericTokenThrowTest,
                         testing::Values(
                             "", " ",
                             "-", "_",
                             "1a", "a1",
                             "1A", "A1",
                             "12A", "A12",
                             "abc", "ABC",
                             "-12", "12-",
                             "11-23"
                             ));


class NumericTokenNoThrowTest : public ::testing::TestWithParam<const char*> {};
TEST_P(NumericTokenNoThrowTest, InitializationNoThrow)
{
    EXPECT_NO_THROW(NumericToken{GetParam()});
}
INSTANTIATE_TEST_SUITE_P(InitializationNoThrow, NumericTokenNoThrowTest,
                         testing::Values(
                             "0", "1234"
                             ));

// comparision

class NumericTokenComparisionTest : public ::testing::TestWithParam<std::pair<const char*, const char*>> {};
TEST_P(NumericTokenComparisionTest, IsLess)
{
    EXPECT_LT(NumericToken{GetParam().first}, NumericToken{GetParam().second});
}

INSTANTIATE_TEST_SUITE_P(Something, NumericTokenComparisionTest,
                         testing::Values(
                             std::make_pair("10", "21")
                             ));

// string conversion

class NumericTokenStringTest : public testing::TestWithParam<std::pair<const char*, const char*>>{};
TEST_P(NumericTokenStringTest, IsEqual)
{
    const auto& p = GetParam();
    NumericToken nt{p.first};
    std::string str = to_string(nt);
    EXPECT_STREQ(str.c_str(), p.second);
}
INSTANTIATE_TEST_SUITE_P(Init, NumericTokenStringTest,
                         testing::Values(
                             std::make_pair("10", "10"),
                             std::make_pair("98712345", "98712345")
                         ));

//
// Sub Token
//

// initialization
class SubTokenTest : public ::testing::TestWithParam<std::tuple<const char*, const char*, rez_int>> {};
TEST_P(SubTokenTest, Initialization)
{
    const auto& p = GetParam();
    AlphanumericSubToken token{std::get<0>(p)};
    EXPECT_EQ(token.s, std::get<1>(p));
    EXPECT_EQ(token.n, std::get<2>(p));
}
INSTANTIATE_TEST_SUITE_P(Init, SubTokenTest,
                         testing::Values(
                             std::make_tuple("10", "10", 10),
                             std::make_tuple("01", "01", 1),
                             std::make_tuple("foo", "foo", REZ_INT_INVALID),
                             std::make_tuple("foo12", "foo12", REZ_INT_INVALID),
                             std::make_tuple("12foo", "12foo", REZ_INT_INVALID),
                             std::make_tuple("-10", "-10", REZ_INT_INVALID)
                             ));

//
// Alphanumeric Token
//

using SubTokenIL = std::initializer_list<AlphanumericSubToken>;

// initialization
class AlphanumericTokenTest : public ::testing::TestWithParam<std::tuple<const char*, SubTokenIL>> {};
TEST_P(AlphanumericTokenTest, Initialization)
{
    const auto& param = GetParam();
    const auto& string = std::get<0>(param);
    const auto& tokens = std::get<1>(param);

    AlphanumericToken at = create_alphanumeric_token(string);
    ASSERT_EQ(at.Get().size(), tokens.size());
}
INSTANTIATE_TEST_SUITE_P(Init, AlphanumericTokenTest,
                         testing::Values(
                             std::make_tuple("42", SubTokenIL{"42"_st}),
                             std::make_tuple("foo_bar", SubTokenIL{"foo_bar"_st}),
                             std::make_tuple("foo_bar42", SubTokenIL{"foo_bar"_st, "42"_st}),
                             std::make_tuple("42_foo_bar53", SubTokenIL{"42"_st, "_foo_bar"_st, "53"_st})
                             ));

// string conversion
class AlphanumericTokenStringTest : public testing::TestWithParam<std::pair<const char*, const char*>>{};
TEST_P(AlphanumericTokenStringTest, IsEqual)
{
    const auto& p = GetParam();
    AlphanumericToken at = create_alphanumeric_token(p.first);
    std::string str = to_string(at);
    EXPECT_STREQ(str.c_str(), p.second);
}
INSTANTIATE_TEST_SUITE_P(StringOperator, AlphanumericTokenStringTest,
                         testing::Values(
                             std::make_pair("12_hello_34_world_56_foo_78_bar_98", "12_hello_34_world_56_foo_78_bar_98")
                         ));
