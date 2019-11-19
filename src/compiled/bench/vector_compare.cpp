#include <benchmark/benchmark.h>

#include <iostream>
#include <vector>

static void BM_compare_less(benchmark::State& state)
{
    std::vector<int> a{10, 20, 30, 40, 50, 60, 70};
    std::vector<int> b{10, 20, 30, 40, 50, 60, 80};

    for (auto _ : state)
    {
        auto r = a < b;
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_compare_less);

static void BM_compare_greater_equal(benchmark::State& state)
{
    std::vector<int> a{10, 20, 30, 40, 50, 60, 70};
    std::vector<int> b{10, 20, 30, 40, 50, 60, 80};

    for (auto _ : state)
    {
        auto r = !(a >= b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_compare_greater_equal);
