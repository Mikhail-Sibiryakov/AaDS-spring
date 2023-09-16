#include <iostream>
#include <vector>
#include <string>
using std::cin, std::cout, std::vector;

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

enum Action {
  ask,
  unite
};

struct Request {
  Action action;
  int u;
  int v;
};

void Solve(vector<Request>& list, DSU& dsu) {
  vector<std::string> ans;
  for (size_t i = list.size(); i >= 1; --i) {
    if (list[i - 1].action == ask) {
      dsu.IsSimilar(list[i - 1].u, list[i - 1].v) ? ans.emplace_back("YES") :
               ans.emplace_back("NO");
    }
    if (list[i - 1].action == unite) {
      dsu.Unite(list[i - 1].u, list[i - 1].v);
    }
  }
  for (size_t i = ans.size(); i >= 1; --i) {
    cout << ans[i - 1] << '\n';
  }
}

int main() {
  int n, m, k;
  cin >> n >> m >> k;
  for (int i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;
  }
  vector<Request> list(k);
  for (int i = 0; i < k; ++i) {
    std::string s;
    int a, b;
    cin >> s >> a >> b;
    list[i].action = ask;
    if (s == "cut") {
      list[i].action = unite;
    }
    list[i].u = a;
    list[i].v = b;
  }
  DSU dsu(n);
  Solve(list, dsu);
}
