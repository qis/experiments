// https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/
// ---------------------------------------------------------------------------------------------------------------------
// Benchmark                                                                           Time             CPU   Iterations
// ---------------------------------------------------------------------------------------------------------------------
// binary_search_find_minimum_in_rotated_sorted_array_slow/5000/900/-1              2108 ns         2176 ns       373333
// binary_search_find_minimum_in_rotated_sorted_array_slow/5000/0/-1                2119 ns         2093 ns       298667
// binary_search_find_minimum_in_rotated_sorted_array_slow/5000/900/2000            2107 ns         2100 ns       320000
// binary_search_find_minimum_in_rotated_sorted_array_slow/5000/0/2000              2114 ns         2051 ns       373333
// binary_search_find_minimum_in_rotated_sorted_array_simple/5000/900/-1            8.91 ns         8.79 ns     74666667
// binary_search_find_minimum_in_rotated_sorted_array_simple/5000/0/-1              8.93 ns         8.79 ns     74666667
// binary_search_find_minimum_in_rotated_sorted_array_simple/5000/900/2000          9.39 ns         9.21 ns     74666667
// binary_search_find_minimum_in_rotated_sorted_array_simple/5000/0/2000            9.06 ns         9.00 ns     74666667

#include <common.hpp>
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

namespace binary_search::find_minimum_in_rotated_sorted_array {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

int slow(const std::vector<int>& nums) noexcept
{
  return *std::min_element(nums.cbegin(), nums.cend());
}

int simple(const std::vector<int>& nums) noexcept
{
  const auto data = nums.data();
  auto lo = data;
  auto hi = lo + nums.size() - 1;
  auto rv = std::numeric_limits<int>::max();
  while (lo <= hi) {
    const auto mi = lo + (hi - lo) / 2;
    const auto mv = *mi;
    const auto lv = *lo;
    if (lv <= mv) {
      rv = std::min(rv, lv);
      lo = mi + 1;
    } else {
      rv = std::min(rv, mv);
      hi = mi - 1;
    }
  }
  return rv;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

std::vector<int> create(std::size_t size, int skip, int rotate = -1)
{
  std::vector<int> nums(size);
  auto value = 0;
  for (auto& n : nums) {
    const auto v = value++;
    n = v == skip ? value++ : v;
  }
  if (rotate > 0) {
    std::rotate(nums.begin(), nums.begin() + rotate, nums.end());
  }
  return nums;
}

}  // namespace binary_search::find_minimum_in_rotated_sorted_array

#if ENABLE_TESTS

TEST_CASE("binary_search::find_minimum_in_rotated_sorted_array::slow")
{
  const auto n0 = binary_search::find_minimum_in_rotated_sorted_array::create(7, 3);
  REQUIRE(binary_search::find_minimum_in_rotated_sorted_array::slow(n0) == 0);

  const auto n3 = binary_search::find_minimum_in_rotated_sorted_array::create(7, 0, 3);
  REQUIRE(binary_search::find_minimum_in_rotated_sorted_array::slow(n3) == 1);
}

TEST_CASE("binary_search::find_minimum_in_rotated_sorted_array::simple")
{
  const auto n0 = binary_search::find_minimum_in_rotated_sorted_array::create(7, 3);
  REQUIRE(binary_search::find_minimum_in_rotated_sorted_array::simple(n0) == 0);

  const auto n3 = binary_search::find_minimum_in_rotated_sorted_array::create(7, 0, 3);
  REQUIRE(binary_search::find_minimum_in_rotated_sorted_array::simple(n3) == 1);
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void binary_search_find_minimum_in_rotated_sorted_array_slow(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto skip = state.range(1);
  const auto rotate = state.range(2);
  const auto nums = binary_search::find_minimum_in_rotated_sorted_array::create(size, skip, rotate);
  for (auto _ : state) {
    auto result = binary_search::find_minimum_in_rotated_sorted_array::slow(nums);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(binary_search_find_minimum_in_rotated_sorted_array_slow)
  ->Args({ 5'000, 900, -1 })
  ->Args({ 5'000, 0, -1 })
  ->Args({ 5'000, 900, 2'000 })
  ->Args({ 5'000, 0, 2'000 });
// clang-format on

static void binary_search_find_minimum_in_rotated_sorted_array_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto skip = state.range(1);
  const auto rotate = state.range(2);
  const auto nums = binary_search::find_minimum_in_rotated_sorted_array::create(size, skip, rotate);
  for (auto _ : state) {
    auto result = binary_search::find_minimum_in_rotated_sorted_array::simple(nums);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(binary_search_find_minimum_in_rotated_sorted_array_simple)
  ->Args({ 5'000, 900, -1 })
  ->Args({ 5'000, 0, -1 })
  ->Args({ 5'000, 900, 2'000 })
  ->Args({ 5'000, 0, 2'000 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
