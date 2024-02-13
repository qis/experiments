// https://leetcode.com/problems/single-number/ in O(1) memory
#include <common.hpp>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

namespace hash_table::single_number {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

int sort(std::vector<int> nums) noexcept
{
  std::sort(nums.begin(), nums.end());
  auto it = nums.data();
  const auto end = it + nums.size();
  auto last = *it;
  if (++it == end || *it != last) {
    return last;
  }
  for (++it; it != end; ++it) {
    const auto v = *it;
    if (v == last) {
      continue;
    }
    last = v;
    if (++it == end || *it != v) {
      return v;
    }
  }
  std::unreachable();
  return -1;
}

int fast(const std::vector<int>& nums) noexcept
{
  auto ans = 0;
  for (const auto i : nums) {
    ans = ans ^ i;
  }
  return ans;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

std::vector<int> create(std::size_t size, int unique)
{
  std::vector<int> nums;
  nums.reserve(size);
  for (auto i = 0; i < unique; i++) {
    nums.push_back(i);
    nums.push_back(i);
  }
  if (unique * 2 < size) {
    nums.push_back(unique);
  }
  auto value = static_cast<int>(nums.size() / 2) + 1;
  for (auto i = nums.size(); i < size; i += 2) {
    if (i + 1 == size) {
      nums.push_back(unique > 0 ? 0 : 1);
      break;
    }
    nums.push_back(value);
    nums.push_back(value);
    value++;
  }
  std::random_shuffle(nums.begin(), nums.end());
  return nums;
}

std::vector<int> benchmark()
{
  static const auto nums = create(10'000, 900);
  return nums;
}

}  // namespace hash_table::single_number

#if ENABLE_TESTS || 1

TEST_CASE("hash_table::single_number::sort")
{
  using namespace hash_table::single_number;
  REQUIRE(sort(create(1, 0)) == 0);
  REQUIRE(sort(create(3, 1)) == 1);
  REQUIRE(sort(create(5, 2)) == 2);
  REQUIRE(sort(create(6, 2)) == 2);
  REQUIRE(sort(hash_table::single_number::benchmark()) == 900);
}

TEST_CASE("hash_table::single_number::fast")
{
  using namespace hash_table::single_number;
  REQUIRE(fast(create(1, 0)) == 0);
  REQUIRE(fast(create(3, 1)) == 1);
  REQUIRE(fast(create(5, 2)) == 2);
  REQUIRE(fast(create(6, 2)) == 2);
  REQUIRE(fast(hash_table::single_number::benchmark()) == 900);
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS || 1

#define BENCHMARK_ITERATIONS 1'000

static void hash_table_single_number_sort(benchmark::State& state)
{
  const auto nums = hash_table::single_number::benchmark();
  std::vector<std::vector<int>> data;
  data.reserve(BENCHMARK_ITERATIONS);
  for (std::size_t i = 0, size = nums.size(); i < size; i++) {
    data.emplace_back(nums);
  }
  auto it = data.begin();
  for (auto _ : state) {
    auto result = hash_table::single_number::sort(*it);
    benchmark::DoNotOptimize(result);
    ++it;
  }
}

BENCHMARK(hash_table_single_number_sort)->Iterations(BENCHMARK_ITERATIONS);

static void hash_table_single_number_fast(benchmark::State& state)
{
  const auto nums = hash_table::single_number::benchmark();
  std::vector<std::vector<int>> data;
  data.reserve(BENCHMARK_ITERATIONS);
  for (std::size_t i = 0, size = nums.size(); i < size; i++) {
    data.emplace_back(nums);
  }
  auto it = data.begin();
  for (auto _ : state) {
    auto result = hash_table::single_number::fast(*it);
    benchmark::DoNotOptimize(result);
    ++it;
  }
}

BENCHMARK(hash_table_single_number_fast)->Iterations(BENCHMARK_ITERATIONS);

#endif  // ENABLE_BENCHMARKS
