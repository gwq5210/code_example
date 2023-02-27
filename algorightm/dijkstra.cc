/*
 * @lc app=leetcode.cn id=743 lang=cpp
 *
 * [743] 网络延迟时间
 *
 * https://leetcode.cn/problems/network-delay-time/description/
 *
 * algorithms
 * Medium (55.29%)
 * Likes:    627
 * Dislikes: 0
 * Total Accepted:    99K
 * Total Submissions: 179.1K
 * Testcase Example:  '[[2,1,1],[2,3,1],[3,4,1]]\n4\n2'
 *
 * 有 n 个网络节点，标记为 1 到 n。
 *
 * 给你一个列表 times，表示信号经过 有向 边的传递时间。 times[i] = (ui, vi, wi)，其中 ui 是源节点，vi 是目标节点，
 * wi 是一个信号从源节点传递到目标节点的时间。
 *
 * 现在，从某个节点 K 发出一个信号。需要多久才能使所有节点都收到信号？如果不能使所有节点收到信号，返回 -1 。
 *
 *
 *
 * 示例 1：
 *
 *
 *
 *
 * 输入：times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
 * 输出：2
 *
 *
 * 示例 2：
 *
 *
 * 输入：times = [[1,2,1]], n = 2, k = 1
 * 输出：1
 *
 *
 * 示例 3：
 *
 *
 * 输入：times = [[1,2,1]], n = 2, k = 2
 * 输出：-1
 *
 *
 *
 *
 * 提示：
 *
 *
 * 1 <= k <= n <= 100
 * 1 <= times.length <= 6000
 * times[i].length == 3
 * 1 <= ui, vi <= n
 * ui != vi
 * 0 <= wi <= 100
 * 所有 (ui, vi) 对都 互不相同（即，不含重复边）
 *
 *
 */

#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Solution {
 public:
  static constexpr int kInf = std::numeric_limits<int>::max() / 2;
  int Floyd(std::vector<std::vector<int>>& times, int n, int k) {
    std::vector<std::vector<int>> dis(n, std::vector<int>(n, kInf));
    for (int i = 0; i < n; ++i) {
      dis[i][i] = 0;
    }
    for (auto& time : times) {
      dis[time[0] - 1][time[1] - 1] = time[2];
    }
    int result = 0;
    for (int k = 0; k < n; ++k) {
      for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
          dis[u][v] = std::min(dis[u][v], dis[u][k] + dis[k][v]);
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      result = std::max(result, dis[k - 1][i]);
    }
    return result == kInf ? -1 : result;
  }
  int BellmanFord(std::vector<std::vector<int>>& times, int n, int k) {
    std::vector<int> dis(n, kInf);
    dis[k - 1] = 0;
    bool flag = false;
    for (int i = 1; i < n; ++i) {
      for (auto& time : times) {
        int from = time[0] - 1;
        int to = time[1] - 1;
        int cost = time[2];
        if (dis[from] + cost < dis[to]) {
          dis[to] = dis[from] + cost;
          flag = true;
        }
      }
      if (!flag) {
        break;
      }
    }
    int result = 0;
    for (int i = 0; i < n; ++i) {
      result = std::max(result, dis[i]);
    }
    return result == kInf ? -1 : result;
  }
  int Dijkstra(std::vector<std::vector<int>>& times, int n, int k) {
    std::vector<std::vector<std::pair<int, int>>> adjs(n);
    for (auto& time : times) {
      adjs[time[0] - 1].emplace_back(std::make_pair(time[2], time[1] - 1));
    }
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    std::vector<int> vis(n);
    std::vector<int> dis(n, kInf);

    int result = 0;
    dis[k - 1] = 0;
    pq.push(std::make_pair(0, k - 1));
    int count = 0;
    while (!pq.empty()) {
      auto [cost, u] = pq.top();
      pq.pop();
      if (vis[u]) {
        continue;
      }
      vis[u] = true;
      ++count;
      result = std::max(result, cost);
      for (auto [w, v] : adjs[u]) {
        if (dis[u] + w < dis[v]) {
          dis[v] = dis[u] + w;
          pq.push(std::make_pair(dis[v], v));
        }
      }
    }
    return count < n || result == kInf ? -1 : result;
  }
  int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k) {
    // return Floyd(times, n, k);
    // return BellmanFord(times, n, k);
    return Dijkstra(times, n, k);
  }
};

int main(int argc, char* argv[]) { return 0; }