#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
using std::cin, std::cout, std::vector;

class DSU {
 public:
  DSU(int n, vector<int>& cost) :
      _size(n, 1), _parent(n, -1), _cheapest(n) {
    for (size_t i = 0; i < cost.size(); ++i) {
      _cheapest[i] = cost[i];
    }
  }

  bool IsSimilar(int u, int v) {
    return GetRoot(u) == GetRoot(v);
  }

  int GetCost(int u) {
    return _cheapest[GetRoot(u)];
  }

  int Unite(int u, int v) {
    u = GetRoot(u);
    v = GetRoot(v);
    if (u == v) {
      return 0;
    }
    if (_size[u] > _size[v]) {
      std::swap(u, v);
    }
    _parent[u] = v;
    _size[u] += _size[v];
    int tmp = std::max(_cheapest[u], _cheapest[v]);
    _cheapest[u] = std::min(_cheapest[u], _cheapest[v]);
    _cheapest[v] = _cheapest[u];
    return tmp;
  }

 private:
  vector<int> _size;
  vector<int> _parent;
  vector<int> _cheapest;

  int GetRoot(int v) {
    if (_parent[v] == -1) {
      return v;
    }
    int root = GetRoot(_parent[v]);
    _parent[v] = root;
    return root;
  }

};

struct Edge {
  int u;
  int v;
  int cost;

  bool operator<(Edge other) const {
    return cost < other.cost;
  }
};

void Solve(int n, vector<Edge>& edges, vector<int>& cost, int s) {
  int ans = s;
  std::sort(edges.begin(), edges.end());
  DSU dsu(n, cost);
  vector<Edge> min_ost;
  for (auto edge : edges) {
    if (!dsu.IsSimilar(edge.u, edge.v) &&
        std::max(dsu.GetCost(edge.u), dsu.GetCost(edge.v)) >= edge.cost) {
      ans -= dsu.Unite(edge.u, edge.v);
      min_ost.push_back(edge);
//      ans -= std::max(cost[edge.u], cost[edge.v]);
      ans += edge.cost;
//      int tmp = std::min(cost[edge.u], cost[edge.v]);
//      cost[edge.u] = tmp;
//      cost[edge.v] = tmp;
    }
  }
  cout << ans << '\n';
}

int main() {
  int n;
  cin >> n;
  vector<Edge> edges;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int cost;
      cin >> cost;
      if (i >= j) {
        continue;
      }
      edges.push_back({i, j, cost});
    }
  }
  vector<int> cost(n);
  int s = 0;
  for (int i = 0; i < n; ++i) {
    cin >> cost[i];
    s += cost[i];
  }

  Solve(n, edges, cost, s);
}
