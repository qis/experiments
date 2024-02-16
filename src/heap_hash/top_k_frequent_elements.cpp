
// https://leetcode.com/problems/top-k-frequent-elements/
// -------------------------------------------------------------------------------------------------------
// Benchmark                                                             Time             CPU   Iterations
// -------------------------------------------------------------------------------------------------------
// heap_hash_top_k_frequent_elements_small<solution::simple>           379 ns          369 ns      1947826
// heap_hash_top_k_frequent_elements_small<solution::optimized>        179 ns          180 ns      3733333
// heap_hash_top_k_frequent_elements_large<solution::simple>         11236 ns        10986 ns        64000
// heap_hash_top_k_frequent_elements_large<solution::optimized>      10364 ns        10254 ns        64000

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

namespace heap_hash::top_k_frequent_elements {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  optimized);
// clang-format on

template <solution Solution>
std::vector<int> run(const std::vector<int>& nums, int k)
{
  std::unordered_map<int, std::size_t> hashmap;
  for (auto i : nums) {
    hashmap[i]++;
  }
  std::priority_queue<int, std::vector<int>> heap;
  for (const auto& it : hashmap) {
    heap.push(it.second);
  }
  std::vector<int> result;
  while (k > 0) {
    const auto top = heap.top();
    heap.pop();
    for (const auto& it : hashmap) {
      if (it.second == top) {
        result.push_back(it.first);
        hashmap.erase(it.first);
        k--;
        break;
      }
    }
  }
  return result;
}

template <>
std::vector<int> run<solution::optimized>(const std::vector<int>& nums, int k)
{
  struct entry {
    int value;
    int count;
  };
  std::vector<entry> table;
  for (const auto n : nums) {
    const auto end = table.end();
    auto it = std::find_if(table.begin(), end, [n](const auto& e) noexcept {
      return e.value == n;
    });
    if (it != end) {
      it->count++;
    } else {
      table.emplace_back(n, 1);
    }
  }
  std::sort(table.begin(), table.end(), [](const auto& lhs, const auto& rhs) noexcept {
    return lhs.count > rhs.count;
  });
  std::vector<int> result;
  for (const auto& e : table) {
    if (!k--) {
      break;
    }
    result.push_back(e.value);
  }
  return result;
}

}  // namespace heap_hash::top_k_frequent_elements

#include <sstream>

std::string format(const auto& result)
{
  std::ostringstream oss;
  for (const auto& e : result) {
    oss << e << ' ';
  }
  return oss.str();
}

#if ENABLE_TESTS

TEST_CASE("heap_hash::top_k_frequent_elements::run")
{
  using namespace heap_hash::top_k_frequent_elements;
  const std::vector<int> n0{ 1, 1, 1, 2, 2, 3 };
  const std::vector<int> n1{ 1 };
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(format(run<solution.value>(n0, 2)) == "1 2 ");
    REQUIRE(format(run<solution.value>(n1, 1)) == "1 ");
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using heap_hash::top_k_frequent_elements::solution;

template <solution Solution>
void heap_hash_top_k_frequent_elements_small(benchmark::State& state)
{
  using namespace heap_hash::top_k_frequent_elements;
  const std::vector<int> nums{ 1, 1, 1, 2, 2, 3 };
  for (auto _ : state) {
    auto result = run<Solution>(nums, 2).empty();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_elements_small, solution::simple);
BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_elements_small, solution::optimized);

template <solution Solution>
void heap_hash_top_k_frequent_elements_large(benchmark::State& state)
{
  using namespace heap_hash::top_k_frequent_elements;
  const std::vector<int> nums{
    -5,  -9,  67,  -10, 4,   -57, 47,  13,  -67, -26, -57, 63,  38,  -68, 62,  -45, -37, 95,  49,  -91, -53, -42, -33,
    80,  78,  -30, -36, 22,  9,   -86, 79,  -1,  44,  -92, 30,  -68, -94, 58,  -51, -26, -38, 5,   -74, 25,  71,  -93,
    52,  -12, -86, 7,   -86, 53,  78,  -31, -5,  -87, 88,  -98, -39, 9,   99,  23,  96,  -90, 51,  -64, 35,  -73, 9,
    60,  -78, 70,  99,  14,  70,  71,  -78, 50,  7,   46,  -89, 57,  -1,  87,  -87, -95, 48,  49,  79,  -54, 31,  28,
    -27, 75,  31,  -76, -12, 35,  40,  -90, -60, -60, -7,  67,  73,  -34, -42, -35, 61,  51,  18,  95,  16,  78,  -81,
    -91, -30, 92,  57,  -79, 5,   41,  29,  72,  -62, -47, 80,  29,  1,   -21, -36, 5,   82,  4,   -12, -52, -56, -47,
    -68, 95,  85,  -87, -7,  -12, 98,  75,  -64, -93, 11,  73,  -81, -9,  -12, -9,  51,  17,  -94, 33,  -9,  57,  -35,
    10,  -17, 87,  -18, -55, -52, 30,  -62, 73,  35,  -74, -47, -63, 77,  -72, -55, 5,   73,  21,  14,  7,   -65, -51,
    -55, -49, 98,  -20, -22, -68, 34,  -20, 92,  55,  47,  -20, 6,   -54, -12, 3,   75,  69,  60,  15,  88,  64,  2,
    -27, -50, 55,  73,  46,  -15, -64, 93,  -47, -75, -55, -75, 21,  -57, 91,  -12, -99, -68, -56, -14, -4,  -77, -94,
    55,  93,  -31, 68,  -12, -23, 59,  -56, -86, 43,  83,  -93, -78, -11, -7,  96,  -3,  -87, -37, 19,  -78, 67,  -29,
    77,  -28, 91,  -73, -68, -22, 18,  -7,  3,   15,  77,  99,  31,  -48, -86, -45, -82, 52,  -39, 8,   -88, -83, -58,
    -77, 5,   87,  -61, 50,  32,  -66, -36, 60,  -53, 52,  70,  -36, -1,  83,  -56, 33,  98,  -80, 28,  1,   -21, -50,
    -60, 44,  99,  18,  83,  -29, 83,  -36, -55, -6,  96,  -60, 61,  75,  6,   -57, 2,   82,  62,  -27, 36,  60,  72,
    92,  61,  -65, 79,  -57, -34, -23, -28, -55, 53,  36,  -80, 5,   -76, 64,  -81, -32, -43, -1,  50,  -16, -72, -74,
    22,  88,  28,  -79, -99, 85,  -13, -34, -76, 85,  6,   21,  -99, 10,  -46, 79,  11,  -70, 17,  47,  -22, -62, 0,
    6,   75,  -19, 57,  -25, -52, -83, 90,  21,  95,  52,  68,  47,  -12, 76,  -9,  -65, 86,  90,  16,  74,  64,  26,
    84,  64,  -42, 97,  -72, 53,  -76, 11,  89,  -62, 67,  100, 15,  53,  68,  -16, 24,  11,  -77, 20,  59,  -95, -50,
    -20, 27,  45,  94,  13,  -93, 86,  49,  12,  19,  17,  -33, -52, -28, 71,  79,  -19, -73, 40,  -99, 83,  77,  19,
    -20, 98,  86,  -5,  -5,  73,  18,  100, 73,  -45, 33,  3,   89,  32,  -53, 73,  16,  -3,  -26, -80, 49,  -78, 67,
    31,  1,   -85, -44, -91, -68, 75,  -74, 95,  23,  89,  99,  -84, 54,  -93, 68,  0,   -41, 66,  -15, -27, -23, -9,
    -68, 37,  45,  -69, 57,  80,  10,  -64, 35,  26,  55,  -67, 31,  -76, 36,  -99, 21,
  };
  for (auto _ : state) {
    auto result = run<Solution>(nums, 7).empty();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_elements_large, solution::simple);
BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_elements_large, solution::optimized);

#endif  // ENABLE_BENCHMARKS
