#include <gtest/gtest.h>

#include "../version/Version.hpp"

#include <tuple>

//
// Numeric Version
//
class NumericVersionThrowTest : public ::testing::TestWithParam<string_view>{};
TEST_P(NumericVersionThrowTest, InitThrow)
{
    EXPECT_THROW(Factory<NumericVersion>::Create(GetParam()), VersionError);
}
INSTANTIATE_TEST_SUITE_P(InitThrow, NumericVersionThrowTest,
                         testing::Values(
                             " ", ".", "-", "--", "..", "!", ">", ".10.", ".10", "10.", "abc.1"
                         ));

class NumericVersionNoThrowTest : public ::testing::TestWithParam<string_view>{};
TEST_P(NumericVersionNoThrowTest, InitNoThrow)
{
    EXPECT_NO_THROW(Factory<AlphanumericVersion>::Create(GetParam()));
}
INSTANTIATE_TEST_SUITE_P(InitNoThrow, NumericVersionNoThrowTest,
                         testing::Values(
                             "", "0.1-10", "1.2.3", "1-2-3"
                         ));

//
// Alphanumeric Version
//

class AlphanumericVersionThrowTest : public ::testing::TestWithParam<string_view>{};
TEST_P(AlphanumericVersionThrowTest, InitThrow)
{
    EXPECT_THROW(Factory<AlphanumericVersion>::Create(GetParam()), VersionError);
}
INSTANTIATE_TEST_SUITE_P(InitThrow, AlphanumericVersionThrowTest,
                         testing::Values(
                             " ", ".", "-", "--", "..", "!", ">", ".10.", ".10", "10."
                         ));

class AlphanumericVersionNoThrowTest : public ::testing::TestWithParam<string_view>{};
TEST_P(AlphanumericVersionNoThrowTest, InitNoThrow)
{
    EXPECT_NO_THROW(Factory<AlphanumericVersion>::Create(GetParam()));
}
INSTANTIATE_TEST_SUITE_P(InitNoThrow, AlphanumericVersionNoThrowTest,
                         testing::Values(
                             "", "foo", "1_a", "0.1-10", "1.2.3", "1-2-3", "foo-bar", "foo_bar"
                         ));

class AlphanumericVersionTokenCountTest : public ::testing::TestWithParam<std::pair<string_view, rez_int>>{};
TEST_P(AlphanumericVersionTokenCountTest, TokenCount)
{
    ASSERT_EQ(Factory<AlphanumericVersion>::Create(GetParam().first)->size(), GetParam().second);
}
INSTANTIATE_TEST_SUITE_P(TokenCount, AlphanumericVersionTokenCountTest,
                         testing::Values(
                             std::make_pair("", 0),
                             std::make_pair("1", 1),
                             std::make_pair("foo_bar", 1),
                             std::make_pair("1-2", 2),
                             std::make_pair("1.2", 2),
                             std::make_pair("1.2-3", 3),
                             std::make_pair("foo1.2bar-3world5", 3)
                         ));


class AlphanumericVersionToString : public ::testing::TestWithParam<std::pair<string_view, string_view>>{};
TEST_P(AlphanumericVersionToString, ToString)
{
    auto version = Factory<AlphanumericVersion>::Create(GetParam().first);
    auto str = std::string(version);
    ASSERT_TRUE(GetParam().second == str);
}
INSTANTIATE_TEST_SUITE_P(ToString, AlphanumericVersionToString,
                         testing::Values(
                             std::make_pair("", "[INF]"),
                             std::make_pair("1", "1"),
                             std::make_pair("foo_bar", "foo_bar"),
                             std::make_pair("1-2", "1-2"),
                             std::make_pair("1.2", "1.2"),
                             std::make_pair("1.2-3", "1.2-3"),
                             std::make_pair("foo1.2bar-3world5", "foo1.2bar-3world5")
                         ));