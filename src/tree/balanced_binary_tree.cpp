// https://leetcode.com/problems/balanced-binary-tree/
#include <common.hpp>
#include <algorithm>

namespace tree::balanced_binary_tree {

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

int balanced(TreeNode* root) noexcept
{
  if (!root) {
    return 0;
  }
  const auto lhs = balanced(root->left);
  const auto rhs = balanced(root->right);
  if (lhs < 0 || rhs < 0 || std::abs(lhs - rhs) > 1) {
    return -1;
  }
  return std::max(lhs, rhs) + 1;
}

bool run(TreeNode* root) noexcept
{
  return balanced(root) >= 0;
}

}  // namespace tree::balanced_binary_tree

#if ENABLE_TESTS

#include <boost/container/static_vector.hpp>

TEST_CASE("tree::balanced_binary_tree::run")
{
  using namespace tree::balanced_binary_tree;
  boost::container::static_vector<TreeNode, 16> nodes;

  auto r0 = &nodes.emplace_back(3);
  r0->left = &nodes.emplace_back(9);
  r0->right = &nodes.emplace_back(20);
  r0->right->left = &nodes.emplace_back(15);
  r0->right->right = &nodes.emplace_back(7);

  auto r1 = &nodes.emplace_back(1);
  r1->left = &nodes.emplace_back(2);
  r1->right = &nodes.emplace_back(2);
  r1->left->left = &nodes.emplace_back(3);
  r1->left->right = &nodes.emplace_back(3);
  r1->left->left->left = &nodes.emplace_back(4);
  r1->left->left->right = &nodes.emplace_back(4);

  REQUIRE(run(r0));
  REQUIRE(!run(r1));
  REQUIRE(run(nullptr));
}

#endif  // ENABLE_TESTS
