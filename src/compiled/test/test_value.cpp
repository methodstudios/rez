#include <gtest/gtest.h>

#include "../version/NewVersion.hpp"

TEST(ValueArray, Methods)
{
    auto k = create_numeric_token("10");
    std::cout << k << std::endl;

    auto u = create_numeric_token<true>("20");
    std::cout << u << std::endl;

//    auto rk = get_reversed(k);
//    auto rj = get_reversed(j);
//    auto rrk = get_reversed(rk);
//
//    std::cout << (k < j) << std::endl;
//    std::cout << (rj < rk) << std::endl;

    // Value<_Type> must be type of storage, so if we store it in the Version, how can we make version reversible?
}