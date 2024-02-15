// https://leetcode.com/problems/valid-parentheses/
#include <common.hpp>
#include <stack>
#include <string>
#include <vector>

namespace queue_stack::valid_parentheses {

bool run(const std::string& s) noexcept
{
  if (s.size() % 2 != 0) {
    return false;
  }
  // clang-format off
  static constexpr char open[130]{
  //  0    1    2    3    4    5    6    7    8    9
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //   9
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  19
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  29
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  39
    ')',   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  49
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  59
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  69
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  79
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  89
      0, ']',   0,   0,   0,   0,   0,   0,   0,   0,  //  99
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 109
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 119
      0,   0,   0, '}',   0,   0,   0,   0,   0,   0,  // 129
  };
  // clang-format on
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
