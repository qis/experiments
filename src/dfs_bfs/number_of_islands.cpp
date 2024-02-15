// https://leetcode.com/problems/number-of-islands/
// -------------------------------------------------------------------------------------------------
// Benchmark                                                       Time             CPU   Iterations
// -------------------------------------------------------------------------------------------------
// dfs_bfs_number_of_islands_run<solution::dfs_recursive>        962 ns          952 ns       640000
// dfs_bfs_number_of_islands_run<solution::dfs_stack>           2028 ns         2040 ns       344615
// dfs_bfs_number_of_islands_run<solution::bfs_list>           22584 ns        22949 ns        32000
// dfs_bfs_number_of_islands_run<solution::bfs_deque>           2121 ns         2131 ns       344615

#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <array>
#include <deque>
#include <list>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

namespace dfs_bfs::number_of_islands {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  dfs_recursive,  // 22ms
  dfs_stack,      // 34ms
  bfs_list,       // 66ms
  bfs_deque);     // 34ms
// clang-format on

template <solution Solution>
class searcher;

//using grid_container = std::vector<std::vector<char>>;
using grid_container = std::vector<std::string_view>;

template <solution Solution>
int run(const grid_container& grid) noexcept
{
  return searcher<Solution>{ grid }.count();
}

struct pair {
  const int x{ 0 };
  const int y{ 0 };
};

static constexpr int directions[4][2]{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

// =====================================================================================================================
// DFS (Recursive)
// =====================================================================================================================

template <>
class searcher<solution::dfs_recursive> {
public:
  searcher(const grid_container& grid) : grid_(grid), xs_(grid[0].size()), ys_(grid.size()), visited_(ys_ * xs_, 0) {}

  int count() noexcept
  {
    auto result = 0;
    for (auto y = 0; y < ys_; y++) {
      for (auto x = 0; x < xs_; x++) {
        const auto i = y * xs_ + x;
        if (grid_[y][x] == '1' && !visited_[i]) {
          run(x, y);
          result++;
        }
      }
    }
    return result;
  }

private:
  void run(int x, int y) noexcept
  {
    const auto i = y * xs_ + x;
    if (y < 0 || y >= ys_ || x < 0 || x >= xs_ || grid_[y][x] != '1' || visited_[i]) {
      return;
    }
    visited_[i] = 1;
    for (const auto& direction : directions) {
      run(x + direction[0], y + direction[1]);
    }
  }

  const grid_container& grid_;
  const int xs_;
  const int ys_;
  std::vector<char> visited_;
};

// =====================================================================================================================
// DFS (Stack)
// =====================================================================================================================

template <>
class searcher<solution::dfs_stack> {
public:
  searcher(const grid_container& grid) : grid_(grid), xs_(grid[0].size()), ys_(grid.size()), visited_(ys_ * xs_, 0) {}

  int count() noexcept
  {
    auto result = 0;
    for (auto y = 0; y < ys_; y++) {
      for (auto x = 0; x < xs_; x++) {
        const auto i = y * xs_ + x;
        if (grid_[y][x] == '1' && !visited_[i]) {
          run(x, y);
          result++;
        }
      }
    }
    return result;
  }

private:
  void run(int x, int y) noexcept
  {
    stack_.emplace_back(x, y);
    while (!stack_.empty()) {
      const auto e = stack_.back();
      stack_.pop_back();
      const auto i = e.y * xs_ + e.x;
      if (grid_[e.y][e.x] != '1' || visited_[i]) {
        continue;
      }
      visited_[i] = 1;
      for (const auto& direction : directions) {
        const auto xd = e.x + direction[0];
        const auto yd = e.y + direction[1];
        if (xd < 0 || xd >= xs_ || yd < 0 || yd >= ys_) {
          continue;
        }
        stack_.emplace_back(xd, yd);
      }
    }
  }

  const grid_container& grid_;
  const int xs_;
  const int ys_;
  std::vector<char> visited_;
  std::vector<pair> stack_;
};

// =====================================================================================================================
// BFS (Queue)
// =====================================================================================================================

template <class Container>
class bfs {
public:
  bfs(const grid_container& grid) : grid_(grid), xs_(grid[0].size()), ys_(grid.size()), visited_(ys_ * xs_, 0) {}

  int count() noexcept
  {
    auto result = 0;
    for (auto y = 0; y < ys_; y++) {
      for (auto x = 0; x < xs_; x++) {
        const auto i = y * xs_ + x;
        if (grid_[y][x] == '1' && !visited_[i]) {
          run(x, y);
          result++;
        }
      }
    }
    return result;
  }

private:
  void run(int x, int y) noexcept
  {
    queue_.emplace(x, y);
    while (!queue_.empty()) {
      const auto e = queue_.front();
      queue_.pop();
      const auto i = e.y * xs_ + e.x;
      if (grid_[e.y][e.x] != '1' || visited_[i]) {
        continue;
      }
      visited_[i] = 1;
      for (const auto& direction : directions) {
        const auto xd = e.x + direction[0];
        const auto yd = e.y + direction[1];
        if (xd < 0 || xd >= xs_ || yd < 0 || yd >= ys_) {
          continue;
        }
        queue_.emplace(xd, yd);
      }
    }
  }

  const grid_container& grid_;
  const int xs_;
  const int ys_;
  std::vector<char> visited_;
  std::queue<pair, Container> queue_;
};

template <>
class searcher<solution::bfs_list> : public bfs<std::list<pair>> {
  using bfs::bfs;
};

template <>
class searcher<solution::bfs_deque> : public bfs<std::deque<pair>> {
  using bfs::bfs;
};

// =====================================================================================================================
// Helpers
// =====================================================================================================================

std::vector<std::string_view> create(const std::string& str, std::size_t xs)
{
  const auto data = str.data();
  const auto size = str.size();
  std::vector<std::string_view> grid;
  grid.reserve(size / xs);
  for (auto it = data, end = data + size; it < end; it += xs) {
    grid.emplace_back(it, xs);
  }
  return grid;
}

}  // namespace dfs_bfs::number_of_islands

#if ENABLE_TESTS

TEST_CASE("dfs_bfs::number_of_islands::run")
{
  using namespace dfs_bfs::number_of_islands;

  const std::string s0{
    "11110"
    "11010"
    "11000"
    "00000"
  };
  const auto g0 = create(s0, 5);

  const std::string s1{
    "11000"
    "11000"
    "00100"
    "00011"
  };
  const auto g1 = create(s1, 5);

  const std::string s2{ "1011011" };
  const auto g2 = create(s2, 1);

  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(run<solution.value>(g0) == 1);
    REQUIRE(run<solution.value>(g1) == 3);
    REQUIRE(run<solution.value>(g2) == 3);
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using dfs_bfs::number_of_islands::solution;

template <solution Solution>
void dfs_bfs_number_of_islands_run(benchmark::State& state)
{
  using namespace dfs_bfs::number_of_islands;
  const std::string str{
    "10011101100000000000"
    "10011001000101010010"
    "00011110101100001010"
    "00011001000111001001"
    "00000001110000000000"
    "10000101011000000101"
    "00010001010101010101"
    "00010100110101101110"
    "00001001100001000101"
    "00100100000100100010"
    "10010000000100101010"
    "01000101011011101100"
    "11010000100000010001"
    "01001110001111101000"
    "00111000110001010000"
    "10010100001000101011"
    "10100000010001010000"
    "01100011101010111100"
    "01000011001010010011"
    "00000011110100011000"
  };
  const auto grid = create(str, 20);
  for (auto _ : state) {
    auto result = run<Solution>(grid);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(dfs_bfs_number_of_islands_run, solution::dfs_recursive);
BENCHMARK_TEMPLATE(dfs_bfs_number_of_islands_run, solution::dfs_stack);
BENCHMARK_TEMPLATE(dfs_bfs_number_of_islands_run, solution::bfs_list);
BENCHMARK_TEMPLATE(dfs_bfs_number_of_islands_run, solution::bfs_deque);

#endif  // ENABLE_BENCHMARKS
