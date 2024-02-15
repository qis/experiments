// https://leetcode.com/problems/remove-invalid-parentheses/
#include <common.hpp>
#include <algorithm>
#include <string>
#include <vector>

namespace dfs_bfs::remove_invalid_parentheses {

void remove(std::string s, std::vector<std::string>& result, std::size_t li, std::size_t lj, char o, char c)
{
  auto counter = 0;
  const auto size = s.size();
  for (auto i = li; i < size; i++) {
    if (s[i] == o) {
      counter++;
    } else if (s[i] == c) {
      counter--;
    }
    if (counter >= 0) {
      continue;
    }
    for (auto j = lj; j <= i; j++) {
      if (s[j] == c && (j == lj || s[j - 1] != c)) {
        remove(s.substr(0, j) + s.substr(j + 1, size - j - 1), result, i, j, o, c);
      }
    }
    return;
  }
  auto r = s;
  std::reverse(r.begin(), r.end());
  if (o == '(') {
    remove(std::move(r), result, 0, 0, ')', '(');
  } else {
    result.push_back(std::move(r));
  }
}

std::vector<std::string> run(std::string s)
{
  std::vector<std::string> result;
  remove(std::move(s), result, 0, 0, '(', ')');
  return result;
}

}  // namespace dfs_bfs::remove_invalid_parentheses

#if ENABLE_TESTS

TEST_CASE("dfs_bfs::remove_invalid_parentheses::run")
{
  using namespace dfs_bfs::remove_invalid_parentheses;
  const auto r0 = run("()())()");
  REQUIRE(r0.size() == 2);
  REQUIRE(std::find(r0.begin(), r0.end(), "(())()") != r0.end());
  REQUIRE(std::find(r0.begin(), r0.end(), "()()()") != r0.end());
  const auto r1 = run("(a)())()");
  REQUIRE(r1.size() == 2);
  REQUIRE(std::find(r1.begin(), r1.end(), "(a())()") != r1.end());
  REQUIRE(std::find(r1.begin(), r1.end(), "(a)()()") != r1.end());
  const auto r2 = run(")(");
  REQUIRE(r2.size() == 1);
  REQUIRE(std::find(r2.begin(), r2.end(), "") != r2.end());
}

#endif  // ENABLE_TESTS
