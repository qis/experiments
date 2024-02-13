// https://leetcode.com/problems/add-two-numbers/
// ---------------------------------------------------------------------------------------
// Benchmark                                             Time             CPU   Iterations
// ---------------------------------------------------------------------------------------
// linked_lists_add_two_numbers_simple/90/90          2869 ns         2790 ns       224000
// linked_lists_add_two_numbers_simple/90/40          2703 ns         2637 ns       248889
// linked_lists_add_two_numbers_simple/40/90          2717 ns         2699 ns       248889
// linked_lists_add_two_numbers_optimized/90/90        159 ns          157 ns      4480000
// linked_lists_add_two_numbers_optimized/90/40        164 ns          160 ns      4480000
// linked_lists_add_two_numbers_optimized/40/90        164 ns          160 ns      4480000

#include <boost/noncopyable.hpp>
#include <common.hpp>
#include <memory>
#include <random>
#include <utility>
#include <vector>

namespace linked_lists::add_two_numbers {

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

// NOTE: The caller is responsible for deleting the returned nodes.
list_node* simple(list_node* l1, list_node* l2) noexcept
{
  list_node* head = nullptr;
  list_node* next = nullptr;
  auto value = 0;
  while (l1 || l2) {
    if (l1) {
      value += l1->value;
      l1 = l1->next;
    }
    if (l2) {
      value += l2->value;
      l2 = l2->next;
    }
    if (next) {
      next->next = new list_node(value % 10, nullptr);
      next = next->next;
    } else {
      head = new list_node(value % 10, nullptr);
      next = head;
    }
    if (value >= 10) {
      value /= 10;
    } else {
      value = 0;
    }
  }
  if (value > 0) {
    next->next = new list_node(value, nullptr);
  }
  return head;
}

list_node* optimized(list_node* l1, list_node* l2) noexcept
{
  auto s1 = l1;
  auto s2 = l2;
  const auto recycle = [&s1, &s2]() noexcept {
    if (s1) {
      return std::exchange(s1, s1->next);
    }
    return std::exchange(s2, s2->next);
  };
  list_node* head = nullptr;
  list_node* next = nullptr;
  auto value = 0;
  while (l1 || l2) {
    if (l1) {
      value += l1->value;
      l1 = l1->next;
    }
    if (l2) {
      value += l2->value;
      l2 = l2->next;
    }
    if (next) {
      next->next = recycle();
      next = next->next;
    } else {
      head = recycle();
      next = head;
    }
    next->value = value % 10;
    if (value >= 10) {
      value /= 10;
    } else {
      value = 0;
    }
  }
  if (value > 0) {
    next->next = recycle();
    next = next->next;
    next->value = value;
  }
  if (next) {
    next->next = nullptr;
  }
  return head;
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

inline auto benchmark(std::size_t s1, std::size_t s2)
{
  static thread_local std::random_device rd;
  static thread_local std::uniform_int_distribution<int> dist(0, 9);
  std::vector<int> v1;
  v1.reserve(s1);
  for (std::size_t i = 0; i < s1; i++) {
    v1.push_back(dist(rd));
  }
  std::vector<int> v2;
  v2.reserve(s2);
  for (std::size_t i = 0; i < s2; i++) {
    v2.push_back(dist(rd));
  }
  struct benchmark_data {
    std::unique_ptr<container> l1;
    std::unique_ptr<container> l2;
  };
  return benchmark_data{ create(v1), create(v2) };
}

auto test()
{
  struct test_data {
    std::unique_ptr<container> l1{ create({ 2, 4, 3 }) };
    std::unique_ptr<container> l2{ create({ 5, 6, 4 }) };
    std::unique_ptr<container> l3{ create({ 7, 0, 8 }) };
  } data;
  return data;
}

}  // namespace linked_lists::add_two_numbers

#if ENABLE_TESTS

TEST_CASE("linked_lists::add_two_numbers::simple")
{
  const auto data = linked_lists::add_two_numbers::test();
  auto n1 = data.l1->head;
  auto n2 = data.l2->head;
  auto n3 = data.l3->head;
  auto head = linked_lists::add_two_numbers::simple(n1, n2);
  REQUIRE(head);
  for (auto node = head; node; node = node->next) {
    REQUIRE(n3);
    REQUIRE(node->value == n3->value);
    n3 = n3->next;
  }
}

TEST_CASE("linked_lists::add_two_numbers::optimized")
{
  const auto data = linked_lists::add_two_numbers::test();
  auto n1 = data.l1->head;
  auto n2 = data.l2->head;
  auto n3 = data.l3->head;
  auto head = linked_lists::add_two_numbers::optimized(n1, n2);
  REQUIRE(head);
  for (auto node = head; node; node = node->next) {
    REQUIRE(n3);
    REQUIRE(node->value == n3->value);
    n3 = n3->next;
  }
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

static void linked_lists_add_two_numbers_simple(benchmark::State& state)
{
  const auto s1 = static_cast<std::size_t>(state.range(0));
  const auto s2 = static_cast<std::size_t>(state.range(1));
  const auto data = linked_lists::add_two_numbers::benchmark(s1, s2);
  for (auto _ : state) {
    auto result = linked_lists::add_two_numbers::simple(data.l1->head, data.l2->head);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(linked_lists_add_two_numbers_simple)
  ->Args({ 90, 90 })
  ->Args({ 90, 40 })
  ->Args({ 40, 90 });
// clang-format on

static void linked_lists_add_two_numbers_optimized(benchmark::State& state)
{
  const auto s1 = static_cast<std::size_t>(state.range(0));
  const auto s2 = static_cast<std::size_t>(state.range(1));
  const auto data = linked_lists::add_two_numbers::benchmark(s1, s2);
  for (auto _ : state) {
    auto result = linked_lists::add_two_numbers::optimized(data.l1->head, data.l2->head);
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(linked_lists_add_two_numbers_optimized)
  ->Args({ 90, 90 })
  ->Args({ 90, 40 })
  ->Args({ 40, 90 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
