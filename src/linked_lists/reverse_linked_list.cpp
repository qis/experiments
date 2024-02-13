// https://leetcode.com/problems/reverse-linked-list/
// ---------------------------------------------------------------------------------------
// Benchmark                                             Time             CPU   Iterations
// ---------------------------------------------------------------------------------------
// linked_lists_reverse_linked_list_simple/100         0.854 ns        0.816 ns    746666667
// linked_lists_reverse_linked_list_simple/100000      0.853 ns        0.837 ns    746666667
// linked_lists_reverse_linked_list_slist/100          0.641 ns        0.641 ns   1000000000
// linked_lists_reverse_linked_list_slist/100000       0.644 ns        0.656 ns   1000000000

#include <boost/intrusive/linear_slist_algorithms.hpp>
#include <boost/noncopyable.hpp>
#include <common.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <cassert>

namespace linked_lists::reverse_linked_list {

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

list_node* simple(list_node* head) noexcept
{
  list_node* result = nullptr;
  while (head) {
    const auto next = std::exchange(head->next, result);
    result = std::exchange(head, next);
  }
  return result;
}

list_node* slist(list_node* head) noexcept
{
  struct list_traits {
    using node = list_node;
    using node_ptr = node*;
    using const_node_ptr = const node_ptr;

    static node_ptr get_next(const_node_ptr n) noexcept
    {
      return n->next;
    }

    static void set_next(node_ptr n, node_ptr next) noexcept
    {
      n->next = next;
    }
  };

  using list = boost::intrusive::linear_slist_algorithms<list_traits>;
  return list::reverse(head);
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

struct container : boost::noncopyable {
  std::vector<list_node> data;
  list_node* head{ nullptr };
};

inline auto create(const std::vector<int>& values)
{
  auto ptr = std::make_unique<container>();
  ptr->data.reserve(values.size());
  if (auto it = values.begin(); it != values.end()) {
    ptr->head = &ptr->data.emplace_back(*it, nullptr);
    auto node = ptr->head;
    while (++it != values.end()) {
      node->next = &ptr->data.emplace_back(*it, nullptr);
      node = node->next;
    }
  }
  return ptr;
}

inline auto benchmark(std::size_t size)
{
  std::vector<int> values;
  values.reserve(size);
  for (auto i = 0; i < static_cast<int>(size); i++) {
    values.push_back(i);
  }
  return create(values);
}

}  // namespace linked_lists::reverse_linked_list

#if ENABLE_TESTS

TEST_CASE("linked_lists::reverse_linked_list::simple")
{
  auto src = linked_lists::reverse_linked_list::create({ 1, 2, 3, 4, 5 });
  auto dst = linked_lists::reverse_linked_list::simple(src->head);
  for (auto i : { 5, 4, 3, 2, 1 }) {
    REQUIRE(dst);
    REQUIRE(dst->value == i);
    dst = dst->next;
  }
  REQUIRE(!dst);
}

TEST_CASE("linked_lists::reverse_linked_list::slist")
{
  auto src = linked_lists::reverse_linked_list::create({ 1, 2, 3, 4, 5 });
  auto dst = linked_lists::reverse_linked_list::slist(src->head);
  for (auto i : { 5, 4, 3, 2, 1 }) {
    REQUIRE(dst);
    REQUIRE(dst->value == i);
    dst = dst->next;
  }
  REQUIRE(!dst);
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void linked_lists_reverse_linked_list_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto data = linked_lists::reverse_linked_list::benchmark(size);
  for (auto _ : state) {
    auto result = linked_lists::reverse_linked_list::simple(data->head);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(linked_lists_reverse_linked_list_simple)
  ->Args({ 100 })
  ->Args({ 100'000 });
// clang-format on

static void linked_lists_reverse_linked_list_slist(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto data = linked_lists::reverse_linked_list::benchmark(size);
  for (auto _ : state) {
    auto result = linked_lists::reverse_linked_list::slist(data->head);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(linked_lists_reverse_linked_list_slist)
  ->Args({ 100 })
  ->Args({ 100'000 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
