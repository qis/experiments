// https://leetcode.com/problems/sliding-window-maximum/
// --------------------------------------------------------------------------------------------------------
// Benchmark                                                              Time             CPU   Iterations
// --------------------------------------------------------------------------------------------------------
// sliding_window_sliding_window_maximum_run<solution::simple>     10579791 ns     10323661 ns           56
// sliding_window_sliding_window_maximum_run<solution::multiset>     728197 ns       697545 ns          896
// sliding_window_sliding_window_maximum_run<solution::deque>         24609 ns        23996 ns        28000

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <algorithm>
#include <deque>
#include <set>
#include <vector>

namespace sliding_window::sliding_window_maximum {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  multiset,
  deque);
// clang-format on

template <solution Solution>
std::vector<int> run(const std::vector<int>& nums, int k)
{
  const auto data = nums.data();
  const auto size = nums.size();
  std::vector<int> result(size - k + 1);
  auto out = result.data();
  for (auto lhs = data, rhs = data + k, end = data + size; rhs <= end; ++lhs, ++rhs) {
    *out++ = *std::max_element(lhs, rhs);
  }
  return result;
}

template <>
std::vector<int> run<solution::multiset>(const std::vector<int>& nums, int k)
{
  const auto data = nums.data();
  const auto size = nums.size();
  std::vector<int> result(size - k + 1);
  auto out = result.data();
  std::multiset<int, std::greater<int>> window(data, data + k);
  for (auto lhs = data, rhs = data + k, end = data + size; rhs != end; ++lhs, ++rhs) {
    *out++ = *window.begin();
    window.erase(window.lower_bound(*lhs));
    window.insert(*rhs);
  }
  *out++ = *window.begin();
  return result;
}

template <>
std::vector<int> run<solution::deque>(const std::vector<int>& nums, int k)
{
  const auto data = nums.data();
  const auto size = nums.size();
  std::vector<int> result(size - k + 1);
  auto out = result.data();
  std::deque<int> queue;
  auto lhs = 0;
  auto rhs = 0;
  while (rhs < size) {
    while (!queue.empty() && nums[queue.back()] < nums[rhs]) {
      queue.pop_back();
    }
    queue.push_back(rhs);
    if (lhs > queue.front()) {
      queue.pop_front();
    }
    if (rhs + 1 >= k) {
      *out++ = nums[queue.front()];
      lhs++;
    }
    rhs++;
  }
  return result;
}

}  // namespace sliding_window::sliding_window_maximum

#include <sstream>

std::string format(const auto& result)
{
  std::ostringstream oss;
  for (auto i : result) {
    oss << i << ' ';
  }
  return oss.str();
}

#if ENABLE_TESTS

TEST_CASE("sliding_window::sliding_window_maximum::run")
{
  using namespace sliding_window::sliding_window_maximum;
  const std::vector<int> n0{ 1, 3, -1, -3, 5, 3, 6, 7 };
  const std::vector<int> n1{ 1 };
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(format(run<solution.value>(n0, 3)) == "3 3 5 5 6 7 ");
    REQUIRE(format(run<solution.value>(n1, 1)) == "1 ");
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using sliding_window::sliding_window_maximum::solution;

template <solution Solution>
void sliding_window_sliding_window_maximum_run(benchmark::State& state)
{
  using namespace sliding_window::sliding_window_maximum;
  std::vector<int> nums(10010);
  auto it = nums.data();
  for (auto i = 1; i <= 10; i++) {
    const auto m = (i - 1) * 1'000;
    for (auto j = i * 1'000; j >= m; j--) {
      *it++ = j;
    }
  }
  for (auto _ : state) {
    auto result = run<Solution>(nums, 5'000).size();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(sliding_window_sliding_window_maximum_run, solution::simple);
BENCHMARK_TEMPLATE(sliding_window_sliding_window_maximum_run, solution::multiset);
BENCHMARK_TEMPLATE(sliding_window_sliding_window_maximum_run, solution::deque);

#endif  // ENABLE_BENCHMARKS
