#include <algorithm>
#include <iostream>
#include <vector>
using std::cin, std::cout, std::vector;


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
  int n, k;
  cin >> n >> k;
  Kuhn kuhn(n, k);
  for (int r = 0; r < n; ++r) {
    while (true) {
      int v;
      cin >> v;
      if (v == 0) {
        break;
      }
      kuhn.g[r].push_back(v - 1);
    }
  }
  kuhn.AlgKyhn();
  int cnt = 0;
  for (int i : kuhn.match) {
    if (i != -1) {
      ++cnt;
    }
  }
  cout << cnt << '\n';
  for (int i = 0; i < kuhn.match.size(); ++i) {
    if (kuhn.match[i] != -1) {
      cout << kuhn.match[i] + 1 << ' ' << i + 1 << '\n';
    }
  }
}
