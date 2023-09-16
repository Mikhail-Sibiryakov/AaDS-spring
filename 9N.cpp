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
  Dinic(int n, int s, int t) : s(s), t(t), graph(n), dist(n), ptr(n, 0) {}

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



 private:
  static const int INF = 1000000000 * 2;
  int s, t;
  vector<Edge> edges;
  vector<vector<int>> graph;
  std::queue<int> q;
  vector<int> dist;
  vector<size_t> ptr;

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
};

int main() {
  int n, m;
  cin >> n >> m;
  Dinic dinic(n, 0, n - 1);
  for (int i = 0; i < m; ++i) {
    int u, v, cap;
    cin >> u >> v >> cap;
    dinic.insert_edge(u - 1, v - 1, cap);
  }
  cout << dinic.GetMaxFlow() << '\n';
  for (size_t i = 0; i < dinic.GetEdges().size(); i += 2) {
    cout << dinic.GetEdges()[i].flow << '\n';
  }
}
