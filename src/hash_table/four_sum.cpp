// https://leetcode.com/problems/4sum/
// ---------------------------------------------------------------------------------------------------------
// Benchmark                                                               Time             CPU   Iterations
// ---------------------------------------------------------------------------------------------------------
// hash_table_four_sum_simple<table::boost_flat_set>                 9332289 ns      9166667 ns           75
// hash_table_four_sum_simple<table::boost_unordered_flat_set>      15996402 ns     15243902 ns           41
// hash_table_four_sum_simple<table::boost_hash>                    10491917 ns     10498047 ns           64
// hash_table_four_sum_simple<table::boost_hash_combine>            16973422 ns     16768293 ns           41
// hash_table_four_sum_simple<table::std_unordered_set_hash>         8388178 ns      8506944 ns           90
// hash_table_four_sum_simple<table::std_unordered_set_hash_mulx>    8638267 ns      8680556 ns           90
// hash_table_four_sum_simple<table::std_set>                        7544307 ns      7638889 ns           90
// hash_table_four_sum_simple<table::optimized>                          402 ns          410 ns      1792000

#include <common.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container_hash/detail/mulx.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/describe.hpp>
#include <boost/functional/hash.hpp>
#include <boost/mp11.hpp>
#include <boost/unordered/unordered_flat_set.hpp>
#include <algorithm>
#include <array>
#include <functional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace hash_table::four_sum {

// =====================================================================================================================
// Solutions
// =====================================================================================================================

// clang-format off
BOOST_DEFINE_ENUM_CLASS(table,
  boost_flat_set,                // option 1
  boost_unordered_flat_set,      // option 2
  boost_hash,                    // option 3
  boost_hash_combine,            // option 4
  std_unordered_set_hash,        // option 5
  std_unordered_set_hash_mulx,   // option 6
  std_set,                       // option 7
  optimized);                    // option 8
// clang-format on

template <table Table>
std::vector<std::vector<int>> simple(std::vector<int>& nums, int target) noexcept
{
  using quadruplet = std::array<int, 4>;
  std::sort(nums.begin(), nums.end());
  auto table = []() {
    if constexpr (Table == table::boost_flat_set) {
      // Option 1: Use boost::container::flat_set from <boost/container/flat_set.hpp>.
      return boost::container::flat_set<quadruplet>{};
    } else if constexpr (Table == table::boost_unordered_flat_set) {
      // Option 2: Use boost::unordered_flat_set from <boost/unordered/unordered_flat_set.hpp>.
      return boost::unordered_flat_set<quadruplet>{};
    } else if constexpr (Table != table::std_set) {
      struct hash {
        std::size_t operator()(const quadruplet& s) const noexcept
        {
          if constexpr (Table == table::boost_hash) {
            // Option 3: Use boost::hash from <boost/container_hash/hash.hpp>.
            return boost::hash<std::array<int, 4>>{}(s);
          } else if constexpr (Table == table::boost_hash_combine) {
            // Option 4: Use boost::hash_combine from <boost/functional/hash.hpp>.
            std::size_t seed{ 0 };
            const auto size = sizeof(quadruplet) / sizeof(std::size_t);
            const auto data = reinterpret_cast<const std::size_t*>(s.data());
            for (std::size_t i = 0; i < size; i++) {
              boost::hash_combine(seed, *(data + i));
            }
            return seed;
          } else if constexpr (Table == table::std_unordered_set_hash_mulx) {
            // Option 5: Use boost::hash_detail::mulx from <boost/container_hash/detail/mulx.hpp> in a non-portable way.
            static_assert(sizeof(int) * 2 == sizeof(std::size_t));
            auto data = reinterpret_cast<const std::size_t*>(s.data());
            const auto h1 = std::hash<std::size_t>{}(*data++);
            const auto h2 = std::hash<std::size_t>{}(*data);
            return boost::hash_detail::mulx(h1, h2);
          } else {
            // Option 6: Use std::hash from <functional> in a non-portable way.
            static_assert(Table == table::std_unordered_set_hash);
            static_assert(sizeof(int) * 2 == sizeof(std::size_t));
            auto data = reinterpret_cast<const std::size_t*>(s.data());
            const auto h1 = std::hash<std::size_t>{}(*data++);
            const auto h2 = std::hash<std::size_t>{}(*data);
            return h1 ^ (h2 << 1);
          }
        }
      };
      return std::unordered_set<quadruplet, hash>{};
    } else {
      return std::set<quadruplet>{};
    }
  }();
  const auto data = nums.data();
  const auto size = nums.size();
  if (size < 4) {
    return {};
  }
  const auto t0 = static_cast<std::int64_t>(target);
  for (std::size_t i0 = 0; i0 < size - 3; i0++) {
    const auto v0 = static_cast<std::int64_t>(data[i0]);
    for (std::size_t i1 = i0 + 1; i1 < size - 2; i1++) {
      const auto v1 = static_cast<std::int64_t>(data[i1]);
      for (std::size_t i2 = i1 + 1; i2 < size - 1; i2++) {
        const auto v2 = static_cast<std::int64_t>(data[i2]);
        for (std::size_t i3 = i2 + 1; i3 < size; i3++) {
          const auto v3 = static_cast<std::int64_t>(data[i3]);
          if (v0 + v1 + v2 + v3 == t0) {
            table.insert({ static_cast<int>(v0), static_cast<int>(v1), static_cast<int>(v2), static_cast<int>(v3) });
          }
        }
      }
    }
  }
  std::vector<std::vector<int>> result;
  result.reserve(table.size());
  for (const auto& e : table) {
    std::vector<int> row{ e[0], e[1], e[2], e[3] };
    result.emplace_back(std::move(row));
  }
  return result;
}

template <>
std::vector<std::vector<int>> simple<table::optimized>(std::vector<int>& nums, int target) noexcept
{
  std::sort(nums.begin(), nums.end());
  std::vector<std::vector<int>> result;
  const auto n = nums.size();
  for (std::size_t i = 0; i < n; i++) {
    if (i > 0 && nums[i] == nums[i - 1]) {
      continue;
    }
    for (std::size_t j = i + 1; j < n; j++) {
      if (j > i + 1 && nums[j] == nums[j - 1]) {
        continue;
      }
      auto k = j + 1;
      auto l = n - 1;
      while (k < l) {
        auto sum = static_cast<long long>(nums[i]);
        sum += nums[j];
        sum += nums[k];
        sum += nums[l];
        if (sum < target) {
          k++;
        } else if (sum > target) {
          l--;
        } else {
          result.push_back({ nums[i], nums[j], nums[k], nums[l] });
          k++;
          l--;
          while (k < l && nums[k] == nums[k - 1]) {
            k++;
          }
          while (k < l && nums[l] == nums[l + 1]) {
            l--;
          }
        }
      }
    }
  }
  return result;
}

}  // namespace hash_table::four_sum

#if ENABLE_TESTS

TEST_CASE("hash_table::four_sum::simple")
{
  using namespace hash_table::four_sum;
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<table>>([&](auto table) {
    SUBCASE(table.name);

    std::vector<int> n0{ 1, 0, -1, 0, -2, 2 };
    auto r0 = simple<table.value>(n0, 0);
    std::sort(r0.begin(), r0.end());
    REQUIRE(r0.size() == 3);
    REQUIRE(r0[0] == std::vector<int>{ -2, -1, 1, 2 });
    REQUIRE(r0[1] == std::vector<int>{ -2, 0, 0, 2 });
    REQUIRE(r0[2] == std::vector<int>{ -1, 0, 0, 1 });

    std::vector<int> n1(20, 2);
    auto r1 = simple<table.value>(n1, 8);
    REQUIRE(r1.size() == 1);
    REQUIRE(r1[0] == std::vector<int>{ 2, 2, 2, 2 });
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using hash_table::four_sum::table;

template <table Table>
void hash_table_four_sum_simple(benchmark::State& state)
{
  std::vector<int> nums(100, 2);
  for (auto _ : state) {
    auto result = hash_table::four_sum::simple<Table>(nums, 8);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::boost_flat_set);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::boost_unordered_flat_set);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::boost_hash);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::boost_hash_combine);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::std_unordered_set_hash);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::std_unordered_set_hash_mulx);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::std_set);
BENCHMARK_TEMPLATE(hash_table_four_sum_simple, table::optimized);

#endif  // ENABLE_BENCHMARKS
