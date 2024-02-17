// https://leetcode.com/problems/partition-labels/
#include <common.hpp>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace two_pointers::partition_labels {

std::vector<int> run(const std::string& s)
{
  std::vector<int> result;
  std::map<char, std::size_t> last;
  const auto size = s.size();
  for (auto l = 0uz; l < size;) {
    auto c = s[l];
    auto r = s.find_last_of(c);
    last[c] = r;
    for (auto i = l + 1; i < r; i++) {
      c = s[i];
      if (const auto it = last.find(c); it != last.end()) {
        r = std::max(r, it->second);
      } else {
        r = std::max(r, s.find_last_of(c));
        last[c] = r;
      }
    }
    result.push_back(r - l + 1);
    l = r + 1;
  }
  return result;
}

}  // namespace two_pointers::partition_labels

#if ENABLE_TESTS

TEST_CASE("two_pointers::partition_labels::run")
{
  using namespace two_pointers::partition_labels;
  REQUIRE(run("ababcbacadefegdehijhklij") == std::vector{ 9, 7, 8 });
  REQUIRE(run("eccbbbbdec") == std::vector{ 10 });
  REQUIRE(run("e") == std::vector{ 1 });
}

#endif  // ENABLE_TESTS
