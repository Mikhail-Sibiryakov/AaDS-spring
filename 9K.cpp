#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
using std::cin, std::cout, std::vector;

struct Edge {
  int from;
  int to;
  int flow;
  int cap;
  Edge* reverse_edge;

  Edge(int u, int v, int cap) : from(u), to(v), flow(0), cap(cap),
                                reverse_edge(nullptr) {}

  int GetResidualCap() const {
    return cap - flow;
  }
};

class FordFulkerson {
 public:
  FordFulkerson(int n, int s, int t) : s(s), t(t), cur_color(1), graph(n), color(n, 0) {}

  void insert_edge(int u, int v, int cap) {
    graph[u].emplace_back(u, v, cap);
    graph[v].emplace_back(v, u, 0);
    graph[u].back().reverse_edge = &graph[v].back();
    graph[v].back().reverse_edge = &graph[u].back();
  }

  int GetMaxFlow() {
    while (true) {
      int tmp = DFS(s, INF);
      if (tmp == 0) {
        break;
      }
      ++cur_color;
    }
    int ans = 0;
    for (Edge& edge : graph[s]) {
      ans += edge.flow;
    }
    return ans;
  }

 private:
  static const int INF = 1000000000 * 2;
  int s, t, cur_color;
  std::deque<std::deque<Edge>> graph;
  vector<int> color;

  int DFS(int v, int min_cap) {
    if (v == t) {
      return min_cap;
    }
    color[v] = cur_color;
    for (Edge& edge : graph[v]) {
      if (edge.GetResidualCap() == 0 || color[edge.to] == cur_color) {
        continue;
      }
      int delta = DFS(edge.to, std::min(min_cap, edge.GetResidualCap()));
      if (delta > 0) {
        edge.flow += delta;
        edge.reverse_edge->flow -= delta;
        return delta;
      }
    }
    return 0;
  }
};

int main() {
  int n, m;
  cin >> n >> m;
  FordFulkerson ff(n, 0, n - 1);
  for (int i = 0; i < m; ++i) {
    int u, v, cap;
    cin >> u >> v >> cap;
    ff.insert_edge(u - 1, v - 1, cap);
  }
  cout << ff.GetMaxFlow() << '\n';
}
