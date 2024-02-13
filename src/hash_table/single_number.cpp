// https://leetcode.com/problems/single-number/
// ----------------------------------------------------------------------------------------------
// Benchmark                                                    Time             CPU   Iterations
// ----------------------------------------------------------------------------------------------
// hash_table_single_number_simple/iterations:1000          43943 ns        46875 ns         1000
// hash_table_single_number_accumulate/iterations:1000      40955 ns        31250 ns         1000
// hash_table_single_number_parallel/iterations:1000        16985 ns        0.000 ns         1000

#include <common.hpp>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <algorithm>
#include <numeric>
#include <thread>
#include <utility>
#include <vector>

namespace hash_table::single_number {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

int simple(const std::vector<int>& nums) noexcept
{
  auto ans = 0;
  for (const auto i : nums) {
    ans = ans ^ i;
  }
  return ans;
}

int accumulate(const std::vector<int>& nums) noexcept
{
  return std::accumulate(nums.cbegin(), nums.cend(), 0, [](int lhs, int rhs) noexcept -> int {
    return lhs ^ rhs;
  });
}

int parallel(const std::vector<int>& nums) noexcept
{
  static const auto threads = std::max(std::thread::hardware_concurrency(), 16u);
  return tbb::parallel_reduce(
    tbb::blocked_range(nums.cbegin(), nums.cend(), std::max(nums.size() / threads, 1uz)),
    0,
    [](const auto& range, int ans) noexcept {
      return std::accumulate(range.begin(), range.end(), ans, [](int lhs, int rhs) noexcept {
        return lhs ^ rhs;
      });
    },
    [](int lhs, int rhs) noexcept {
      return lhs ^ rhs;
    });
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
  static const auto nums = create(999'999, 99'999);
  return nums;
}

}  // namespace hash_table::single_number

#if ENABLE_TESTS

TEST_CASE("hash_table::single_number::simple")
{
  using namespace hash_table::single_number;
  REQUIRE(simple(create(1, 0)) == 0);
  REQUIRE(simple(create(3, 1)) == 1);
  REQUIRE(simple(create(5, 2)) == 2);
  REQUIRE(simple(create(6, 2)) == 2);
  REQUIRE(simple(hash_table::single_number::benchmark()) == 99'999);
}

TEST_CASE("hash_table::single_number::accumulate")
{
  using namespace hash_table::single_number;
  REQUIRE(accumulate(create(1, 0)) == 0);
  REQUIRE(accumulate(create(3, 1)) == 1);
  REQUIRE(accumulate(create(5, 2)) == 2);
  REQUIRE(accumulate(create(6, 2)) == 2);
  REQUIRE(accumulate(hash_table::single_number::benchmark()) == 99'999);
}

TEST_CASE("hash_table::single_number::parallel")
{
  using namespace hash_table::single_number;
  REQUIRE(parallel(create(1, 0)) == 0);
  REQUIRE(parallel(create(3, 1)) == 1);
  REQUIRE(parallel(create(5, 2)) == 2);
  REQUIRE(parallel(create(6, 2)) == 2);
  REQUIRE(parallel(hash_table::single_number::benchmark()) == 99'999);
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

#define BENCHMARK_ITERATIONS 1'000

static void hash_table_single_number_simple(benchmark::State& state)
{
  const auto nums = hash_table::single_number::benchmark();
  for (auto _ : state) {
    auto result = hash_table::single_number::simple(nums);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_single_number_simple)->Iterations(BENCHMARK_ITERATIONS);

static void hash_table_single_number_accumulate(benchmark::State& state)
{
  const auto nums = hash_table::single_number::benchmark();
  for (auto _ : state) {
    auto result = hash_table::single_number::accumulate(nums);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_single_number_accumulate)->Iterations(BENCHMARK_ITERATIONS);

static void hash_table_single_number_parallel(benchmark::State& state)
{
  const auto nums = hash_table::single_number::benchmark();
  for (auto _ : state) {
    auto result = hash_table::single_number::parallel(nums);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_single_number_parallel)->Iterations(BENCHMARK_ITERATIONS);

#endif  // ENABLE_BENCHMARKS
