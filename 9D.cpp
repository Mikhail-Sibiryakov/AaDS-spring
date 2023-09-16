#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
using std::cin, std::cout, std::vector;

enum class Color { WHITE, GRAY, BLACK };

vector<vector<int>> lifts;
vector<vector<int>> g;
int n;
vector<int> t_in, t_out, len;
vector<Color> color;
int timer = 0;
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

int Log(int k) {
  int ans = 0;
  while (k > 0) {
    ++ans;
    k = k / 2;
  }
  return ans + 1;
}

void Init() {
  g.resize(n);
  lifts.resize(Log(n));
  for (auto& arr : lifts) {
    arr.resize(n, 0);
  }
  t_in.resize(n);
  t_out.resize(n);
  color.resize(n, Color::WHITE);
  len.resize(n, 0);
}

bool Ancestor(int x, int y) {
  return (t_in[x] <= t_in[y]) && (t_out[x] >= t_out[y]);
}

void Prep() {
  for (size_t k = 0; k < lifts.size() - 1; ++k) {
    for (int v = 0; v < n; ++v) {
      lifts[k + 1][v] = lifts[k][lifts[k][v]];
    }
  }
}

int LCA(int u, int v) {
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

int main() {
  cin >> n;
  Init();
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;
    g[u - 1].push_back(v - 1);
    g[v - 1].push_back(u - 1);
  }
  Dfs(0);
  Prep();
  int q;
  cin >> q;
  for (int t = 0; t < q; ++t) {
    int a, b;
    cin >> a >> b;
    --a;
    --b;
    int tmp = LCA(a, b);
    cout << abs(len[a] - len[tmp]) + abs(len[b] - len[tmp]) << '\n';
  }
}
