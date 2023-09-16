#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <set>
using std::cin, std::cout, std::vector;

enum class Color { WHITE, GRAY, BLACK };

class DSU {
 public:
  DSU(int n) : _size(n + 1, 1), _parent(n + 1, -1) {}

  bool IsSimilar(int u, int v) {
    return GetRoot(u) == GetRoot(v);
  }

  void Unite(int u, int v) {
    u = GetRoot(u);
    v = GetRoot(v);
    if (u == v) {
      return;
    }
    if (_size[u] > _size[v]) {
      std::swap(u, v);
    }
    _parent[u] = v;
    _size[u] += _size[v];
  }

 private:
  vector<int> _size;
  vector<int> _parent;

  int GetRoot(int v) {
    if (_parent[v] == -1) {
      if (_parent[v] == v) {
        cout << v << '\n';
      }
      return v;
    }
    int root = GetRoot(_parent[v]);
    _parent[v] = root;
    return root;
  }

};

class LCA {
 private:
  const vector<std::set<int>>& g;
  vector<vector<int>> lifts;
  vector<int> t_in, t_out, len;
  vector<Color> color;
  int n;
  int timer = 0;

  int Log(int k) {
    int ans = 0;
    while (k > 0) {
      ++ans;
      k = k / 2;
    }
    return ans + 1;
  }

  void Dfs(int v) {
    t_in[v] = timer++;
    color[v] = Color::GRAY;
    for (auto to : g[v]) {
      if (color[to] == Color::WHITE) {
        len[to] = len[v] + 1;
        lifts[0][to] = v;
        Dfs(to);
      }
    }
    color[v] = Color::BLACK;
    t_out[v] = timer++;
  }

  bool Ancestor(int x, int y) const {
    return (t_in[x] <= t_in[y]) && (t_out[x] >= t_out[y]);
  }

  void Prep() {
    for (size_t k = 0; k < lifts.size() - 1; ++k) {
      for (int v = 0; v < n; ++v) {
        lifts[k + 1][v] = lifts[k][lifts[k][v]];
      }
    }
  }

 public:

  explicit LCA(int n, const vector<std::set<int>>& graph) :
      g(graph), lifts(Log(n), vector<int>(n, 0)), t_in(n),
      t_out(n), len(n, 0), color(n, Color::WHITE), n(n) {}

  void SetVertex(int v) {
    lifts[0][v] = v;
    Dfs(v);
    Prep();
  }

  int GetLen(int v) const {
    return len[v];
  }

  int GetLCA(int u, int v) const {
    if (Ancestor(u, v)) {
      return u;
    }
    for (int k = lifts.size(); k > 0; --k) {
      if (!Ancestor(lifts[k - 1][u], v)) {
        u = lifts[k - 1][u];
      }
    }
    return lifts[0][u];
  }
};

class GraphManager {
 private:
  int n;
  const vector<vector<int>>& g;
  vector<vector<int>> without_bridges;
  vector<bool> visited;
  vector<int> color;
  vector<int> tin, tout, ret;
  int timer = 0;
  int col = 0;
  std::set<std::pair<int, int>> bridges;

 public:
  GraphManager(int n, const vector<vector<int>>& g) : n(n), g(g), without_bridges(n),
                                                      visited(n, false), color(n, -1),
                                                      tin(n), tout(n), ret(n) {}

  void DFS(int v, int parent = -1) {
    visited[v] = true;
    tin[v] = ret[v] = timer++;
    for (int to : g[v]) {
      if (to == parent) {
        continue;
      }
      if (visited[to]) {
        ret[v] = std::min(ret[v], tin[to]);
      } else {
        DFS(to, v);
        ret[v] = std::min(ret[v], ret[to]);
        if (ret[to] == tin[to]) {
          // (v, to) is bridge
          bridges.insert(std::make_pair(v, to));
          bridges.insert(std::make_pair(to, v));
        }
      }
    }
    tout[v] = timer++;
  }

  void FindBridges() {
    for (int v = 0; v < n; ++v) {
      if (!visited[v]) {
        DFS(v);
      }
    }
  }

  void ShowBridge() {
    for (const auto& p : bridges) {
      cout << p.first+ 1 << ' ' << p.second + 1 << '\n';
    }
    cout << '\n';
  }

  void DFSPaint(int v, int c, int parent = -1) {
    color[v] = c;
    for (int to : g[v]) {
      if (to == parent) {
        continue;
      }
      if (bridges.count(std::make_pair(v, to))) {
        continue;
      }
//      cout << "color[v] = " << color[to] << ' ' << v +1<< ' ' << to+1 << '\n';
      if (color[to] == -1) {
        DFSPaint(to, c, v);
      }
    }
  }

  void Paint() {
    for (int v = 0; v < n; ++v) {
      if (color[v] == -1) {
        DFSPaint(v, col++);
      }
    }
  }

  vector<int>& GetColor() {
    return color;
  }

  int getCol() {
    return col;
  }
};

int main() {
  int n, m, f, q = 0;
  cin >> n >> m >> f;
  --f;
  vector<vector<int>> g(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    --a;
    --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  GraphManager manager(n, g);
  manager.FindBridges();
  manager.Paint();
  const vector<int>& color = manager.GetColor();
  int new_n = manager.getCol();
  vector<std::set<int>> new_g(new_n);
  for (int u = 0; u < g.size(); ++u) {
    for (int v : g[u]) {
      if (color[u] == color[v]) {
        continue;
      }
      new_g[color[u]].insert(color[v]);
    }
  }
  LCA lca(new_n, new_g);
  lca.SetVertex(color[f]);
  cin >> q;
  for (int t = 0; t < q; ++t) {
    int a, b;
    cin >> a >> b;
    --a;
    --b;
    int lca_ab = lca.GetLCA(color[a], color[b]);
    cout << lca.GetLen(lca_ab) << '\n';
  }
}
