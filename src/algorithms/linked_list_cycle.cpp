// https://leetcode.com/problems/linked-list-cycle/
// ---------------------------------------------------------------------------------------------
// Benchmark                                                   Time             CPU   Iterations
// ---------------------------------------------------------------------------------------------
// algorithms_linked_list_cycle_simple/80000/0         447161400 ns    453125000 ns            2
// algorithms_linked_list_cycle_simple/80000/2000      447263750 ns    453125000 ns            2
// algorithms_linked_list_cycle_simple/80000/60000     454168750 ns    460937500 ns            2
// algorithms_linked_list_cycle_optimized/80000/0          71147 ns        69754 ns        11200
// algorithms_linked_list_cycle_optimized/80000/2000      139324 ns       134969 ns         4978
// algorithms_linked_list_cycle_optimized/80000/60000     104003 ns       100446 ns         5600

#include <boost/container/static_vector.hpp>
#include <boost/intrusive/linear_slist_algorithms.hpp>
#include <boost/noncopyable.hpp>
#include <common.hpp>
#include <algorithm>
#include <memory>
#include <vector>

namespace algorithms::linked_list_cycle {

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

bool simple(list_node* head)
{
  std::vector<list_node*> nodes;
  for (auto node = head; node; node = node->next) {
    if (std::find(nodes.begin(), nodes.end(), node) != nodes.end()) {
      return true;
    }
    nodes.emplace_back(node);
  }
  return false;
}

bool optimized(list_node* head) noexcept
{
  if (!head) {
    return false;
  }
  auto fast = head;
  auto slow = head;
  while (fast && fast->next) {
    fast = fast->next->next;
    slow = slow->next;
    if (fast == slow) {
      return true;
    }
  }
  return false;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

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

struct container : boost::noncopyable {
  std::vector<list::node> data;
  list::node_ptr head{ nullptr };
};

using list_ptr = std::unique_ptr<container>;

inline list_ptr create(const std::vector<int>& values)
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

inline auto test()
{
  struct test_data {
    list_ptr v4 = create({ 3, 2, 0, -4 });
    list_ptr v1 = create({ 1 });
    list_ptr i4 = create({ 3, 2, 0, -4 });
    list_ptr i2 = create({ 1, 2 });
    list_ptr i1 = create({ 1 });
  } data;
  data.i4->head->next->next->next->next = data.i4->head->next;
  data.i2->head->next->next = data.i4->head;
  data.i1->head->next = data.i4->head;
  return data;
}

inline auto benchmark(std::size_t size, std::size_t node)
{
  std::vector<int> values;
  values.reserve(size);
  for (std::size_t i = 0; i < size; i++) {
    values.push_back(static_cast<int>(i));
  }
  auto data = create(values);
  if (node > 0 && node <= size) {
    auto last = data->head;
    auto target = data->head;
    std::size_t index = 0;
    do {
      if (index == node) {
        target = last;
      }
      index++;
      last = last->next;
    } while (last->next);
    last->next = target;
  }
  return data;
}

}  // namespace algorithms::linked_list_cycle

#if ENABLE_TESTS

TEST_CASE("algorithms::linked_list_cycle::simple")
{
  const auto data = algorithms::linked_list_cycle::test();
  REQUIRE(!algorithms::linked_list_cycle::simple(data.v4->head));
  REQUIRE(!algorithms::linked_list_cycle::simple(data.v1->head));
  REQUIRE(algorithms::linked_list_cycle::simple(data.i4->head));
  REQUIRE(algorithms::linked_list_cycle::simple(data.i2->head));
  REQUIRE(algorithms::linked_list_cycle::simple(data.i1->head));
}

TEST_CASE("algorithms::linked_list_cycle::optimized")
{
  const auto data = algorithms::linked_list_cycle::test();
  REQUIRE(!algorithms::linked_list_cycle::optimized(data.v4->head));
  REQUIRE(!algorithms::linked_list_cycle::optimized(data.v1->head));
  REQUIRE(algorithms::linked_list_cycle::optimized(data.i4->head));
  REQUIRE(algorithms::linked_list_cycle::optimized(data.i2->head));
  REQUIRE(algorithms::linked_list_cycle::optimized(data.i1->head));
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void algorithms_linked_list_cycle_simple(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto node = static_cast<std::size_t>(state.range(1));
  const auto data = algorithms::linked_list_cycle::benchmark(size, node);
  for (auto _ : state) {
    auto result = algorithms::linked_list_cycle::simple(data->head);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_linked_list_cycle_simple)
  ->Args({ 80'000, 0 })
  ->Args({ 80'000, 2'000 })
  ->Args({ 80'000, 60'000 });
// clang-format on

static void algorithms_linked_list_cycle_optimized(benchmark::State& state)
{
  const auto size = static_cast<std::size_t>(state.range(0));
  const auto node = static_cast<std::size_t>(state.range(1));
  const auto data = algorithms::linked_list_cycle::benchmark(size, node);
  for (auto _ : state) {
    auto result = algorithms::linked_list_cycle::optimized(data->head);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_linked_list_cycle_optimized)
  ->Args({ 80'000, 0 })
  ->Args({ 80'000, 2'000 })
  ->Args({ 80'000, 60'000 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
