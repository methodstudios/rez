#include <gtest/gtest.h>

#include "../version/Bound.hpp"

#include <tuple>

//
// Lower Bound
//
class LowerBoundToStringTest : public testing::TestWithParam<std::tuple<string_view, bool, string_view>>{};
TEST_P(LowerBoundToStringTest, ToString)
{
    const auto& p = GetParam();
    const auto& version_a_parm = std::get<0>(p);
    const auto& inclusive_parm = std::get<1>(p);
    const auto& result_parm = std::get<2>(p);

    LowerBound<AlphanumericVersion> l_bound{version_a_parm, inclusive_parm};

    const auto bound_str = std::string{l_bound};
    ASSERT_EQ(result_parm, bound_str);
}
INSTANTIATE_TEST_SUITE_P(ToString, LowerBoundToStringTest,
                         testing::Values(
                             std::make_tuple("", true, ""),
                             std::make_tuple("", false, ">"),
                             std::make_tuple("", false, ">"),
                             std::make_tuple("0", false, ">0"),
                             std::make_tuple("0", true, "0+"),
                             std::make_tuple("foo", false, ">foo"),
                             std::make_tuple("foo", true, "foo+")
                         ));


class LowerBoundContainsTest : public testing::TestWithParam<std::tuple<string_view, bool, string_view, bool>>{};
TEST_P(LowerBoundContainsTest, ContainsVersion)
{
    const auto& p = GetParam();
    const auto& version_a_parm = std::get<0>(p);
    const auto& inclusive_parm = std::get<1>(p);
    const auto& version_b_parm = std::get<2>(p);

    AlphanumericVersion version_b = Factory<AlphanumericVersion>::Create(version_b_parm);
    LowerBound<AlphanumericVersion> l_bound{version_a_parm, inclusive_parm};

    const auto& equals_parm = std::get<3>(p);
    ASSERT_EQ(l_bound.contains_version(version_b), equals_parm);
}
INSTANTIATE_TEST_SUITE_P(ContainsVersion, LowerBoundContainsTest,
                         testing::Values(
                             std::make_tuple("0.0.1", /* inclusive */ false, "0.0.1", /* contains version */ false),
                             std::make_tuple("0.0.1", /* inclusive */ true, "0.0.1", /* contains version */ true),
                             std::make_tuple("0.0.1", /* inclusive */ false, "0.0.2", /* contains version */ true),
                             std::make_tuple("0.0.2", /* inclusive */ false, "0.0.1", /* contains version */ false),
                             std::make_tuple("0.0.2", /* inclusive */ true, "0.0.1", /* contains version */ false)
                         ));

//
// Upper Bound
//
class UpperBoundToStringTest : public testing::TestWithParam<std::tuple<string_view, bool, string_view>>{};
TEST_P(UpperBoundToStringTest, ToString)
{
    const auto& p = GetParam();
    const auto& version_a_parm = std::get<0>(p);
    const auto& inclusive_parm = std::get<1>(p);
    const auto& result_parm = std::get<2>(p);

    UpperBound<AlphanumericVersion> bound{version_a_parm, inclusive_parm};
    const auto bound_str = std::string{bound};
    ASSERT_EQ(bound_str, result_parm);
}
INSTANTIATE_TEST_SUITE_P(ToString, UpperBoundToStringTest,
                         testing::Values(
                             std::make_tuple("0", false, "<0"),
                             std::make_tuple("0.1.2", false, "<0.1.2")
                         ));

class UpperBoundContainsTest : public testing::TestWithParam<std::tuple<string_view, string_view, bool>>{};
TEST_P(UpperBoundContainsTest, ContainsVersion)
{
    const auto& p = GetParam();
    const auto& version_a_parm = std::get<0>(p);
    const auto& version_b_parm = std::get<1>(p);

    AlphanumericVersion version_b = Factory<AlphanumericVersion>::Create(version_b_parm);
    UpperBound<AlphanumericVersion> bound{version_a_parm, true};

    ASSERT_EQ(bound.contains_version(version_b), std::get<2>(p));
}
INSTANTIATE_TEST_SUITE_P(ContainsVersion, UpperBoundContainsTest,
                         testing::Values(
                             std::make_tuple("0.0.1", "0.0.1", true), // inclusive
                             std::make_tuple("0.0.1", "0.0.10", false), // a < b - false
                             std::make_tuple("0.0.1", "0.0.2", false)
                         ));

//
// Bound
//
class BoundIntersectsTest : public testing::TestWithParam<std::tuple<string_view, string_view, string_view, string_view, bool>>{};
TEST_P(BoundIntersectsTest, ContainsVersionInclusive)
{
    const auto& p = GetParam();

    const auto& version_aa_parm = std::get<0>(p);
    const auto& version_ab_parm = std::get<1>(p);

    const auto& version_ba_parm = std::get<2>(p);
    const auto& version_bb_parm = std::get<3>(p);

    Bound<AlphanumericVersion> bound_a{version_aa_parm, false, version_ab_parm, false};
    Bound<AlphanumericVersion> bound_b{version_ba_parm, false, version_bb_parm, false};

    ASSERT_EQ(bound_a.intersects(bound_b), std::get<4>(p));
}
INSTANTIATE_TEST_SUITE_P(ContainsVersion, BoundIntersectsTest,
                         testing::Values(
                             std::make_tuple("0.0.1", "0.0.9", "0.0.2", "0.0.4", /* intersects */ true), // A intersects with B
                             std::make_tuple("0.0.2", "0.0.4", "0.0.1", "0.0.9", /* intersects */ true), // B intersects with A
                             std::make_tuple("0.0.1", "0.0.9", "0.0.2", "0.0.9", /* intersects */ true),
                             std::make_tuple("0.0.1", "0.0.3", "0.0.3", "0.0.5", /* intersects */ false), // inclusive is false
                             std::make_tuple("0.0.1", "0.0.3", "0.0.4", "0.0.5", /* intersects */ false)
                         ));
