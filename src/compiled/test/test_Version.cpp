#include <gtest/gtest.h>

#include "../version/Version.hpp"

#include <tuple>

using namespace testing;

//
// Numeric Version
//
class NumericVersionThrowTest : public TestWithParam<string_view>
{
};
TEST_P(NumericVersionThrowTest, InitThrow) { ASSERT_THROW(Factory<NumericVersion>::Create(GetParam()), VersionError); }
INSTANTIATE_TEST_SUITE_P(InitThrow, NumericVersionThrowTest,
                         Values(" ", ".", "-", "--", "..", "!", ">", ".10.", ".10", "10.", "abc.1"));

class NumericVersionNoThrowTest : public TestWithParam<string_view>
{
};
TEST_P(NumericVersionNoThrowTest, InitNoThrow) { ASSERT_NO_THROW(Factory<NumericVersion>::Create(GetParam())); }
INSTANTIATE_TEST_SUITE_P(InitNoThrow, NumericVersionNoThrowTest, Values("", "0.1-10", "1.2.3", "1-2-3"));

class NumericVersionCompareTest : public TestWithParam<std::pair<string_view, rez_int>>
{
};
TEST_P(NumericVersionCompareTest, Equals)
{
    auto version = Factory<NumericVersion>::Create(GetParam().first);
    ASSERT_EQ(version.Major(), GetParam().second);
}
INSTANTIATE_TEST_SUITE_P(Equals, NumericVersionCompareTest, Values(std::make_pair("0", 0), std::make_pair("10", 10)));

TEST(NumericVersion, Infinity)
{
    auto version = Factory<NumericVersion>::Create("1000000000");
    ASSERT_FALSE(version.IsInfinity());

    auto& infinity = NumericVersion::Inf();
    ASSERT_TRUE(infinity.IsInfinity());

    ASSERT_LT(version, infinity);
}

TEST(AlphanumericVersion, Infinity)
{
    auto version = Factory<AlphanumericVersion>::Create("hello_world-1000000000foo");
    ASSERT_FALSE(version.IsInfinity());

    auto& infinity = AlphanumericVersion::Inf();
    ASSERT_TRUE(infinity.IsInfinity());

    ASSERT_LT(version, infinity);
}

TEST(NumericVersion, IsEmpty)
{
    auto version = Factory<NumericVersion>::Create("");
    ASSERT_TRUE(version.IsEmpty());
}

TEST(AlphanumericVersion, IsEmpty)
{
    auto version = Factory<AlphanumericVersion>::Create("");
    ASSERT_TRUE(version.IsEmpty());
}

//
// Alphanumeric Version
//
class AlphanumericVersionThrowTest : public TestWithParam<string_view>
{
};
TEST_P(AlphanumericVersionThrowTest, InitThrow)
{
    ASSERT_THROW(Factory<AlphanumericVersion>::Create(GetParam()), VersionError);
}
INSTANTIATE_TEST_SUITE_P(InitThrow, AlphanumericVersionThrowTest,
                         Values(" ", ".", "-", "--", "..", "!", ">", ".10.", ".10", "10."));

class AlphanumericVersionNoThrowTest : public TestWithParam<string_view>
{
};
TEST_P(AlphanumericVersionNoThrowTest, InitNoThrow) { ASSERT_NO_THROW(Factory<AlphanumericVersion>::Create(GetParam())); }
INSTANTIATE_TEST_SUITE_P(InitNoThrow, AlphanumericVersionNoThrowTest,
                         Values("", "foo", "1_a", "0.1-10", "1.2.3", "1-2-3", "foo-bar", "foo_bar"));

class AlphanumericVersionTokenCountTest : public TestWithParam<std::pair<string_view, rez_int>>
{
};
TEST_P(AlphanumericVersionTokenCountTest, TokenCount)
{
    ASSERT_EQ(Factory<AlphanumericVersion>::Create(GetParam().first)->size(), GetParam().second);
}
INSTANTIATE_TEST_SUITE_P(TokenCount, AlphanumericVersionTokenCountTest,
                         Values(std::make_pair("", 0), std::make_pair("1", 1), std::make_pair("foo_bar", 1),
                                std::make_pair("1-2", 2), std::make_pair("1.2", 2), std::make_pair("1.2-3", 3),
                                std::make_pair("foo1.2bar-3world5", 3)));

class AlphanumericVersionToString : public TestWithParam<std::pair<string_view, string_view>>
{
};
TEST_P(AlphanumericVersionToString, ToString)
{
    auto version = Factory<AlphanumericVersion>::Create(GetParam().first);
    auto str = std::string(version);
    ASSERT_TRUE(GetParam().second == str);
}
INSTANTIATE_TEST_SUITE_P(ToString, AlphanumericVersionToString,
                         Values(std::make_pair("", "[INF]"), std::make_pair("1", "1"), std::make_pair("foo_bar", "foo_bar"),
                                std::make_pair("1-2", "1-2"), std::make_pair("1.2", "1.2"), std::make_pair("1.2-3", "1.2-3"),
                                std::make_pair("foo1.2bar-3world5", "foo1.2bar-3world5")));