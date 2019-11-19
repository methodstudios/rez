#include <gtest/gtest.h>

#include "../version/token.hpp"

#include <limits>
#include <utility>
#include <tuple>

using namespace rez;

//
// Numeric Value
//

// initialization
class NumericTokenThrowTest : public ::testing::TestWithParam<const char*> {};
TEST_P(NumericTokenThrowTest, InitializationThrow)
{
    EXPECT_THROW(NumericToken{GetParam()}, std::invalid_argument);
}
INSTANTIATE_TEST_SUITE_P(InitializationThrow, NumericTokenThrowTest,
                         testing::Values("", " ", "-", "_", "1a", "a1", "abc", "A12", "-12", "1-123"));


class NumericTokenNoThrowTest : public ::testing::TestWithParam<const char*> {};
TEST_P(NumericTokenNoThrowTest, InitializationNoThrow)
{
    EXPECT_NO_THROW(NumericToken{GetParam()});
}
INSTANTIATE_TEST_SUITE_P(InitializationNoThrow, NumericTokenNoThrowTest,
                         testing::Values("0", "1234"));

// comparision

class NumericTokenComparisionTest : public ::testing::TestWithParam<std::pair<const char*, const char*>> {};
TEST_P(NumericTokenComparisionTest, IsLess)
{
    EXPECT_LT(NumericToken{GetParam().first}, NumericToken{GetParam().second});
}

INSTANTIATE_TEST_SUITE_P(Something, NumericTokenComparisionTest,
                         testing::Values(std::make_pair("10", "21")));

//
// Sub Value
//

// initialization
class SubTokenTest : public ::testing::TestWithParam<std::tuple<const char*, const char*, r_int>> {};
TEST_P(SubTokenTest, Initialization)
{
    const auto& param = GetParam();
    SubToken token{std::get<0>(param)};
    EXPECT_EQ(token.s, std::get<1>(param));
    EXPECT_EQ(token.n, std::get<2>(param));
}
INSTANTIATE_TEST_SUITE_P(Init, SubTokenTest,
                         testing::Values(
                             std::make_tuple("10", "10", 10),
                             std::make_tuple("01", "01", 1),
                             std::make_tuple("foo", "foo", rez::R_INT_INVALID),
                             std::make_tuple("foo12", "foo12", rez::R_INT_INVALID),
                             std::make_tuple("12foo", "12foo", rez::R_INT_INVALID),
                             std::make_tuple("-10", "-10", rez::R_INT_INVALID)
                             ));

//
// Alphanumeric Value
//

// initialization
class AlphanumericTokenTest : public ::testing::TestWithParam<std::tuple<const char*, std::initializer_list<SubToken>>> {};
TEST_P(AlphanumericTokenTest, Initialization)
{
    const auto& param = GetParam();

    const auto& string = std::get<0>(param);
    const auto& tokens = std::get<1>(param);

    AlphanumericToken at{string};

    ASSERT_EQ(at.Get().size(), tokens.size());
}
INSTANTIATE_TEST_SUITE_P(Init, AlphanumericTokenTest,
                         testing::Values(
                             std::make_tuple("hello_world", std::initializer_list<SubToken>{"hello_world"_st}),
                             std::make_tuple("hello_world28", std::initializer_list<SubToken>{"hello_world"_st, "28"_st})
                         ));