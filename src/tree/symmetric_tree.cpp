// https://leetcode.com/problems/symmetric-tree/
#include <common.hpp>
#include <string>
#include <vector>

namespace tree::symmetric_tree {

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

bool is_mirror(TreeNode* lhs, TreeNode* rhs) noexcept
{
  if (lhs == rhs) {
    return true;
  }
  if (!lhs || !rhs) {
    return false;
  }
  return lhs->val == rhs->val && is_mirror(lhs->left, rhs->right) && is_mirror(lhs->right, rhs->left);
}

bool run(TreeNode* root) noexcept
{
  return root && is_mirror(root->left, root->right);
}

}  // namespace tree::symmetric_tree

#if ENABLE_TESTS

#include <boost/container/static_vector.hpp>

TEST_CASE("tree::symmetric_tree::run")
{
  using namespace tree::symmetric_tree;
  boost::container::static_vector<TreeNode, 12> nodes;

  auto r0 = &nodes.emplace_back(1);
  r0->left = &nodes.emplace_back(2);
  r0->left->left = &nodes.emplace_back(3);
  r0->left->right = &nodes.emplace_back(4);
  r0->right = &nodes.emplace_back(2);
  r0->right->left = &nodes.emplace_back(4);
  r0->right->right = &nodes.emplace_back(3);

  auto r1 = &nodes.emplace_back(1);
  r1->left = &nodes.emplace_back(2);
  r1->left->right = &nodes.emplace_back(3);
  r1->right = &nodes.emplace_back(2);
  r1->right->right = &nodes.emplace_back(3);

  REQUIRE(run(r0));
  REQUIRE(!run(r1));
}

#endif  // ENABLE_TESTS
