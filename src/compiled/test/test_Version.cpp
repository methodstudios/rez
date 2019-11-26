#include <gtest/gtest.h>

#include "../version/Version.hpp"

#include <memory>

TEST(Version, Initialization)
{
    auto a = Factory<VersionT<AlphanumericToken, false>>::Create("1.2.3");

    ASSERT_EQ(a->Size(), 3);

    ASSERT_EQ(a->Major(), "1"_at);
    ASSERT_EQ(a->Minor(), "2"_at);
    ASSERT_EQ(a->Patch(), "3"_at);

    ASSERT_EQ(a->seps[0], ".");
    ASSERT_EQ(a->seps[1], ".");
}
