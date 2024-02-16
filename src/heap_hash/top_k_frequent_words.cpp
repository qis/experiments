// https://leetcode.com/problems/top-k-frequent-words/
#include <common.hpp>
#include <algorithm>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace heap_hash::top_k_frequent_words {

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

}  // namespace heap_hash::top_k_frequent_words

#if ENABLE_TESTS

TEST_CASE("heap_hash::top_k_frequent_words::run")
{
  using namespace heap_hash::top_k_frequent_words;

  const std::vector<std::string> w0 = { "i", "love", "leetcode", "i", "love", "coding" };
  const auto r0 = run(w0, 2);
  REQUIRE(r0.size() == 2);
  REQUIRE(r0[0] == "i");
  REQUIRE(r0[1] == "love");

  const std::vector<std::string> w1 = { "the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is" };
  const auto r1 = run(w1, 4);
  REQUIRE(r1.size() == 4);
  REQUIRE(r1[0] == "the");
  REQUIRE(r1[1] == "is");
  REQUIRE(r1[2] == "sunny");
  REQUIRE(r1[3] == "day");
}

#endif  // ENABLE_TESTS
