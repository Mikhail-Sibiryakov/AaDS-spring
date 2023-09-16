#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
using std::cin, std::cout, std::vector;

void DFS(int v, const vector<std::set<int>>& graph, vector<bool>& used) {
  used[v] = true;
  for (int to : graph[v]) {
    if (!used[to]) {
      DFS(to, graph, used);
    }
  }
}

int main() {
  int m, n;
  cin >> m >> n;
  vector<std::set<int>> g(m + n);
  vector<bool> saturated(m + n);
  for (int s = 0; s < m; ++s) {
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
      int v;
      cin >> v;
      g[s].insert(v - 1 + m);
    }
  }
  for (int i = 0; i < m; ++i) {
    int l;
    cin >> l;
    if (l == 0) {
      continue;
    }
    saturated[i] = true;
    saturated[l - 1 + m] = true;
    g[i].erase(l - 1 + m);
    g[l - 1 + m].insert(i);
  }
  
  vector<bool> used(n + m, false);
  for (int v = 0; v < m; ++v) {
    if (!used[v] && !saturated[v]) {
      DFS(v, g, used);
    }
  }

  vector<int> l_minus, r_plus;
  for (int i = 0; i < m + n; ++i) {
    if (i < m && !used[i]) {
      l_minus.push_back(i);
    } else if (i >= m && used[i]) {
      r_plus.push_back(i);
    }
  }

  cout << l_minus.size() + r_plus.size() << '\n';
  cout << l_minus.size() << ' ';
  for (int i : l_minus) {
    cout << i + 1 << ' ';
  }
  cout << '\n' << r_plus.size() << ' ';
  for (int i : r_plus) {
    cout << i + 1 - m << ' ';
  }
}
