// https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
#include <common.hpp>
#include <algorithm>
#include <limits>
#include <vector>

namespace greedy_problems::best_time_to_buy_and_sell_stock {

int run(const std::vector<int>& prices) noexcept
{
  auto ans = 0;
  auto min = std::numeric_limits<int>::max();
  for (const auto i : prices) {
    ans = std::max(ans, i - min);
    min = std::min(min, i);
  }
  return ans;
}

}  // namespace greedy_problems::best_time_to_buy_and_sell_stock

#if ENABLE_TESTS

TEST_CASE("greedy_problems::best_time_to_buy_and_sell_stock::run")
{
  using namespace greedy_problems::best_time_to_buy_and_sell_stock;
  const std::vector<int> p0{ 7, 1, 5, 3, 6, 4 };
  const std::vector<int> p1{ 7, 6, 4, 3, 1 };
  const std::vector<int> p2{ 3, 2, 6, 5, 0, 3 };
  const std::vector<int> p3{ 2, 1, 2, 1, 0, 1, 2 };
  REQUIRE(run(p0) == 5);
  REQUIRE(run(p1) == 0);
  REQUIRE(run(p2) == 4);
  REQUIRE(run(p3) == 2);
}

#endif  // ENABLE_TESTS
