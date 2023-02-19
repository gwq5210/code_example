#include <algorithm>
#include <cstdio>
#include <numeric>
#include <string>
#include <vector>

template <typename T>
class SegTree {
 public:
  struct Node {
    T val = 0;
    T lazy = 0;
  };

  SegTree(const std::vector<int>& arr) : arr_(arr) {
    // 根节点区间范围是[0, end_]
    end_ = arr.size() - 1;
    // 线段树节点的数量是4*arr.size()
    nodes_.resize(arr.size() * 4);
    // 递归建树
    Build(0, end_, root_);
  }

  // 对 [l,r] 区间建立线段树,当前根的编号为 p
  void Build(int l, int r, int p) {
    // 如果区间数目为1，则直接更新树节点的值
    if (l == r) {
      nodes_[p].val = arr_[l];
      return;
    }

    // 如果写成 (l + r) / 2 可能会超出 int 范围
    int mid = l + (r - l) / 2;
    // 递归对左[l, mid]右[mid + 1, r]区间建树
    Build(l, mid, p * 2);
    Build(mid + 1, r, p * 2 + 1);
    // 从子节点更新当前节点p
    PushUp(p);
  }

  // 从子节点更新当前节点p
  void PushUp(int p) { nodes_[p].val = nodes_[p * 2].val + nodes_[p * 2 + 1].val; }
  // 如果当前节点 p 的懒标记非空,则更新当前节点两个子节点的值和懒标记值
  void PushDown(int pl, int pr, int p) {
    if (!nodes_[p].lazy) {
      return;
    }
    int mid = pl + (pr - pl) / 2;
    UpdateNode(pl, mid, p * 2, nodes_[p].lazy);
    UpdateNode(mid + 1, pr, p * 2 + 1, nodes_[p].lazy);
    nodes_[p].lazy = 0;
  }
  void UpdateNode(int pl, int pr, int p, T val) {
    nodes_[p].val += (pr - pl + 1) * val;
    nodes_[p].lazy += val;
  }

  // 返回区间[0, end_]的结果
  int Query() { return Query(0, end_, 0, end_, root_); }
  // 返回区间[l, r]的结果
  int Query(int l, int r) { return Query(l, r, 0, end_, root_); }

  // [l, r] 为查询区间, [pl, pr] 为当前节点包含的区间, p 为当前节点的编号
  int Query(int l, int r, int pl, int pr, int p) {
    // 当前区间为询问区间的子集时直接返回当前区间的结果
    if (pl >= l && pr <= r) {
      return nodes_[p].val;
    }

    //
    PushDown(pl, pr, p);

    // 递归求左右区间的结果
    int sum = 0;
    int mid = pl + (pr - pl) / 2;
    if (mid >= l) {
      // 如果左儿子代表的区间 [pl, mid] 与询问区间有交集, 则递归查询左儿子
      sum += Query(l, r, pl, mid, p * 2);
    }
    if (mid + 1 <= r) {
      // 如果右儿子代表的区间 [mid + 1, pr] 与询问区间有交集, 则递归查询右儿子
      sum += Query(l, r, mid + 1, pr, p * 2 + 1);
    }
    return sum;
  }

  // 修改区间[l, r]的值
  void Modify(int l, int r, int val) { Modify(l, r, val, 0, end_, root_); }
  // [l, r] 为修改区间, val 为被修改的元素的变化量
  // [pl, pr] 为当前节点包含的区间, p 为当前节点的编号
  void Modify(int l, int r, T val, int pl, int pr, int p) {
    if (pl >= l && pr <= r) {
      // 当前区间为修改区间的子集时直接修改当前节点的值,然后打标记,结束修改
      UpdateNode(pl, pr, p, val);
      return;
    }

    // 如果当前节点的懒标记非空,则更新当前节点两个子节点的值和懒标记值
    PushDown(pl, pr, p);

    // 递归更新左右区间的值
    int mid = pl + (pr - pl) / 2;
    if (mid >= l) {
      // 如果左儿子代表的区间 [pl, mid] 与更新区间有交集, 则递归更新左儿子
      Modify(l, r, val, pl, mid, p * 2);
    }
    if (mid + 1 <= r) {
      // 如果右儿子代表的区间 [mid + 1, pr] 与更新区间有交集, 则递归更新右儿子
      Modify(l, r, val, mid + 1, pr, p * 2 + 1);
    }

    // 使用左右子节点的值更新当前节点的值
    PushUp(p);
  }

  // 根节点，表示的范围为[0, end_]
  int root_ = 1;
  int end_ = 0;
  std::vector<Node> nodes_;
  const std::vector<T>& arr_;
};

int main(int argc, char* argv[]) {
  int n = 100;
  std::vector<int> arr(n);
  SegTree<int> seg_tree(arr);
  printf("sum = %d\n", seg_tree.Query());
  seg_tree.Modify(0, 10, 100);
  printf("sum = %d\n", seg_tree.Query());
  seg_tree.Modify(0, 1, 100);
  printf("sum = %d\n", seg_tree.Query());
  int l = 10;
  int r = n - 1;
  printf("sum(%d, %d) = %d\n", l, r, seg_tree.Query(l, r));
  return 0;
}