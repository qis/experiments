// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/
#include <common.hpp>
#include <algorithm>
#include <limits>
#include <vector>

namespace greedy_problems::best_time_to_buy_and_sell_stock_ii {

int run(const std::vector<int>& prices) noexcept
{
  auto ans = 0;
  auto max = std::numeric_limits<int>::max();
  for (auto i : prices) {
    if (i > max) {
      ans += i - max;
    }
    max = i;
  }
  return ans;
}

}  // namespace greedy_problems::best_time_to_buy_and_sell_stock_ii

#if ENABLE_TESTS

TEST_CASE("greedy_problems::best_time_to_buy_and_sell_stock_ii::run")
{
  using namespace greedy_problems::best_time_to_buy_and_sell_stock_ii;
  const std::vector<int> p0{ 7, 1, 5, 3, 6, 4 };
  const std::vector<int> p1{ 1, 2, 3, 4, 5 };
  const std::vector<int> p2{ 7, 6, 4, 3, 1 };
  REQUIRE(run(p0) == 7);
  REQUIRE(run(p1) == 4);
  REQUIRE(run(p2) == 0);
}

#endif  // ENABLE_TESTS
