#include <algorithm>
#include <cstdio>
#include <numeric>
#include <string>
#include <vector>

static constexpr int kInf = std::numeric_limits<int>::max() / 2;

struct Edge {
  int from = 0;
  int to = 0;
  int cost = 0;
};

// 返回值为true表示存在负环
// Bellman Ford 都是基于动态规划，其原始的状态定义为 f[i][k] 代表从起点到 i 点，且经过最多 k 条边的最短路径。
// 我们也可以这样定义f[k][i]，应该是等价的
// 这样的状态定义引导我们能够使用 Bellman Ford 来解决有边数限制的最短路问题。
bool DPBellmanFord(int n, int src, const std::vector<Edge>& edges, std::vector<std::vector<int>>& dis) {
  dis = std::vector<std::vector<int>>(n, std::vector<int>(n, kInf));
  // 初始状态，dis[src][0] = 0，其他dis[i][0](i!=src)为kInf
  dis[src][0] = 0;
  // flag代表某一次遍历是否发生了松弛
  bool flag = false;
  // 最多执行n-1次松弛
  // 当第n次还可以松弛时，表示从src可以到达一个负环
  for (int i = 1; i <= n; ++i) {
    flag = false;
    for (auto& edge : edges) {
      dis[edge.to][i] = dis[edge.to][i - 1];
      if (dis[edge.to][i] > dis[edge.from][i - 1] + edge.cost) {
        dis[edge.to][i] = dis[edge.from][i - 1] + edge.cost;
        flag = true;
      }
    }
    if (!flag) {
      break;
    }
  }
  return flag;
}

// 返回值为true表示存在负环
// 上述的状态转移，只与上一维度有关系，可以尝试将上一个维度去掉
bool BellmanFord(int n, int src, const std::vector<Edge>& edges, std::vector<int>& dis) {
  std::vector<std::vector<Edge>> adjs(n);
  dis = std::vector<int>(n, kInf);
  for (auto& edge : edges) {
    adjs[edge.from].emplace_back(edge);
  }
  dis[src] = 0;
  bool flag = false;
  for (int i = 1; i <= n; ++i) {
    flag = false;
    for (int u = 0; u < n; ++u) {
      for (auto& edge : adjs[u]) {
        // 将上一个维度去掉后, 这一轮松弛可能出现所使用的边大于i条
        // 例如对于边u->v,使用的dis[u]不能保证是上一轮进行的松弛
        // 因为可能存在边a->u已经在边u->v之前更新过dis[u]了
        // 因此其使用的边可能会大于i条
        // 如果需要求解有限制的最短路问题，可以使用原始的动态规划定义
        // 保证每一轮的边数一定满足需求，但是可以使用滚动数组来降低空间复杂度
        if (dis[edge.from] + edge.cost < dis[edge.to]) {
          dis[edge.to] = dis[edge.from] + edge.cost;
          flag = true;
        }
      }
    }
    if (!flag) {
      break;
    }
  }
  return flag;
}

int main(int argc, char* argv[]) { return 0; }