// https://leetcode.com/problems/binary-search/
// -------------------------------------------------------------------------------------------
// Benchmark                                                 Time             CPU   Iterations
// -------------------------------------------------------------------------------------------
// algorithms_binary_search_simple/150000/100001          14.6 ns         14.4 ns     49777778
// algorithms_binary_search_simple/150000/100000          13.0 ns         13.2 ns     49777778
// algorithms_binary_search_optimized/150000/100001       14.8 ns         14.8 ns     49777778
// algorithms_binary_search_optimized/150000/100000       15.1 ns         14.6 ns     44800000
// algorithms_binary_search_standard/150000/100001        8.39 ns         8.54 ns     89600000
// algorithms_binary_search_standard/150000/100000        8.62 ns         8.79 ns     74666667

#include <common.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

namespace algorithms::binary_search {

int simple(std::vector<int>& nums, int target) noexcept
{
  const auto size = static_cast<int>(nums.size());
  if (size < 1) {
    return -1;
  }
  const auto data = nums.data();
  auto lhs = 0;
  auto rhs = size - 1;
  while (lhs <= rhs) {
    const auto m = (lhs + rhs) / 2;
    if (data[m] < target) {
      lhs = m + 1;
    } else if (data[m] > target) {
      rhs = m - 1;
    } else {
      return m;
    }
  }
  return -1;
}

int optimized(std::vector<int>& nums, int target) noexcept
{
  const auto size = static_cast<int>(nums.size());
  if (size < 1) {
    return -1;
  }
  const auto data = nums.data();
  auto lhs = 0;
  auto rhs = size - 1;
  while (lhs != rhs) {
    // NOTE: This should be one of the following:
    // - static_cast<int>(ceil((lhs + rhs) / 2.0))
    // - a test if `lhs + rhs` is 0
    const auto m = (lhs + rhs + 1) / 2;
    if (data[m] > target) {
      rhs = m - 1;
    } else {
      lhs = m;
    }
  }
  if (data[lhs] == target) {
    return lhs;
  }
  return -1;
}

int standard(std::vector<int>& nums, int target) noexcept
{
  if (const auto it = std::lower_bound(nums.begin(), nums.end(), target); it != nums.end() && *it == target) {
    return static_cast<int>(std::distance(nums.begin(), it));
  }
  return -1;
}

inline auto create(std::size_t size)
{
  std::vector<int> nums;
  nums.reserve(size);
  for (std::size_t i = 0; i < size; i++) {
    nums.push_back(static_cast<int>(i * 2));
  }
  return nums;
}

void test()
{
  auto nums = create(10);
  std::cout << simple(nums, 4) << std::endl;
}

}  // namespace algorithms::binary_search

#if ENABLE_TESTS

TEST_CASE("algorithms::binary_search::simple")
{
  auto nums = algorithms::binary_search::create(15);
  REQUIRE(algorithms::binary_search::simple(nums, 3) == -1);
  REQUIRE(algorithms::binary_search::simple(nums, 4) == 2);
  nums = algorithms::binary_search::create(2);
  REQUIRE(algorithms::binary_search::simple(nums, -1) == -1);
  REQUIRE(algorithms::binary_search::simple(nums, 2) == 1);
}

TEST_CASE("algorithms::binary_search::optimized")
{
  auto nums = algorithms::binary_search::create(15);
  REQUIRE(algorithms::binary_search::optimized(nums, 3) == -1);
  REQUIRE(algorithms::binary_search::optimized(nums, 4) == 2);
  nums = algorithms::binary_search::create(2);
  REQUIRE(algorithms::binary_search::simple(nums, -1) == -1);
  REQUIRE(algorithms::binary_search::simple(nums, 2) == 1);
}

TEST_CASE("algorithms::binary_search::standard")
{
  auto nums = algorithms::binary_search::create(15);
  REQUIRE(algorithms::binary_search::standard(nums, 3) == -1);
  REQUIRE(algorithms::binary_search::standard(nums, 4) == 2);
  nums = algorithms::binary_search::create(2);
  REQUIRE(algorithms::binary_search::simple(nums, -1) == -1);
  REQUIRE(algorithms::binary_search::simple(nums, 2) == 1);
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void algorithms_binary_search_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  auto nums = algorithms::binary_search::create(size);
  for (auto _ : state) {
    auto result = algorithms::binary_search::simple(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_binary_search_simple)
  ->Args({ 150'000, 100'001 })
  ->Args({ 150'000, 100'000 });
// clang-format on

static void algorithms_binary_search_optimized(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  auto nums = algorithms::binary_search::create(size);
  for (auto _ : state) {
    auto result = algorithms::binary_search::optimized(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_binary_search_optimized)
  ->Args({ 150'000, 100'001 })
  ->Args({ 150'000, 100'000 });
// clang-format on

static void algorithms_binary_search_standard(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  auto nums = algorithms::binary_search::create(size);
  for (auto _ : state) {
    auto result = algorithms::binary_search::standard(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_binary_search_standard)
  ->Args({ 150'000, 100'001 })
  ->Args({ 150'000, 100'000 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
