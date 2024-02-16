// https://leetcode.com/problems/top-k-frequent-words/
// ----------------------------------------------------------------------------------------------------
// Benchmark                                                          Time             CPU   Iterations
// ----------------------------------------------------------------------------------------------------
// heap_hash_top_k_frequent_words_small<solution::simple>           579 ns          578 ns      1000000
// heap_hash_top_k_frequent_words_small<solution::optimized>        712 ns          698 ns       896000
// heap_hash_top_k_frequent_words_large<solution::simple>          2951 ns         2982 ns       235789
// heap_hash_top_k_frequent_words_large<solution::optimized>       7540 ns         7324 ns        74667

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <algorithm>
#include <map>
#include <queue>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace heap_hash::top_k_frequent_words {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  optimized);
// clang-format on

template <solution Solution>
std::vector<std::string> run(const std::vector<std::string>& words, int k)
{
  struct entry {
    std::size_t hash{ 0 };
    std::size_t count{ 0 };
    std::string_view word;
  };
  std::vector<entry> table;
  for (auto& word : words) {
    const auto hash = std::hash<std::string_view>{}(word);
    const auto compare = [hash](const auto& e) noexcept {
      return e.hash == hash;
    };
    const auto end = table.end();
    if (auto it = std::find_if(table.begin(), end, compare); it != end) {
      it->count++;
    } else {
      table.emplace_back(hash, 1, word);
    }
  }
  std::sort(table.begin(), table.end(), [](const auto& lhs, const auto& rhs) noexcept {
    if (lhs.count == rhs.count) {
      return lhs.word.compare(rhs.word) < 0;
    }
    return lhs.count > rhs.count;
  });
  std::vector<std::string> result;
  for (auto& e : table) {
    if (k <= 0) {
      break;
    }
    result.emplace_back(e.word);
    k--;
  }
  return result;
}

template <>
std::vector<std::string> run<solution::optimized>(const std::vector<std::string>& words, int k)
{
  std::priority_queue<int> heap;
  std::map<std::string_view, int> hashmap;
  std::vector<std::string> result;
  for (const auto& word : words) {
    hashmap[word]++;
  }
  for (const auto& it : hashmap) {
    heap.push(it.second);
  }
  while (k > 0) {
    const auto top = heap.top();
    heap.pop();
    for (const auto& it : hashmap) {
      if (top == it.second) {
        result.emplace_back(it.first);
        hashmap.erase(it.first);
        k--;
        break;
      }
    }
  }
  return result;
}

}  // namespace heap_hash::top_k_frequent_words

#if ENABLE_TESTS

TEST_CASE("heap_hash::top_k_frequent_words::run")
{
  using namespace heap_hash::top_k_frequent_words;

  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    const std::vector<std::string> w0{ "i", "love", "leetcode", "i", "love", "coding" };
    const auto r0 = run<solution.value>(w0, 2);
    REQUIRE(r0.size() == 2);
    REQUIRE(r0[0] == "i");
    REQUIRE(r0[1] == "love");

    const std::vector<std::string> w1{ "the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is" };
    const auto r1 = run<solution.value>(w1, 4);
    REQUIRE(r1.size() == 4);
    REQUIRE(r1[0] == "the");
    REQUIRE(r1[1] == "is");
    REQUIRE(r1[2] == "sunny");
    REQUIRE(r1[3] == "day");
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using heap_hash::top_k_frequent_words::solution;

template <solution Solution>
void heap_hash_top_k_frequent_words_small(benchmark::State& state)
{
  using namespace heap_hash::top_k_frequent_words;
  const std::vector<std::string> words{
    "the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is",
  };
  for (auto _ : state) {
    auto result = run<Solution>(words, 4).empty();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_words_small, solution::simple);
BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_words_small, solution::optimized);

template <solution Solution>
void heap_hash_top_k_frequent_words_large(benchmark::State& state)
{
  using namespace heap_hash::top_k_frequent_words;
  const std::vector<std::string> words{
    "mrxoofvzrs", "yhnlafgy",   "eszipx",     "vqbqmnk",    "dyzk",       "waa",        "owjcfjp",    "amohed",
    "hacmbdvs",   "owjcfjp",    "iwdv",       "mrxoofvzrs", "gxjnxoq",    "gxjnxoq",    "hacmbdvs",   "ntubtyw",
    "xofhe",      "fyypfyx",    "dyzk",       "aavhiasb",   "pyggsye",    "vqbqmnk",    "waa",        "ukbdosxorh",
    "waa",        "pyggsye",    "ukbdosxorh", "fyypfyx",    "mrxoofvzrs", "qujjz",      "vqbqmnk",    "ocfp",
    "ntubtyw",    "bqwgmr",     "kzjmkhjhku", "hacmbdvs",   "gxjnxoq",    "ukbdosxorh", "pyggsye",    "pyggsye",
    "wckkv",      "waa",        "ocfp",       "gxjnxoq",    "eszipx",     "peo",        "bqwgmr",     "waa",
    "eszipx",     "mrxoofvzrs", "kzjmkhjhku", "kzjmkhjhku", "pyggsye",    "peo",        "owjcfjp",    "iwdv",
    "wckkv",      "lgstmz",     "kzjmkhjhku", "kzjmkhjhku", "szrpvy",     "amohed",     "peo",        "fyypfyx",
    "owjcfjp",    "vncgg",      "ocfp",       "amohed",     "aavhiasb",   "luu",        "kzjmkhjhku", "szrpvy",
    "hacmbdvs",   "mhavilzr",   "szrpvy",     "ukbdosxorh", "dyzk",       "amohed",     "szrpvy",     "wckkv",
    "peo",        "qujjz",      "lgtgrbjix",  "fyypfyx",    "luu",        "fyypfyx",    "rkuqqzhtal", "kzjmkhjhku",
    "qujjz",      "ocfp",       "bqwgmr",     "mhavilzr",   "wckkv",      "peo",        "eszipx",     "qujjz",
    "fsnduz",     "dyzk",       "rkuqqzhtal", "kzjmkhjhku", "vqbqmnk",    "hacmbdvs",   "peo",        "lgstmz",
    "xofhe",      "mhavilzr",   "dyzk",       "xofhe",      "ntubtyw",    "eszipx",     "iwdv",       "pyggsye",
    "ntubtyw",    "vqbqmnk",    "owjcfjp",    "qujjz",      "rkuqqzhtal", "ntubtyw",    "xflmefk",    "xflmefk",
    "szrpvy",     "dyzk",       "szrpvy",     "dyzk",       "cwn",        "gxjnxoq",    "ntubtyw",    "bqwgmr",
    "cwn",        "gxjnxoq",    "peo",        "gxjnxoq",    "cwn",        "kzjmkhjhku", "osv",        "mrxoofvzrs",
    "owjcfjp",    "owjcfjp",    "eszipx",     "lgstmz",     "szrpvy",     "eszipx",     "amohed",     "bqwgmr",
    "dyzk",       "iwdv",       "szrpvy",     "peo",        "bqwgmr",     "gge",        "owjcfjp",    "fkphnd",
    "bqwgmr",     "amohed",     "rkuqqzhtal", "qujjz",      "amohed",     "wckkv",      "cwn",        "owjcfjp",
    "ocfp",       "lgtgrbjix",  "ocfp",       "wckkv",      "pyggsye",    "lgstmz",     "aavhiasb",   "gxjnxoq",
    "yhnlafgy",   "aavhiasb",   "ocfp",       "qujjz",      "hacmbdvs",   "owjcfjp",    "gxjnxoq",    "bnvmykwx",
    "xiraocddj",  "rkuqqzhtal", "vqbqmnk",    "rkuqqzhtal", "ukbdosxorh", "eszipx",     "lgtgrbjix",  "szrpvy",
    "lgstmz",     "gge",        "mrxoofvzrs", "dyzk",       "fyypfyx",    "bqwgmr",     "hacmbdvs",   "lgtgrbjix",
    "xofhe",      "lgstmz",     "kzjmkhjhku", "wckkv",      "wckkv",      "mrxoofvzrs", "bqwgmr",     "iwdv",
    "peo",        "wckkv",      "yhnlafgy",   "xofhe",      "cwn",        "fyypfyx",    "qujjz",      "lgtgrbjix",
    "pyggsye",    "ocfp",       "hacmbdvs",   "dyzk",       "eszipx",     "ntubtyw",    "fkphnd",     "ntubtyw",
    "xofhe",      "cwn",        "peo",        "lgtgrbjix",  "rkuqqzhtal", "mhavilzr",
  };
  for (auto _ : state) {
    auto result = run<Solution>(words, 13).empty();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_words_large, solution::simple);
BENCHMARK_TEMPLATE(heap_hash_top_k_frequent_words_large, solution::optimized);

#endif  // ENABLE_BENCHMARKS
