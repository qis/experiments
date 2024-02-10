// https://leetcode.com/problems/merge-k-sorted-lists/
#include <algorithm>
#include <utility>
#include <vector>

namespace algorithms::merge_k_sorted_lists {

// Definition for singly-linked list.
struct list_node {
  int value{ 0 };
  list_node* next{ nullptr };
};

list_node* simple(std::vector<list_node*>& lists)
{
  // Returns the node with the lowest value and advances the linked list of that node in the 'lists' variable.
  auto advance_min_element = [&lists]() noexcept -> list_node* {
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

}  // namespace algorithms::merge_k_sorted_lists

#include <boost/container/static_vector.hpp>
#include <common.hpp>

TEST_CASE("algorithms::merge_k_sorted_lists::merge")
{
  using algorithms::merge_k_sorted_lists::list_node;

  boost::container::static_vector<list_node, 8> data;
  std::vector<list_node*> lists;

  auto node = lists.emplace_back(&data.emplace_back(1));
  node = node->next = &data.emplace_back(4);
  node = node->next = &data.emplace_back(5);

  node = lists.emplace_back(&data.emplace_back(1));
  node = node->next = &data.emplace_back(3);
  node = node->next = &data.emplace_back(4);

  node = lists.emplace_back(&data.emplace_back(2));
  node = node->next = &data.emplace_back(6);

  auto list = algorithms::merge_k_sorted_lists::simple(lists);
  for (auto v : { 1, 1, 2, 3, 4, 4, 5, 6 }) {
    REQUIRE(list);
    REQUIRE(list->value == v);
    list = list->next;
  }
}
