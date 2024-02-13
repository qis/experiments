// https://leetcode.com/problems/merge-k-sorted-lists/
// ----------------------------------------------------------------------------------------------------------------
// Benchmark                                                                      Time             CPU   Iterations
// ----------------------------------------------------------------------------------------------------------------
// linked_lists_merge_k_sorted_lists_simple/16/8000/iterations:8            5338875 ns      5859375 ns            8
// linked_lists_merge_k_sorted_lists_simple/8000/16/iterations:8         2028185525 ns   1960937500 ns            8
// linked_lists_merge_k_sorted_lists_simple_tbb/16/8000/iterations:8      106614162 ns     54687500 ns            8
// linked_lists_merge_k_sorted_lists_simple_tbb/8000/16/iterations:8      828158013 ns    351562500 ns            8
// linked_lists_merge_k_sorted_lists_successive/16/8000/iterations:8        1991712 ns      5859375 ns            8
// linked_lists_merge_k_sorted_lists_successive/8000/16/iterations:8     2972353687 ns   2859375000 ns            8
// linked_lists_merge_k_sorted_lists_cheat/16/8000/iterations:8             2414363 ns      1953125 ns            8
// linked_lists_merge_k_sorted_lists_cheat/8000/16/iterations:8             1987888 ns      1953125 ns            8

#include <boost/container/static_vector.hpp>
#include <boost/core/noncopyable.hpp>
#include <common.hpp>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_sort.h>
#include <algorithm>
#include <array>
#include <memory>
#include <random>
#include <thread>
#include <utility>
#include <vector>

namespace linked_lists::merge_k_sorted_lists {

// =====================================================================================================================
// Definitions
// =====================================================================================================================

struct list_node {
  int value{ 0 };
  list_node* next{ nullptr };
};

// =====================================================================================================================
// Solutions
// =====================================================================================================================

list_node* simple(std::vector<list_node*>& lists)
{
  // Returns the node with the lowest value and advances the linked list of that node in the 'lists' variable.
  const auto advance_min_element = [&lists]() noexcept -> list_node* {
    // Note: This can be called with std::execution::par_unseq for large 'lists' sizes.
    auto it = std::min_element(lists.begin(), lists.end(), [](const auto& lhs, const auto& rhs) noexcept {
      if (lhs) {
        if (rhs) {
          return lhs->value < rhs->value;
        }
        return true;
      }
      return false;
    });
    if (it == lists.end() || !*it) {
      return nullptr;
    }
    return std::exchange(*it, (*it)->next);
  };

  // Get first node.
  auto beg = advance_min_element();
  if (!beg) {
    return nullptr;
  }

  // Append the node with the lowest value as long as there are nodes left in the 'lists' variable.
  for (auto end = beg; end; end = end->next) {
    end->next = advance_min_element();
  }

  return beg;
}

list_node* simple_tbb(std::vector<list_node*>& lists, std::size_t grain = 1)
{
  using iterator = list_node**;

  // Parallel version of std::min_element.
  const auto min_element = [grain](iterator begin, iterator end) noexcept -> iterator {
    struct searcher {
      iterator result{ nullptr };
      constexpr searcher() = default;
      searcher(searcher& s, tbb::split) noexcept
      {
        result = nullptr;
      }
      void operator()(const tbb::blocked_range<iterator>& sub) noexcept
      {
        auto it = std::min_element(sub.begin(), sub.end(), less);
        if (it != sub.end() && (!result || less(*it, *result))) {
          result = it;
        }
      }
      void join(searcher& rhs) noexcept
      {
        if (!result || (rhs.result && less(*rhs.result, *result))) {
          result = rhs.result;
        }
      }
      static bool less(const list_node* lhs, const list_node* rhs) noexcept
      {
        if (lhs) {
          if (rhs) {
            return lhs->value < rhs->value;
          }
          return true;
        }
        return false;
      }
    } s;
    tbb::parallel_reduce(tbb::blocked_range(begin, end, grain), s);
    return s.result ? s.result : end;
  };

  // Returns the node with the lowest value and advances the linked list of that node in the 'lists' variable.
  const auto lists_begin = lists.data();
  const auto lists_end = lists_begin + lists.size();
  const auto advance_min_element = [&lists, &min_element, lists_begin, lists_end]() noexcept -> list_node* {
    auto it = min_element(lists_begin, lists_end);
    if (it == lists_end || !*it) {
      return nullptr;
    }
    return std::exchange(*it, (*it)->next);
  };

  // Get first node.
  auto beg = advance_min_element();
  if (!beg) {
    return nullptr;
  }

  // Append the node with the lowest value as long as there are nodes left in the 'lists' variable.
  for (auto end = beg; end; end = end->next) {
    end->next = advance_min_element();
  }

  return beg;
}

list_node* successive(std::vector<list_node*>& lists)
{
  list_node* beg = nullptr;
  for (auto src : lists) {
    // Check if the first value in src is smaller, than the first value in beg, then swap.
    if (beg) {
      if (src) {
        if (src->value < beg->value) {
          std::swap(beg, src);
        }
      } else {
        continue;
      }
    } else {
      std::swap(beg, src);
      continue;
    }

    // At this point, beg and src are valid and beg <= src is guaranteed. Merge src into beg.
    for (auto dst = beg; src;) {
      if (dst->next) {
        // dst is not the last beg node.
        if (src->value < dst->next->value) {
          // src should replace dst->next.
          //
          //   const auto src_next = src->next;
          //   const auto dst_next = dst->next;
          //   dst->next = src;
          //   dst->next->next = dst_next;
          //   src = src_next;
          //
          dst->next = std::exchange(src, std::exchange(src->next, dst->next));
        } else {
          // src should not replace dst->next.
          dst = dst->next;
        }
      } else {
        // dst is the last beg note, so src can just be appended to it.
        dst->next = src;
        break;
      }
    }
  }
  return beg;
}

list_node* cheat(std::vector<list_node*>& lists)
{
  using iterator = std::vector<list_node*>::iterator;

  // Get first valid list.
  constexpr auto get = [](iterator it, iterator end) noexcept {
    return std::find_if(it, end, [](const auto& e) noexcept {
      return e != nullptr;
    });
  };

  const auto end = lists.end();
  auto it = get(lists.begin(), end);
  if (it == end) {
    return nullptr;
  }

  // Copy values of the first list.
  std::vector<int> values;
  values.reserve(lists.size());

  const auto result = *it;
  values.push_back(result->value);

  auto last = result;
  while (last->next) {
    last = last->next;
    values.push_back(last->value);
  }

  // Copy values and concatenate lists.
  for (it = get(std::next(it), end); it != end; it = get(std::next(it), end)) {
    last->next = *it;
    while (last->next) {
      last = last->next;
      values.push_back(last->value);
    }
  }

  // Sort and assign values.
  // std::sort(std::execution::par_unseq, values.begin(), values.end());
  tbb::parallel_sort(values.begin(), values.end());
  auto node = result;
  for (auto value : values) {
    node->value = value;
    node = node->next;
  }

  return result;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

auto test()
{
  struct test_data : boost::noncopyable {
    boost::container::static_vector<list_node, 8> data;
    std::vector<list_node*> lists;
    std::array<int, 8> compare;
  };
  auto p = std::make_unique<test_data>();
  auto n = p->lists.emplace_back(&p->data.emplace_back(1));
  n = n->next = &p->data.emplace_back(4);
  n = n->next = &p->data.emplace_back(5);
  n = p->lists.emplace_back(&p->data.emplace_back(1));
  n = n->next = &p->data.emplace_back(3);
  n = n->next = &p->data.emplace_back(4);
  n = p->lists.emplace_back(&p->data.emplace_back(2));
  n = n->next = &p->data.emplace_back(6);
  p->compare = { 1, 1, 2, 3, 4, 4, 5, 6 };
  return p;
}

template <std::size_t Size>
auto benchmark(std::size_t lists, std::size_t length)
{
  struct benchmark_input : boost::noncopyable {
    benchmark_input(std::size_t size) : data(size) {}
    std::vector<list_node> data;
    std::vector<list_node*> lists;
  };
  const auto generate = [lists, length]() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 5);
    auto p = std::make_unique<benchmark_input>(lists * length);
    std::size_t data_index = 0;
    for (std::size_t i = 0; i < lists; i++) {
      auto node = p->lists.emplace_back(&p->data[data_index++]);
      node->value = dist(rd);
      for (std::size_t j = 1; j < length; j++) {
        node->next = &p->data[data_index++];
        node->next->value = node->value + dist(rd);
        node = node->next;
      }
    }
    return p;
  };
  std::array<std::unique_ptr<benchmark_input>, Size> inputs;
  tbb::parallel_for(
    tbb::blocked_range(inputs.begin(), inputs.end()),
    [generate](const auto& range) {
      for (auto it = range.begin(); it != range.end(); ++it) {
        *it = generate();
      }
    },
    tbb::simple_partitioner());
  for (auto& e : inputs) {
    e = generate();
  }
  return inputs;
}

}  // namespace linked_lists::merge_k_sorted_lists

#if ENABLE_TESTS

TEST_CASE("linked_lists::merge_k_sorted_lists::simple")
{
  auto data = linked_lists::merge_k_sorted_lists::test();
  auto list = linked_lists::merge_k_sorted_lists::simple(data->lists);
  for (auto v : data->compare) {
    REQUIRE(list);
    REQUIRE(list->value == v);
    list = list->next;
  }
}

TEST_CASE("linked_lists::merge_k_sorted_lists::tbb")
{
  auto data = linked_lists::merge_k_sorted_lists::test();
  auto list = linked_lists::merge_k_sorted_lists::simple_tbb(data->lists);
  for (auto v : data->compare) {
    REQUIRE(list);
    REQUIRE(list->value == v);
    list = list->next;
  }
}

TEST_CASE("linked_lists::merge_k_sorted_lists::successive")
{
  auto data = linked_lists::merge_k_sorted_lists::test();
  auto list = linked_lists::merge_k_sorted_lists::successive(data->lists);
  for (auto v : data->compare) {
    REQUIRE(list);
    REQUIRE(list->value == v);
    list = list->next;
  }
}

TEST_CASE("linked_lists::merge_k_sorted_lists::cheat")
{
  auto data = linked_lists::merge_k_sorted_lists::test();
  auto list = linked_lists::merge_k_sorted_lists::cheat(data->lists);
  for (auto v : data->compare) {
    REQUIRE(list);
    REQUIRE(list->value == v);
    list = list->next;
  }
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void linked_lists_merge_k_sorted_lists_simple(benchmark::State& state)
{
  const auto lists = static_cast<std::size_t>(state.range(0));
  const auto length = static_cast<std::size_t>(state.range(1));
  const auto data = linked_lists::merge_k_sorted_lists::benchmark<8>(lists, length);
  auto input = data.begin();
  for (auto _ : state) {
    auto result = linked_lists::merge_k_sorted_lists::simple((*input)->lists);
    benchmark::DoNotOptimize(result);
    input = std::next(input);
  }
}
BENCHMARK(linked_lists_merge_k_sorted_lists_simple)->Args({ 16, 8'000 })->Args({ 8'000, 16 })->Iterations(8);

static void linked_lists_merge_k_sorted_lists_simple_tbb(benchmark::State& state)
{
  const auto lists = static_cast<std::size_t>(state.range(0));
  const auto length = static_cast<std::size_t>(state.range(1));
  const auto grain = lists / std::thread::hardware_concurrency() * 2;
  const auto data = linked_lists::merge_k_sorted_lists::benchmark<8>(lists, length);
  auto input = data.begin();
  for (auto _ : state) {
    auto result = linked_lists::merge_k_sorted_lists::simple_tbb((*input)->lists, grain);
    benchmark::DoNotOptimize(result);
    input = std::next(input);
  }
}
BENCHMARK(linked_lists_merge_k_sorted_lists_simple_tbb)->Args({ 16, 8'000 })->Args({ 8'000, 16 })->Iterations(8);

static void linked_lists_merge_k_sorted_lists_successive(benchmark::State& state)
{
  const auto lists = static_cast<std::size_t>(state.range(0));
  const auto length = static_cast<std::size_t>(state.range(1));
  const auto data = linked_lists::merge_k_sorted_lists::benchmark<8>(lists, length);
  auto input = data.begin();
  for (auto _ : state) {
    auto result = linked_lists::merge_k_sorted_lists::successive((*input)->lists);
    benchmark::DoNotOptimize(result);
    input = std::next(input);
  }
}
BENCHMARK(linked_lists_merge_k_sorted_lists_successive)->Args({ 16, 8'000 })->Args({ 8'000, 16 })->Iterations(8);

static void linked_lists_merge_k_sorted_lists_cheat(benchmark::State& state)
{
  const auto lists = static_cast<std::size_t>(state.range(0));
  const auto length = static_cast<std::size_t>(state.range(1));
  const auto data = linked_lists::merge_k_sorted_lists::benchmark<8>(lists, length);
  auto input = data.begin();
  for (auto _ : state) {
    auto result = linked_lists::merge_k_sorted_lists::cheat((*input)->lists);
    benchmark::DoNotOptimize(result);
    input = std::next(input);
  }
}
BENCHMARK(linked_lists_merge_k_sorted_lists_cheat)->Args({ 16, 8'000 })->Args({ 8'000, 16 })->Iterations(8);

#endif  // ENABLE_BENCHMARKS
