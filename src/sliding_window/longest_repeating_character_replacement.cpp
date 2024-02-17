// https://leetcode.com/problems/longest-repeating-character-replacement/
#include <common.hpp>
#include <algorithm>
#include <array>
#include <string>

namespace sliding_window::longest_repeating_character_replacement {

int run(const std::string& s, int k)
{
  const auto data = s.data();
  const auto size = s.size();
  std::array<int, 'Z' + 1> count{};
  auto result = 0;
  auto max = 0;
  for (auto lhs = data, rhs = data, end = data + size; rhs != end; ++rhs) {
    // Count and track the character, with the most number of occurances.
    max = std::max(max, ++count[*rhs]);

    // Validate current window.
    if (rhs - lhs + 1 - max > k) {
      // Remove left character from the window, since the window size exceeds `k`.
      count[*lhs]--;
      ++lhs;
    }

    // Store preliminary result.
    result = std::max(result, static_cast<int>(rhs - lhs + 1));
  }
  return result;
}

}  // namespace sliding_window::longest_repeating_character_replacement

#if ENABLE_TESTS

TEST_CASE("sliding_window::longest_repeating_character_replacement::run")
{
  using namespace sliding_window::longest_repeating_character_replacement;
  REQUIRE(run("ABAB", 2) == 4);
  REQUIRE(run("AABABBA", 1) == 4);
}

#endif  // ENABLE_TESTS
