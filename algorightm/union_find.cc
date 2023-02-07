#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>

class UnionFind {
 public:
  UnionFind(int n) : parents_(n) {
    std::iota(parents_.begin(), parents_.end(), 0);
  }

  int Find(int x) {
    return parents_[x] == x ? x : parents_[x] = Find(parents_[x]);
  }

  void Union(int x, int y) {
    parents_[Find(y)] = Find(x);
  }

 private:
  std::vector<int> parents_;
};

int main(int argc, char* argv[]) {
  return 0;
}