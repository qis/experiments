// https://leetcode.com/problems/container-with-most-water/
#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <vector>

namespace two_pointers::container_with_most_water {

int run(const std::vector<int>& height)
{
  auto max = 0;
  auto lhs = height.data();
  auto rhs = lhs + height.size() - 1;
  while (lhs < rhs) {
    const auto x = rhs - lhs;
    const auto l = *lhs;
    const auto r = *rhs;
    if (l < r) {
      if (const auto cur = l * x; cur > max) {
        max = cur;
      }
      ++lhs;
    } else {
      if (const auto cur = r * x; cur > max) {
        max = cur;
      }
      --rhs;
    }
  }
  return max;
}

}  // namespace two_pointers::container_with_most_water

#if ENABLE_TESTS

TEST_CASE("two_pointers::container_with_most_water::run")
{
  using namespace two_pointers::container_with_most_water;
  const std::vector<int> h0{ 1, 8, 6, 2, 5, 4, 8, 3, 7 };
  REQUIRE(run(h0) == 49);
  const std::vector<int> h1{ 1, 1 };
  REQUIRE(run(h1) == 1);
}

#endif  // ENABLE_TESTS
