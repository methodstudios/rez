#include <gtest/gtest.h>

#include "../version/Comparable.hpp"

// simple comparable type, TODO: provide more unit tests
using Data = bool;

TEST(Comparable, Comparision)
{

    auto a = Factory<Data>::Create<NOR_CMP>(false);
    auto b = Factory<Data>::Create<REV_CMP>(true);

    ASSERT_LT(a, b);
    ASSERT_LT(b, a);
}