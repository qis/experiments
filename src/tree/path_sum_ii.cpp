// https://leetcode.com/problems/path-sum-ii/
#include <common.hpp>
#include <vector>

namespace tree::path_sum_ii {

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

void solve(std::vector<std::vector<int>>& result, std::vector<int>& current, TreeNode* root, int target, bool first)
{
  if (!root) {
    return;
  }
  current.push_back(root->val);
  target -= root->val;
  if (target == 0 && !first) {
    result.push_back(current);
  } else {
    solve(result, current, root->left, target, false);
    solve(result, current, root->right, target, false);
  }
  current.pop_back();
}

std::vector<std::vector<int>> run(TreeNode* root, int target)
{
  std::vector<std::vector<int>> result;
  std::vector<int> current;
  solve(result, current, root, target, true);
  return result;
}

}  // namespace tree::path_sum_ii

#if ENABLE_TESTS

#include <boost/container/static_vector.hpp>
#include <sstream>

std::string format(const auto& result)
{
  std::ostringstream oss;
  oss << '[';
  for (const auto& row : result) {
    oss << '[';
    const auto beg = row.cbegin();
    const auto end = row.cend();
    for (auto it = beg; it != end; ++it) {
      if (it != beg) {
        oss << ',';
      }
      oss << *it;
    }
    oss << ']';
  }
  oss << ']';
  return oss.str();
}

TEST_CASE("tree::path_sum_ii::run")
{
  using namespace tree::path_sum_ii;
  boost::container::static_vector<TreeNode, 20> nodes;

  auto r0 = &nodes.emplace_back(5);
  r0->left = &nodes.emplace_back(4);
  r0->left->left = &nodes.emplace_back(11);
  r0->left->left->left = &nodes.emplace_back(7);
  r0->left->left->right = &nodes.emplace_back(2);
  r0->right = &nodes.emplace_back(8);
  r0->right->left = &nodes.emplace_back(13);
  r0->right->right = &nodes.emplace_back(4);
  r0->right->right->left = &nodes.emplace_back(5);
  r0->right->right->right = &nodes.emplace_back(1);

  auto r1 = &nodes.emplace_back(1);
  r1->left = &nodes.emplace_back(2);
  r1->right = &nodes.emplace_back(3);

  auto r2 = &nodes.emplace_back(1);
  r2->left = &nodes.emplace_back(2);

  auto r3 = &nodes.emplace_back(1);
  r3->left = &nodes.emplace_back(2);
  r3->left->left = &nodes.emplace_back(3);
  r3->left->left->left = &nodes.emplace_back(4);
  r3->left->left->left->left = &nodes.emplace_back(5);

  REQUIRE(format(run(r0, 22)) == "[[5,4,11,2],[5,8,4,5]]");
  REQUIRE(format(run(r1, 5)) == "[]");
  REQUIRE(format(run(r2, 0)) == "[]");

  // NOTE: No idea why this is the expected result!
  REQUIRE(format(run(r3, 6)) == "[]");
}

#endif  // ENABLE_TESTS
