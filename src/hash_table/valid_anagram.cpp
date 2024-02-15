// https://leetcode.com/problems/valid-anagram/
// --------------------------------------------------------------------------------------------
// Benchmark                                                  Time             CPU   Iterations
// --------------------------------------------------------------------------------------------
// hash_table_valid_anagram_run<solution::simple>         19901 ns        19531 ns        32000
// hash_table_valid_anagram_run<solution::optimized>       1178 ns         1172 ns       560000

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <algorithm>
#include <array>
#include <string>

namespace hash_table::valid_anagram {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  optimized);
// clang-format on

template <solution Solution>
bool run(const std::string& s, const std::string& t) noexcept
{
  auto sc = s;
  auto tc = t;
  std::sort(sc.begin(), sc.end());
  std::sort(tc.begin(), tc.end());
  return sc == tc;
}

template <>
bool run<solution::optimized>(const std::string& s, const std::string& t) noexcept
{
  if (s.empty() || s.size() != t.size()) {
    return false;
  }
  std::array<int, 256> count{};
  for (const auto c : s) {
    count[c]++;
  }
  for (const auto c : t) {
    count[c]--;
  }
  const auto beg = count.data() + 'A';
  const auto end = count.data() + 'z';
  return std::all_of(beg, end, [](int i) noexcept {
    return i == 0;
  });
}

}  // namespace hash_table::valid_anagram

#if ENABLE_TESTS

TEST_CASE("hash_table::valid_anagram::run")
{
  using namespace hash_table::valid_anagram;
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(run<solution.value>("anagram", "nagaram"));
    REQUIRE(!run<solution.value>("rat", "car"));
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using hash_table::valid_anagram::solution;

template <solution Solution>
void hash_table_valid_anagram_run(benchmark::State& state)
{
  std::string s;
  for (std::size_t i = 0; i < 1'000; i++) {
    s.push_back('a' + i % 26);
  }
  for (std::size_t i = 0; i < 1'000; i++) {
    s.push_back('A' + i % 26);
  }
  auto t = s;
  std::random_shuffle(t.begin(), t.end());
  for (auto _ : state) {
    auto result = hash_table::valid_anagram::run<Solution>(s, t);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(hash_table_valid_anagram_run, solution::simple);
BENCHMARK_TEMPLATE(hash_table_valid_anagram_run, solution::optimized);

#endif  // ENABLE_BENCHMARKS
