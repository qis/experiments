// https://leetcode.com/problems/same-tree/
#include <common.hpp>
#include <vector>

namespace tree::same_tree {

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

bool run(TreeNode* p, TreeNode* q) noexcept
{
  if (p == q) {
    return true;
  }
  if (!p || !q) {
    return false;
  }
  return p->val == q->val && run(p->left, q->left) && run(p->right, q->right);
}

}  // namespace tree::same_tree

#if ENABLE_TESTS

#include <boost/container/static_vector.hpp>

TEST_CASE("tree::same_tree::run")
{
  using namespace tree::same_tree;
  boost::container::static_vector<TreeNode, 16> nodes;

  auto p0 = &nodes.emplace_back(1);
  p0->left = &nodes.emplace_back(2);
  p0->right = &nodes.emplace_back(3);
  auto q0 = &nodes.emplace_back(1);
  q0->left = &nodes.emplace_back(2);
  q0->right = &nodes.emplace_back(3);

  auto p1 = &nodes.emplace_back(1);
  p1->left = &nodes.emplace_back(2);
  auto q1 = &nodes.emplace_back(1);
  q1->right = &nodes.emplace_back(2);

  auto p2 = &nodes.emplace_back(1);
  p2->left = &nodes.emplace_back(2);
  p2->right = &nodes.emplace_back(1);
  auto q2 = &nodes.emplace_back(1);
  q2->left = &nodes.emplace_back(1);
  q2->right = &nodes.emplace_back(2);

  REQUIRE(run(p0, q0));
  REQUIRE(!run(p1, q1));
  REQUIRE(!run(p2, q2));
}

#endif  // ENABLE_TESTS
