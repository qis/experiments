// https://leetcode.com/problems/search-a-2d-matrix/

#include <common.hpp>
#include <algorithm>
#include <utility>
#include <vector>

namespace binary_search::search_a_2d_matrix {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

bool simple(std::vector<std::vector<int>>& matrix, int target) noexcept
{
  if (matrix.empty() || matrix[0].empty()) {
    return false;
  }
  const std::vector<int> tmp;
  const auto beg = matrix.cbegin();
  const auto end = matrix.cend();
  auto row = std::lower_bound(beg, end, tmp, [target](const auto& row, const auto&) noexcept {
    return row[0] <= target;
  });
  if (row == beg) {
    return false;
  }
  row = std::prev(row);
  const auto row_beg = row->cbegin();
  const auto row_end = row->cend();
  if (const auto it = std::lower_bound(row_beg, row_end, target); it != row_end) {
    return *it == target;
  } else {
    return *std::prev(it) == target;
  }
  std::unreachable();
  return false;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

std::vector<std::vector<int>> test(std::size_t x, std::size_t y, int skip = -1)
{
  std::vector<std::vector<int>> matrix;
  matrix.reserve(y);
  auto v = 1;
  for (std::size_t i = 0; i < y; i++) {
    auto& row = matrix.emplace_back();
    row.reserve(x);
    for (std::size_t j = 0; j < x; j++) {
      row.push_back(v == skip ? skip + 1 : v);
      v++;
    }
  }
  return matrix;
}

}  // namespace binary_search::search_a_2d_matrix

#if ENABLE_TESTS

TEST_CASE("binary_search::search_a_2d_matrix::simple")
{
  auto m = binary_search::search_a_2d_matrix::test(0, 0);
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 0));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 1));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 2));

  m = binary_search::search_a_2d_matrix::test(1, 0);
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 0));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 1));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 2));

  m = binary_search::search_a_2d_matrix::test(1, 1);
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 0));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 1));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 2));

  m = binary_search::search_a_2d_matrix::test(2, 1, 2);
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 0));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 1));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 2));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 3));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 4));

  m = binary_search::search_a_2d_matrix::test(1, 2, 2);
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 0));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 1));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 2));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 3));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 4));

  m = binary_search::search_a_2d_matrix::test(10, 10, 2);
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 0));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 1));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 2));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 3));
  REQUIRE(binary_search::search_a_2d_matrix::simple(m, 100));
  REQUIRE(!binary_search::search_a_2d_matrix::simple(m, 101));
}

#endif  // ENABLE_TESTS
