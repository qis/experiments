// https://leetcode.com/problems/valid-parentheses/
#include <common.hpp>
#include <algorithm>
#include <array>
#include <stack>
#include <string>
#include <vector>

namespace queue_stack::valid_parentheses {

constexpr auto min = std::min({ 'a', 'A', 'z', 'Z', '(', ')', '[', ']', '{', '}' });
constexpr auto max = std::max({ 'a', 'A', 'z', 'Z', '(', ')', '[', ']', '{', '}' });
constexpr auto open = []() noexcept {
  std::array<char, max> table{};
  table['('] = ')';
  table['['] = ']';
  table['{'] = '}';
  return table;
}();

bool run(const std::string& s) noexcept
{
  if (s.size() % 2 != 0) {
    return false;
  }
  std::stack<char, std::vector<char>> stack;
  stack.push(0);
  for (const auto c : s) {
    if (const auto e = open[c]) {
      stack.push(e);
    } else if (c == stack.top()) {
      stack.pop();
    } else {
      return false;
    }
  }
  return stack.size() == 1;
}

}  // namespace queue_stack::valid_parentheses

#if ENABLE_TESTS

TEST_CASE("queue_stack::valid_parentheses::run")
{
  using namespace queue_stack::valid_parentheses;
  REQUIRE(run("()"));
  REQUIRE(run("()[]{}"));
  REQUIRE(run("{[]}"));
  REQUIRE(!run("([)]"));
  REQUIRE(!run("(])"));
}

#endif  // ENABLE_TESTS
