// https://leetcode.com/problems/two-sum/
// ---------------------------------------------------------------------------------------------------
// Benchmark                                                         Time             CPU   Iterations
// ---------------------------------------------------------------------------------------------------
// hash_table_two_sum_simple/100000/5000                     805010200 ns    765625000 ns            1
// hash_table_two_sum_unordered_map/100000/5000                4235409 ns      4141566 ns          166
// hash_table_two_sum_flat_map/100000/5000                     1306154 ns      1227679 ns          560
// hash_table_two_sum_unordered_flat_map/100000/5000           1058109 ns      1049805 ns          640
// hash_table_two_sum_flat_multimap/100000/5000                1187615 ns      1147461 ns          640
// hash_table_two_sum_flat_multimap_lower_bound/100000/5000    1438727 ns      1395089 ns          448

#include <common.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hash_table::two_sum {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

std::vector<int> simple(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  const auto size = static_cast<int>(nums.size());
  for (auto i = 0; i < size; i++) {
    for (auto j = i + 1; j < size; j++) {
      if (data[i] + data[j] == target) {
        return { i, j };
      }
    }
  }
  std::unreachable();
  return {};
}

std::vector<int> unordered_map(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  const auto size = static_cast<int>(nums.size());
  std::unordered_map<int, int> table;
  table.reserve(static_cast<std::size_t>(size));
  table.emplace(nums[0], 0);
  for (auto i = 1; i < size; i++) {
    const auto v = data[i];
    const auto d = target - v;
    if (const auto it = table.find(d); it != table.end()) {
      return { it->second, i };
    }
    table.emplace(v, i);
  }
  std::unreachable();
  return {};
}

std::vector<int> flat_map(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  const auto size = static_cast<int>(nums.size());
  boost::container::flat_map<int, int> table;
  table.reserve(static_cast<std::size_t>(size));
  table.emplace(nums[0], 0);
  for (auto i = 1; i < size; i++) {
    const auto v = data[i];
    const auto d = target - v;
    if (const auto it = table.find(d); it != table.end()) {
      return { it->second, i };
    }
    table.emplace(v, i);
  }
  std::unreachable();
  return {};
}

std::vector<int> unordered_flat_map(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  const auto size = static_cast<int>(nums.size());
  boost::unordered_flat_map<int, int> table;
  table.reserve(static_cast<std::size_t>(size));
  table.emplace(nums[0], 0);
  for (auto i = 1; i < size; i++) {
    const auto v = data[i];
    const auto d = target - v;
    if (const auto it = table.find(d); it != table.end()) {
      return { it->second, i };
    }
    table.emplace(v, i);
  }
  std::unreachable();
  return {};
}

std::vector<int> flat_multimap(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  const auto size = static_cast<int>(nums.size());
  boost::container::flat_multimap<int, int> table;
  table.reserve(static_cast<std::size_t>(size));
  for (auto i = 0; i < size; i++) {
    table.emplace(data[i], i);
  }
  const auto end = table.cend();
  for (auto one = table.cbegin(); one != end; ++one) {
    const auto d = target - one->first;
    auto two = table.find(d);
    if (two == end) {
      continue;
    }
    if (two == one && (++two == end || two->first != d)) {
      continue;
    }
    return { one->second, two->second };
  }
  std::unreachable();
  return {};
}

std::vector<int> flat_multimap_lower_bound(const std::vector<int>& nums, int target) noexcept
{
  const auto data = nums.data();
  const auto size = static_cast<int>(nums.size());
  boost::container::flat_multimap<int, int> table;
  table.reserve(static_cast<std::size_t>(size));
  for (auto i = 0; i < size; i++) {
    table.emplace(data[i], i);
  }
  const auto end = table.cend();
  for (auto one = table.cbegin(); one != end; ++one) {
    const auto d = target - one->first;
    const auto two = std::lower_bound(std::next(one), end, d, [](const auto& it, int i) noexcept {
      return it.first < i;
    });
    if (two == end) {
      continue;
    }
    if (two->first == d) {
      return { one->second, two->second };
    }
  }
  std::unreachable();
  return {};
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

std::vector<int> benchmark(std::size_t size, int target)
{
  std::vector<int> nums(size);
  auto index = target + 1;
  for (auto& n : nums) {
    n = index++;
  }
  switch (size) {
  case 0:
    break;
  case 1:
    nums[0] = target;
    break;
  case 2:
    nums[0] = target / 2;
    nums[1] = target - target / 2;
    break;
  default:
    nums[size / 2] = target / 2;
    nums[size / 2 + size / 3] = target - target / 2;
    break;
  }
  return nums;
}

}  // namespace hash_table::two_sum

#if ENABLE_TESTS || 1

TEST_CASE("hash_table::two_sum::simple")
{
  using namespace hash_table::two_sum;
  REQUIRE(simple({ 2, 7, 11, 15 }, 9) == std::vector{ 0, 1 });
  REQUIRE(simple({ 3, 2, 4 }, 6) == std::vector{ 1, 2 });
  REQUIRE(simple({ 3, 3 }, 6) == std::vector{ 0, 1 });
  REQUIRE(simple(hash_table::two_sum::benchmark(5, 6), 6) == std::vector{ 2, 3 });
}

TEST_CASE("hash_table::two_sum::unordered_map")
{
  using namespace hash_table::two_sum;
  REQUIRE(unordered_map({ 2, 7, 11, 15 }, 9) == std::vector{ 0, 1 });
  REQUIRE(unordered_map({ 3, 2, 4 }, 6) == std::vector{ 1, 2 });
  REQUIRE(unordered_map({ 3, 3 }, 6) == std::vector{ 0, 1 });
  REQUIRE(unordered_map(hash_table::two_sum::benchmark(5, 6), 6) == std::vector{ 2, 3 });
}

TEST_CASE("hash_table::two_sum::flat_map")
{
  using namespace hash_table::two_sum;
  REQUIRE(flat_map({ 2, 7, 11, 15 }, 9) == std::vector{ 0, 1 });
  REQUIRE(flat_map({ 3, 2, 4 }, 6) == std::vector{ 1, 2 });
  REQUIRE(flat_map({ 3, 3 }, 6) == std::vector{ 0, 1 });
  REQUIRE(flat_map(hash_table::two_sum::benchmark(5, 6), 6) == std::vector{ 2, 3 });
}

TEST_CASE("hash_table::two_sum::unordered_flat_map")
{
  using namespace hash_table::two_sum;
  REQUIRE(unordered_flat_map({ 2, 7, 11, 15 }, 9) == std::vector{ 0, 1 });
  REQUIRE(unordered_flat_map({ 3, 2, 4 }, 6) == std::vector{ 1, 2 });
  REQUIRE(unordered_flat_map({ 3, 3 }, 6) == std::vector{ 0, 1 });
  REQUIRE(unordered_flat_map(hash_table::two_sum::benchmark(5, 6), 6) == std::vector{ 2, 3 });
}

TEST_CASE("hash_table::two_sum::flat_multimap")
{
  using namespace hash_table::two_sum;
  REQUIRE(flat_multimap({ 2, 7, 11, 15 }, 9) == std::vector{ 0, 1 });
  REQUIRE(flat_multimap({ 3, 2, 4 }, 6) == std::vector{ 1, 2 });
  REQUIRE(flat_multimap({ 3, 3 }, 6) == std::vector{ 0, 1 });
  REQUIRE(flat_multimap(hash_table::two_sum::benchmark(5, 6), 6) == std::vector{ 2, 3 });
}

TEST_CASE("hash_table::two_sum::flat_multimap_lower_bound")
{
  using namespace hash_table::two_sum;
  REQUIRE(flat_multimap_lower_bound({ 2, 7, 11, 15 }, 9) == std::vector{ 0, 1 });
  REQUIRE(flat_multimap_lower_bound({ 3, 2, 4 }, 6) == std::vector{ 1, 2 });
  REQUIRE(flat_multimap_lower_bound({ 3, 3 }, 6) == std::vector{ 0, 1 });
  REQUIRE(flat_multimap_lower_bound(hash_table::two_sum::benchmark(5, 6), 6) == std::vector{ 2, 3 });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS || 1

static void hash_table_two_sum_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  const auto nums = hash_table::two_sum::benchmark(size, target);
  for (auto _ : state) {
    auto result = hash_table::two_sum::simple(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_two_sum_simple)->Args({ 100'000, 5'000 });

static void hash_table_two_sum_unordered_map(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  const auto nums = hash_table::two_sum::benchmark(size, target);
  for (auto _ : state) {
    auto result = hash_table::two_sum::unordered_map(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_two_sum_unordered_map)->Args({ 100'000, 5'000 });

static void hash_table_two_sum_flat_map(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  const auto nums = hash_table::two_sum::benchmark(size, target);
  for (auto _ : state) {
    auto result = hash_table::two_sum::flat_map(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_two_sum_flat_map)->Args({ 100'000, 5'000 });

static void hash_table_two_sum_unordered_flat_map(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  const auto nums = hash_table::two_sum::benchmark(size, target);
  for (auto _ : state) {
    auto result = hash_table::two_sum::unordered_flat_map(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_two_sum_unordered_flat_map)->Args({ 100'000, 5'000 });

static void hash_table_two_sum_flat_multimap(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  const auto nums = hash_table::two_sum::benchmark(size, target);
  for (auto _ : state) {
    auto result = hash_table::two_sum::flat_multimap(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_two_sum_flat_multimap)->Args({ 100'000, 5'000 });

static void hash_table_two_sum_flat_multimap_lower_bound(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto target = state.range(1);
  const auto nums = hash_table::two_sum::benchmark(size, target);
  for (auto _ : state) {
    auto result = hash_table::two_sum::flat_multimap_lower_bound(nums, target);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(hash_table_two_sum_flat_multimap_lower_bound)->Args({ 100'000, 5'000 });

#endif  // ENABLE_BENCHMARKS
