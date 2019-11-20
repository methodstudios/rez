#include <gtest/gtest.h>
#include <fmt/format.h>

#include "../version/value.hpp"
#include "../version/token.hpp"

using namespace rez;

TEST(ValueArray, Methods)
{
    ValueArray<int> int_arr;
    int_arr.Append(10);
    int_arr.Append(200);

    EXPECT_EQ(int_arr[0], 10);
    EXPECT_EQ(int_arr[1], 200);

//
//    std::cout << tok.Size() << std::endl;
}