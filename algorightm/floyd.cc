#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>

static constexpr int kInf = std::numeric_limits<int>::max() / 2;

struct Edge {
  int from = 0;
  int to = 0;
  int cost = 0;
};

void Floyd(int n, const std::vector<Edge>& edges, std::vector<std::vector<int>>& dis) {
  dis = std::vector<std::vector<int>>(n, std::vector<int>(n, kInf));
  for (auto& edge : edges) {
    dis[edge.from][edge.to] = edge.cost;
  }
  for (int i = 0; i < n; ++i) {
    dis[i][i] = 0;
  }
  for (int k = 0; k < n; ++k) {
    for (int u = 0; u < n; ++u) {
      for (int v = 0; v < n; ++v) {
        // dis[k][u][v] = std::min(dis[dis - 1][u][v], dis[k - 1][u][k] + dis[k - 1][k][v]);
        dis[u][v] = std::min(dis[u][v], dis[u][k] + dis[k][v]);
      }
    }
  }
}

int main(int argc, char* argv[]) {
  return 0;
}