// https://leetcode.com/problems/sliding-window-median/
// --------------------------------------------------------------------------------------------------------
// Benchmark                                                              Time             CPU   Iterations
// --------------------------------------------------------------------------------------------------------
// sliding_window_sliding_window_median_run<solution::simple>     363951150 ns    359375000 ns            2
// sliding_window_sliding_window_median_run<solution::one_heap>      819379 ns       837054 ns          896
// sliding_window_sliding_window_median_run<solution::two_heaps>     637416 ns       627790 ns          896

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <algorithm>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

namespace sliding_window::sliding_window_median {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  one_heap,
  two_heaps);
// clang-format on

template <solution Solution>
std::vector<double> run(const std::vector<int>& nums, int k)
{
  std::vector<int> buffer(static_cast<std::size_t>(k));
  const auto tmp = buffer.data();
  const auto beg = nums.data();
  const auto end = beg + nums.size() - k + 1;
  std::vector<double> result;
  result.reserve(nums.size() - k + 1);
  const auto even = k % 2 == 0;
  const auto half = k / 2 - (even ? 1 : 0);
  for (auto it = beg; it != end; ++it) {
    std::copy(it, it + k, tmp);
    std::sort(tmp, tmp + k);
    result.push_back(even ? *(tmp + half) / 2.0 + *(tmp + half + 1) / 2.0 : *(tmp + half));
  }
  return result;
}

template <>
std::vector<double> run<solution::one_heap>(const std::vector<int>& nums, int k)
{
  const auto data = nums.data();
  const auto size = nums.size();
  const auto even = k % 2 == 0;
  const auto half = k / 2 - (even ? 1 : 0);
  std::vector<double> result(size - k + 1);
  auto out = result.data();

  std::multiset<int> window(data, data + k);
  auto mid = std::next(window.begin(), half);

  if (even) {
    for (auto lhs = data, rhs = data + k, end = data + size; rhs != end; ++lhs, ++rhs) {
      *out++ = *mid / 2.0 + *std::next(mid) / 2.0;
      window.insert(*rhs);
      if (*rhs < *mid) {
        --mid;
      }
      if (*lhs <= *mid) {
        ++mid;
      }
      window.erase(window.lower_bound(*lhs));
    }
    *out++ = *mid / 2.0 + *std::next(mid) / 2.0;
  } else {
    for (auto lhs = data, rhs = data + k, end = data + size; rhs != end; ++lhs, ++rhs) {
      *out++ = *mid;
      window.insert(*rhs);
      if (*rhs < *mid) {
        --mid;
      }
      if (*lhs <= *mid) {
        ++mid;
      }
      window.erase(window.lower_bound(*lhs));
    }
    *out++ = *mid;
  }
  return result;
}

template <>
std::vector<double> run<solution::two_heaps>(const std::vector<int>& nums, int k)
{
  const auto data = nums.data();
  const auto size = nums.size();
  const auto even = k % 2 == 0;
  const auto half = k / 2 - (even ? 1 : 0);
  std::vector<double> result(size - k + 1);
  auto out = result.data();

  std::priority_queue<int> lo;
  std::priority_queue<int, std::vector<int>, std::greater<int>> hi;
  std::unordered_map<int, int> table;

  // Index of current incoming element being processed.
  auto i = 0;

  // Distribute the first `k` elements to the `lo` and `hi` heaps according to their values.
  while (i < k) {
    lo.push(nums[i++]);
  }
  for (auto j = 0; j < k / 2; j++) {
    hi.push(lo.top());
    lo.pop();
  }

  for (auto lhs = data, rhs = data + k, end = data + size; rhs != end; ++lhs, ++rhs) {
    const auto lt = lo.top();

    // Get median of current window.
    *out++ = even ? lt / 2.0 + hi.top() / 2.0 : lt;

    const auto l = *lhs;  // outgoing element
    const auto r = *rhs;  // incoming element

    // Number `l` exits window.
    auto balance = l <= lt ? -1 : 1;
    table[l]++;

    // Number `r` enters window.
    if (!lo.empty() && r <= lt) {
      balance++;
      lo.push(r);
    } else {
      balance--;
      hi.push(r);
    }

    // Re-balance heaps.
    if (balance < 0) {  // `lo` needs more valid elements
      lo.push(hi.top());
      hi.pop();
      balance++;
    } else if (balance > 0) {  // `hi` needs more valid elements
      hi.push(lo.top());
      lo.pop();
      balance--;
    }

    // Remove invalid numbers that should be discarded from heap tops.
    while (table[lo.top()]) {
      table[lo.top()]--;
      lo.pop();
    }
    while (!hi.empty() && table[hi.top()]) {
      table[hi.top()]--;
      hi.pop();
    }
  }
  // Get median of last window.
  *out++ = even ? lo.top() / 2.0 + hi.top() / 2.0 : lo.top();
  return result;
}

}  // namespace sliding_window::sliding_window_median

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

TEST_CASE("sliding_window::sliding_window_median::run")
{
  using namespace sliding_window::sliding_window_median;
  const std::vector<int> n0{ 1, 3, -1, -3, 5, 3, 6, 7 };
  const std::vector<int> n1{ 1, 2, 3, 4, 2, 3, 1, 4, 2 };
  const std::vector<int> n2{ 1, 4, 2, 3 };
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(format(run<solution.value>(n0, 3)) == "1 -1 -1 3 5 6 ");
    REQUIRE(format(run<solution.value>(n1, 3)) == "2 3 3 3 2 3 2 ");
    REQUIRE(format(run<solution.value>(n2, 4)) == "2.5 ");
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using sliding_window::sliding_window_median::solution;

template <solution Solution>
void sliding_window_sliding_window_median_run(benchmark::State& state)
{
  using namespace sliding_window::sliding_window_median;
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

BENCHMARK_TEMPLATE(sliding_window_sliding_window_median_run, solution::simple);
BENCHMARK_TEMPLATE(sliding_window_sliding_window_median_run, solution::one_heap);
BENCHMARK_TEMPLATE(sliding_window_sliding_window_median_run, solution::two_heaps);

#endif  // ENABLE_BENCHMARKS
