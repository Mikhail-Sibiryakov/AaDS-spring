#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
using std::cin, std::cout, std::vector;

void DFS(int v, const vector<std::vector<int>>& graph, vector<bool>& used) {
  used[v] = true;
  for (int to : graph[v]) {
    if (!used[to]) {
      DFS(to, graph, used);
    }
  }
}

class Kuhn {
 public:
  vector<bool> used;
  vector<int> match;
  vector<vector<int>> g;

  explicit Kuhn(int n, int k) : used(n), match(k, -1), g(n) {}

  bool augment(int v) {
    if (used[v]) {
      return false;
    }
    used[v] = true;
    for (int to : g[v]) {
      if (match[to] == -1 || augment(match[to])) {
        match[to] = v;
        return true;
      }
    }
    return false;
  }

  void AlgKyhn() {
    used.assign(used.size(), false);
    for (int v = 0; v < used.size(); ++v) {
      if (augment(v)) {
        used.assign(used.size(), false);
      }
    }
  }

};

int main() {
  int n, m;
  cin >> n >> m;
  Kuhn kuhn(n, n);
  vector<vector<int>>& g = kuhn.g;
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    --u;
    --v;
    g[u].push_back(v);
  }
//  for (int i = 0; i < n; ++i) {
//    g[i].push_back(i);
//  }
  kuhn.AlgKyhn();
  vector<vector<int>> new_g(n);
  vector<bool> in_match(n, false);
  int ans = 0;
  for (int i = 0; i < kuhn.match.size(); ++i) {
    if (kuhn.match[i] != -1) {
//      cout << kuhn.match[i] + 1 << ' ' << i + 1 << '\n';
      new_g[kuhn.match[i]].push_back(i);
      new_g[i].push_back(kuhn.match[i]);
      in_match[i] = in_match[kuhn.match[i]] = true;
    }
  }

//  for (int i = 0; i <n; ++i) {
//    cout << i + 1 << ": ";
//    for (int j : new_g[i]) {
//      cout << j + 1<< ' ';
//    }
//    cout << '\n';
//  }

  for (bool b : in_match) {
    if (!b) {
      ++ans;
    }
  }

  int num_comp = 0;
  vector<bool> used(n, false);
  for (int v = 0; v < n; ++v) {
    if (new_g[v].empty()) {
      continue;
    }
    if (!used[v]) {
//      cout << "v = " << v + 1 << '\n';
      ++num_comp;
      DFS(v, new_g, used);
    }
  }
  cout << ans + num_comp << '\n';
}
