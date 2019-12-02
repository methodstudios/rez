#include <gtest/gtest.h>

#include "../version/Bound.hpp"

#include <tuple>

class LowerBoundToStringTest : public testing::TestWithParam<std::tuple<string_view, bool, string_view>>{};
TEST_P(LowerBoundToStringTest, ToString)
{
    const auto& p = GetParam();
    const auto& version_a_parm = std::get<0>(p);
    const auto& inclusive_parm = std::get<1>(p);
    const auto& result_parm = std::get<2>(p);

    AlphanumericVersion version = Factory<AlphanumericVersion>::Create(version_a_parm);
    LowerBound<AlphanumericVersion> l_bound{version, inclusive_parm};

    const auto bound_str = std::string{l_bound};
    ASSERT_EQ(result_parm, bound_str);
}
INSTANTIATE_TEST_SUITE_P(ToString, LowerBoundToStringTest,
                         testing::Values(
                             std::make_tuple("", true, ""),
                             std::make_tuple("", false, ">"),
                             std::make_tuple("0", false, ">0"),
                             std::make_tuple("0", true, "0+"),
                             std::make_tuple("foo", false, ">foo"),
                             std::make_tuple("foo", true, "foo+")
                         ));


class LowerBoundCompareTest : public testing::TestWithParam<std::tuple<string_view, bool, string_view, bool>>{};
TEST_P(LowerBoundCompareTest, ContainsVersion)
{
    const auto& p = GetParam();
    const auto& version_a_parm = std::get<0>(p);
    const auto& inclusive_parm = std::get<1>(p);
    const auto& version_b_parm = std::get<2>(p);

    AlphanumericVersion version_a = Factory<AlphanumericVersion>::Create(version_a_parm);
    AlphanumericVersion version_b = Factory<AlphanumericVersion>::Create(version_b_parm);

    LowerBound<AlphanumericVersion> l_bound{version_a, inclusive_parm};

    const auto& equals_parm = std::get<3>(p);
    ASSERT_EQ(l_bound.contains_version(version_b), equals_parm);
}
INSTANTIATE_TEST_SUITE_P(ContainsVersion, LowerBoundCompareTest,
                         testing::Values(
                             std::make_tuple("0.0.1", false, "0.0.1", false),
                             std::make_tuple("0.0.1", false, "0.0.2", false)
                         ));

TEST(A,B)
{
}


