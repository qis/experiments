// https://leetcode.com/problems/search-in-rotated-sorted-array-ii/
// ---------------------------------------------------------------------------------------------------------------------
// Benchmark                                                                           Time             CPU   Iterations
// ---------------------------------------------------------------------------------------------------------------------
// binary_search_search_in_rotated_sorted_array_ii_slow/5000/900/-1/899              126 ns          122 ns      4480000
// binary_search_search_in_rotated_sorted_array_ii_slow/5000/900/-1/900              682 ns          670 ns      1120000
// binary_search_search_in_rotated_sorted_array_ii_slow/5000/900/2000/899            526 ns          547 ns      1000000
// binary_search_search_in_rotated_sorted_array_ii_slow/5000/900/2000/900            676 ns          672 ns      1000000
// binary_search_search_in_rotated_sorted_array_ii_slow/5000/2900/2000/2899          127 ns          126 ns      5600000
// binary_search_search_in_rotated_sorted_array_ii_slow/5000/2900/2000/2900          677 ns          688 ns      1000000
// binary_search_search_in_rotated_sorted_array_ii_simple/5000/900/-1/899           9.38 ns         9.42 ns     74666667
// binary_search_search_in_rotated_sorted_array_ii_simple/5000/900/-1/900           11.5 ns         11.7 ns     64000000
// binary_search_search_in_rotated_sorted_array_ii_simple/5000/900/2000/899         9.85 ns         9.84 ns     74666667
// binary_search_search_in_rotated_sorted_array_ii_simple/5000/900/2000/900         14.0 ns         13.8 ns     49777778
// binary_search_search_in_rotated_sorted_array_ii_simple/5000/2900/2000/2899       10.1 ns         10.3 ns     64000000
// binary_search_search_in_rotated_sorted_array_ii_simple/5000/2900/2000/2900       11.4 ns         11.5 ns     64000000

#include <common.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

namespace binary_search::search_in_rotated_sorted_array_ii {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

bool slow(const std::vector<int>& nums, int target) noexcept
{
  return std::find(nums.cbegin(), nums.cend(), target) != nums.cend();
}

bool simple(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  auto li = data;
  auto hi = li + nums.size() - 1;
  while (li <= hi) {
    const auto mi = li + (hi - li) / 2;
    const auto mv = *mi;
    if (mv == target) {
      return true;
    }
    if (mv == *li && *li == *hi) {
      ++li;
      --hi;
      continue;
    }
    const auto lv = *li;
    if (mv >= lv) {
      if (mv >= target && lv <= target) {
        hi = mi - 1;
      } else {
        li = mi + 1;
      }
      continue;
    }
    const auto hv = *hi;
    if (mv <= target && hv >= target) {
      li = mi + 1;
    } else {
      hi = mi - 1;
    }
  }
  return false;
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
    n = v == skip ? v - 1 : v;
  }
  if (rotate > 0) {
    std::rotate(nums.begin(), nums.begin() + rotate, nums.end());
  }
  return nums;
}

}  // namespace binary_search::search_in_rotated_sorted_array_ii

#if ENABLE_TESTS

TEST_CASE("binary_search::search_in_rotated_sorted_array_ii::slow")
{
  const auto n0 = binary_search::search_in_rotated_sorted_array_ii::create(7, 3);
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::slow(n0, -1));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::slow(n0, 0));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::slow(n0, 2));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::slow(n0, 3));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::slow(n0, 4));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::slow(n0, 8));

  const auto n3 = binary_search::search_in_rotated_sorted_array_ii::create(7, 3, 3);
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::slow(n3, -1));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::slow(n3, 0));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::slow(n3, 2));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::slow(n3, 3));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::slow(n3, 4));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::slow(n3, 8));
}

TEST_CASE("binary_search::search_in_rotated_sorted_array_ii::simple")
{
  const auto n0 = binary_search::search_in_rotated_sorted_array_ii::create(7, 3);
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::simple(n0, -1));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::simple(n0, 0));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::simple(n0, 2));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::simple(n0, 3));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::simple(n0, 4));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::simple(n0, 8));

  const auto n3 = binary_search::search_in_rotated_sorted_array_ii::create(7, 3, 3);
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::simple(n3, -1));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::simple(n3, 0));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::simple(n3, 2));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::simple(n3, 3));
  REQUIRE(binary_search::search_in_rotated_sorted_array_ii::simple(n3, 4));
  REQUIRE(!binary_search::search_in_rotated_sorted_array_ii::simple(n3, 8));
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void binary_search_search_in_rotated_sorted_array_ii_slow(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto skip = state.range(1);
  const auto rotate = state.range(2);
  const auto target = state.range(3);
  const auto nums = binary_search::search_in_rotated_sorted_array_ii::create(size, skip, rotate);
  for (auto _ : state) {
    auto result = binary_search::search_in_rotated_sorted_array_ii::slow(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(binary_search_search_in_rotated_sorted_array_ii_slow)
  ->Args({ 5'000, 900, -1, 899 })
  ->Args({ 5'000, 900, -1, 900 })
  ->Args({ 5'000, 900, 2'000, 899 })
  ->Args({ 5'000, 900, 2'000, 900 })
  ->Args({ 5'000, 2'900, 2'000, 2'899 })
  ->Args({ 5'000, 2'900, 2'000, 2'900 });
// clang-format on

static void binary_search_search_in_rotated_sorted_array_ii_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto skip = state.range(1);
  const auto rotate = state.range(2);
  const auto target = state.range(3);
  const auto nums = binary_search::search_in_rotated_sorted_array_ii::create(size, skip, rotate);
  for (auto _ : state) {
    auto result = binary_search::search_in_rotated_sorted_array_ii::simple(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(binary_search_search_in_rotated_sorted_array_ii_simple)
  ->Args({ 5'000, 900, -1, 899 })
  ->Args({ 5'000, 900, -1, 900 })
  ->Args({ 5'000, 900, 2'000, 899 })
  ->Args({ 5'000, 900, 2'000, 900 })
  ->Args({ 5'000, 2'900, 2'000, 2'899 })
  ->Args({ 5'000, 2'900, 2'000, 2'900 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
