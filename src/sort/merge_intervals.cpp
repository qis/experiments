// https://leetcode.com/problems/merge-intervals/
#include <common.hpp>
#include <boost/container/static_vector.hpp>
#include <algorithm>
#include <array>
#include <vector>

namespace sort::merge_intervals {

//using interval = boost::container::static_vector<int, 2>;
//using interval = std::vector<int>;
using interval = std::array<int, 2>;

std::vector<interval> run(std::vector<interval>& intervals)
{
  const auto size = intervals.size();
  if (size <= 1) {
    return std::move(intervals);
  }
  const auto beg = intervals.data();
  const auto end = beg + size;
  std::sort(beg, end);
  std::vector<interval> result;
  for (auto it = beg; it != end; ++it) {
    std::size_t skip = 0;
    auto& it1 = (*it)[1];
    for (auto in = it + 1; in != end && (*in)[0] <= it1; ++in) {
      const auto in1 = (*in)[1];
      if (in1 > it1) {
        it1 = in1;
      }
      ++skip;
    }
    result.push_back(*it);
    it += skip;
  }
  return result;
}

}  // namespace sort::merge_intervals

#if ENABLE_TESTS

#include <sstream>

std::string format(const auto& intervals)
{
  std::ostringstream oss;
  for (auto i = intervals.cbegin(); i != intervals.cend(); ++i) {
    if (i != intervals.cbegin()) {
      oss << ',';
    }
    oss << '[';
    for (auto j = i->cbegin(); j != i->cend(); ++j) {
      if (j != i->cbegin()) {
        oss << ',';
      }
      oss << *j;
    }
    oss << ']';
  }
  return oss.str();
}

TEST_CASE("sort::merge_intervals::run")
{
  using namespace sort::merge_intervals;
  std::vector<interval> i0{ { 1, 3 }, { 2, 6 }, { 8, 10 }, { 15, 18 } };
  REQUIRE(format(run(i0)) == "[1,6],[8,10],[15,18]");

  std::vector<interval> i1{ { 1, 4 }, { 4, 5 } };
  REQUIRE(format(run(i1)) == "[1,5]");

  std::vector<interval> i2{ { 1, 4 }, { 0, 4 } };
  REQUIRE(format(run(i2)) == "[0,4]");

  std::vector<interval> i3{ { 1, 4 }, { 2, 3 } };
  REQUIRE(format(run(i3)) == "[1,4]");
}

#endif  // ENABLE_TESTS
