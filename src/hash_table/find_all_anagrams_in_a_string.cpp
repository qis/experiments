// https://leetcode.com/problems/find-all-anagrams-in-a-string/
// ------------------------------------------------------------------------------------------------------------
// Benchmark                                                                  Time             CPU   Iterations
// ------------------------------------------------------------------------------------------------------------
// hash_table_find_all_anagrams_in_a_string_run<solution::simple>         10070 ns        10254 ns        64000
// hash_table_find_all_anagrams_in_a_string_run<solution::optimized>        813 ns          837 ns       896000

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

namespace hash_table::find_all_anagrams_in_a_string {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  optimized);
// clang-format on

template <solution Solution>
std::vector<int> run(const std::string& s, const std::string& p)
{
  const auto ssize = s.size();
  const auto psize = p.size();
  if (ssize < psize) {
    return {};
  }
  const auto pcount = [&p]() noexcept {
    std::array<int, 256> count{};
    for (const auto c : p) {
      ++count[c];
    }
    return count;
  }();
  const auto is_anagram = [psize, &pcount](const char* const beg) noexcept {
    auto scount = pcount;
    const auto end = beg + psize;
    for (auto it = beg; it < end; ++it) {
      --scount[*it];
    }
    const auto sbeg = scount.data() + 'A';
    const auto send = scount.data() + 'z';
    return std::all_of(sbeg, send, [](const int i) noexcept {
      return i == 0;
    });
  };
  std::vector<int> result;
  const auto data = s.data();
  const auto size = static_cast<int>(ssize - psize);
  result.reserve(static_cast<std::size_t>(size));
  for (auto i = 0; i <= size; i++) {
    if (is_anagram(data + i)) {
      result.push_back(i);
    }
  }
  return result;
}

template <>
std::vector<int> run<solution::optimized>(const std::string& s, const std::string& p)
{
  const auto ssize = static_cast<int>(s.size());
  const auto psize = static_cast<int>(p.size());
  if (ssize < psize) {
    return {};
  }
  std::array<int, 256> count{};
  for (const auto c : p) {
    ++count[c];
  }
  auto required = psize;
  std::vector<int> result;
  result.reserve(static_cast<std::size_t>(ssize - psize));
  for (int l = 0, r = 0; r < ssize; ++r) {
    if (--count[s[r]] >= 0) {
      --required;
    }
    while (required == 0) {
      if (r - l + 1 == psize) {
        result.push_back(l);
      }
      if (++count[s[l++]] > 0) {
        ++required;
      }
    }
  }
  return result;
}

}  // namespace hash_table::find_all_anagrams_in_a_string

#if ENABLE_TESTS

TEST_CASE("hash_table::find_all_anagrams_in_a_string::run")
{
  using namespace hash_table::find_all_anagrams_in_a_string;
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(run<solution.value>("cbaebabacd", "abc") == std::vector<int>{ 0, 6 });
    REQUIRE(run<solution.value>("abab", "ab") == std::vector<int>{ 0, 1, 2 });
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using hash_table::find_all_anagrams_in_a_string::solution;

template <solution Solution>
void hash_table_find_all_anagrams_in_a_string_run(benchmark::State& state)
{
  using namespace hash_table::find_all_anagrams_in_a_string;
  std::string s;
  s.reserve(400);
  for (std::size_t i = 0; i < 200; i++) {
    s.append("ab");
  }
  const std::string p{ "ab" };
  for (auto _ : state) {
    auto result = run<Solution>(s, p);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(hash_table_find_all_anagrams_in_a_string_run, solution::simple);
BENCHMARK_TEMPLATE(hash_table_find_all_anagrams_in_a_string_run, solution::optimized);

#endif  // ENABLE_BENCHMARKS
