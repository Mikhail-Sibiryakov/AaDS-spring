#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
using std::cin, std::cout, std::vector;

struct Edge {
  int from;
  int to;
  int64_t flow;
  int64_t cap;

  Edge(int u, int v, int cap) : from(u), to(v), flow(0), cap(cap) {}

  int64_t GetResidualCap() const {
    return cap - flow;
  }
};

class Dinic {
 public:
  Dinic(int n, int s, int t) : s(s), t(t), graph(n), dist(n),
                               ptr(n, 0), used(n, false) {}

  void insert_edge(int u, int v, int cap) {
    graph[u].push_back(static_cast<int>(edges.size()));
    edges.emplace_back(u, v, cap);
    graph[v].push_back(static_cast<int>(edges.size()));
    edges.emplace_back(v, u, 0);
  }

  int64_t GetMaxFlow() {
    int64_t ans = 0;
    while (true) {
      if (!BFS()) {
        break;
      }
      ptr.assign(ptr.size(), 0);
      while (true) {
        int64_t tmp = DFS(s, INF);
        if (tmp == 0) {
          break;
        }
        ans += tmp;
      }
    }
    return ans;
  }

  vector<Edge>& GetEdges() {
    return edges;
  }

  void MinCut() {
    int64_t cost = 0;
    int cnt = 0;
    vector<int> ans;
    DFS(s);
    for (size_t i = 0; i < used.size(); ++i) {
      if (!used[i]) {
        continue;
      }
//      cout << i + 1 << ' ';
      for (auto id : graph[i]) {
        if (!used[edges[id].to]) {
          if (id % 2 == 1) {
            continue;
          }
          ++cnt;
          cost += edges[id].flow;
//          cout << edges[id].from + 1 << ' ' << edges[id].to + 1 << ' ' << (id / 2) + 1 << '\n';
          ans.push_back(id / 4);
        }
      }
    }
//    cout << '\n';
    cout << cnt << ' ' << cost << '\n';
    std::sort(ans.begin(), ans.end());
    for (auto i : ans) {
      cout << i + 1 << ' ';
    }
  }

 private:
  static const int INF = 1000000000 * 2;
  int s, t;
  vector<Edge> edges;
  vector<vector<int>> graph;
  std::queue<int> q;
  vector<int> dist;
  vector<size_t> ptr;
  vector<int> vertices;
  vector<bool> used;

  bool BFS() {
    dist.assign(dist.size(), -1);
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int i : graph[v]) {
        const Edge& kEdge = edges[i];
        int to = kEdge.to;
        if (dist[to] == -1 && kEdge.GetResidualCap() > 0) {
          q.push(to);
          dist[to] = dist[v] + 1;
        }
      }
    }
    return dist[t] != -1;
  }

  int64_t DFS(int v, int64_t flow) {
    if (flow == 0 || v == t) {
      return flow;
    }
    while (ptr[v] < graph[v].size()) {
      int i = graph[v][ptr[v]];
      Edge& edge = edges[i];
      int to = edge.to;
      if (dist[to] != dist[v] + 1 || edge.GetResidualCap() == 0) {
        ++ptr[v];
        continue;
      }
      int64_t delta = DFS(to, std::min(flow, edge.GetResidualCap()));
      if (delta == 0) {
        ++ptr[v];
        continue;
      }
      edge.flow += delta;
      edges[i ^ 1].flow -= delta;
      return delta;
    }
    return 0;
  }

  void DFS(int v) {
    used[v] = true;
    for (int i : graph[v]) {
      int to = edges[i].to;
      if (!used[to] && edges[i].GetResidualCap() > 0) {
        DFS(to);
      }
    }
  }
};

int main() {
  int n, m;
  cin >> n >> m;
  Dinic dinic(n, 0, n - 1);
  for (int i = 0; i < m; ++i) {
    int u, v, cap;
    cin >> u >> v >> cap;
    dinic.insert_edge(u - 1, v - 1, cap);
    dinic.insert_edge(v - 1, u - 1, cap);
  }
  dinic.GetMaxFlow();
  dinic.MinCut();
}
