#include <gtest/gtest.h>

#include "../version/Comparable.hpp"

TEST(Comparable, IsComparableType)
{
    ASSERT_TRUE(is_comparable<Comparable<int>>::value );
    ASSERT_FALSE( is_comparable<int>::value);

    { // ASSERT TRUE is a macro and , is as an arg
        constexpr bool is_cmp = is_comparable<Comparable<int, NORMAL>>::value;
        ASSERT_TRUE(is_cmp);
    }

    { // ASSERT TRUE is a macro and , is as an arg
        constexpr bool is_cmp = is_comparable<Comparable<int, REVERSED>>::value;
        ASSERT_TRUE(is_cmp);
    }

    { // reverse sort key
        Comparable<int> comp_a;
        auto comp_b = reverse_sort_key(comp_a);

        ASSERT_TRUE(is_comparable<decltype(comp_a)>::value);
        ASSERT_TRUE(is_comparable<decltype(comp_b)>::value);
    }
}

TEST(Comparable, ReverseSortKey)
{
    Comparable<int, false> val{10};
    auto rev = reverse_sort_key(val);

    ASSERT_FALSE(is_reversed<decltype(val)>::value );
    ASSERT_EQ(*val, 10);

    ASSERT_TRUE(is_reversed<decltype(rev)>::value );
    ASSERT_EQ(*rev, 10);
}

TEST(Comparable, Comparision)
{
    auto a = Factory<Comparable<bool, NORMAL>>::Create(false);
    auto b = Factory<Comparable<bool, REVERSED>>::Create(true);

    ASSERT_LT(a, b);
    ASSERT_LT(b, a);

}

TEST(Comparable, SetGet)
{
    Comparable<int> val{0};
    ASSERT_EQ(*val, 0);

    *val = 10;
    ASSERT_EQ(*val, 10);
}

TEST(Comparable, CustomDataAccessor)
{
    struct Data
    {
        int value;
    };

    Comparable<Data> val{0};
    ASSERT_EQ(val->value, 0);

    val->value = 10;
    ASSERT_EQ(val->value, 10);
}

