// https://leetcode.com/problems/search-in-rotated-sorted-array/
// ---------------------------------------------------------------------------------------------------------------------
// Benchmark                                                                           Time             CPU   Iterations
// ---------------------------------------------------------------------------------------------------------------------
// binary_search_search_in_rotated_sorted_array_slow/5000/900/-1/899                 126 ns          126 ns      4977778
// binary_search_search_in_rotated_sorted_array_slow/5000/900/-1/900                 676 ns          670 ns      1120000
// binary_search_search_in_rotated_sorted_array_slow/5000/900/2000/899               528 ns          547 ns      1000000
// binary_search_search_in_rotated_sorted_array_slow/5000/900/2000/900               675 ns          670 ns      1120000
// binary_search_search_in_rotated_sorted_array_slow/5000/2900/2000/2899             126 ns          126 ns      4977778
// binary_search_search_in_rotated_sorted_array_slow/5000/2900/2000/2900             683 ns          663 ns       896000
// binary_search_search_in_rotated_sorted_array_simple/5000/900/-1/899              9.52 ns         9.63 ns     74666667
// binary_search_search_in_rotated_sorted_array_simple/5000/900/-1/900              10.3 ns         10.5 ns     64000000
// binary_search_search_in_rotated_sorted_array_simple/5000/900/2000/899            10.9 ns         11.0 ns     64000000
// binary_search_search_in_rotated_sorted_array_simple/5000/900/2000/900            11.4 ns         11.2 ns     56000000
// binary_search_search_in_rotated_sorted_array_simple/5000/2900/2000/2899          9.15 ns         9.07 ns     89600000
// binary_search_search_in_rotated_sorted_array_simple/5000/2900/2000/2900          10.1 ns         10.0 ns     74666667

#include <common.hpp>
#include <algorithm>
#include <vector>

namespace binary_search::search_in_rotated_sorted_array {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

int slow(const std::vector<int>& nums, int target)
{
  if (const auto it = std::find(nums.cbegin(), nums.cend(), target); it != nums.cend()) {
    return std::distance(nums.cbegin(), it);
  }
  return -1;
}

int simple(const std::vector<int>& nums, int target)
{
  const auto data = nums.data();
  auto lo = data;
  auto hi = lo + nums.size() - 1;
  while (lo <= hi) {
    const auto mi = lo + (hi - lo) / 2;
    const auto mv = *mi;
    if (mv == target) {
      return static_cast<int>(mi - data);
    }
    if (*lo <= mv) {
      if (*lo <= target && target < mv) {
        hi = mi - 1;
      } else {
        lo = mi + 1;
      }
    } else {
      if (mv < target && target <= *hi) {
        lo = mi + 1;
      } else {
        hi = mi - 1;
      }
    }
  }
  return -1;
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

}  // namespace binary_search::search_in_rotated_sorted_array

#if ENABLE_TESTS

TEST_CASE("binary_search::search_in_rotated_sorted_array::slow")
{
  const auto n0 = binary_search::search_in_rotated_sorted_array::create(7, 3);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n0, -1) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n0, 0) == 0);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n0, 2) == 2);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n0, 3) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n0, 4) == 3);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n0, 8) == -1);

  const auto n3 = binary_search::search_in_rotated_sorted_array::create(7, 3, 3);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n3, -1) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n3, 0) == 4);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n3, 2) == 6);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n3, 3) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n3, 4) == 0);
  REQUIRE(binary_search::search_in_rotated_sorted_array::slow(n3, 8) == -1);
}

TEST_CASE("binary_search::search_in_rotated_sorted_array::simple")
{
  const auto n0 = binary_search::search_in_rotated_sorted_array::create(7, 3);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n0, -1) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n0, 0) == 0);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n0, 2) == 2);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n0, 3) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n0, 4) == 3);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n0, 8) == -1);

  const auto n3 = binary_search::search_in_rotated_sorted_array::create(7, 3, 3);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n3, -1) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n3, 0) == 4);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n3, 2) == 6);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n3, 3) == -1);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n3, 4) == 0);
  REQUIRE(binary_search::search_in_rotated_sorted_array::simple(n3, 8) == -1);
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void binary_search_search_in_rotated_sorted_array_slow(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto skip = state.range(1);
  const auto rotate = state.range(2);
  const auto target = state.range(3);
  const auto nums = binary_search::search_in_rotated_sorted_array::create(size, skip, rotate);
  for (auto _ : state) {
    auto result = binary_search::search_in_rotated_sorted_array::slow(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(binary_search_search_in_rotated_sorted_array_slow)
  ->Args({ 5'000, 900, -1, 899 })
  ->Args({ 5'000, 900, -1, 900 })
  ->Args({ 5'000, 900, 2'000, 899 })
  ->Args({ 5'000, 900, 2'000, 900 })
  ->Args({ 5'000, 2'900, 2'000, 2'899 })
  ->Args({ 5'000, 2'900, 2'000, 2'900 });
// clang-format on

static void binary_search_search_in_rotated_sorted_array_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto skip = state.range(1);
  const auto rotate = state.range(2);
  const auto target = state.range(3);
  const auto nums = binary_search::search_in_rotated_sorted_array::create(size, skip, rotate);
  for (auto _ : state) {
    auto result = binary_search::search_in_rotated_sorted_array::simple(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(binary_search_search_in_rotated_sorted_array_simple)
  ->Args({ 5'000, 900, -1, 899 })
  ->Args({ 5'000, 900, -1, 900 })
  ->Args({ 5'000, 900, 2'000, 899 })
  ->Args({ 5'000, 900, 2'000, 900 })
  ->Args({ 5'000, 2'900, 2'000, 2'899 })
  ->Args({ 5'000, 2'900, 2'000, 2'900 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
