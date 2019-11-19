#include <benchmark/benchmark.h>

#include "../version/token.hpp"

static void BM_NumericTokenInitialization(benchmark::State& state)
{
    const std::string version{"10238192"};
    for (auto _ : state)
    {
        rez::NumericToken t{version};
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(BM_NumericTokenInitialization);

static void BM_AlphanumericInitialization(benchmark::State& state)
{
    const std::string version{"0_hello"};
    for (auto _ : state)
    {
        rez::AlphanumericToken t{version};
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(BM_AlphanumericInitialization);

static void BM_DeepCopyInitialization(benchmark::State& state)
{
    const std::string version{"hello_12_world_23_foo_45_bar_67"};
    for (auto _ : state)
    {
        rez::AlphanumericToken t{version, true};
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(BM_DeepCopyInitialization);

static void BM_ShallowCopyInitialization(benchmark::State& state)
{
    const std::string version{"hello_12_world_23_foo_45_bar_67"};
    for (auto _ : state)
    {
        rez::AlphanumericToken t{version, false};
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(BM_ShallowCopyInitialization);

static void BM_DeepCopyComparison(benchmark::State& state)
{
    const rez::AlphanumericToken tA{"hello_12_world_23_foo_45_bar_67", true};
    const rez::AlphanumericToken tB{"hello_12_world_23_foo_45_bar_68", true};

    for (auto _ : state)
    {
        bool r  = tA < tB;
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_DeepCopyComparison);

static void BM_ShallowCopyComparison(benchmark::State& state)
{
    const rez::AlphanumericToken tA{"hello_12_world_23_foo_45_bar_67", false};
    const rez::AlphanumericToken tB{"hello_12_world_23_foo_45_bar_68", false};

    for (auto _ : state)
    {
        bool r  = tA < tB;
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_ShallowCopyComparison);

// Run the benchmark
BENCHMARK_MAIN();