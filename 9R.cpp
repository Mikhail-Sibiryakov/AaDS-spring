#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using std::cin, std::cout, std::vector;

const int INF = 2'000'000'000;

template<typename T>
void Print(const T& v) {
  for (const auto& item : v) {
    cout << item << ' ';
  }
  cout << '\n';
}

class Alg {
 public:

  // Обязательно проинициализировать графы оба
  Alg(int n) : n(n), matrix(n, vector<int>(n)), g(n, vector<int>(n)), used_left(n, true),
               used_right(n, false), used(n, false), potential_row(n, 0),
               potential_column(n, 0), match(n, -1),
               match_lr(n, -1) {}

  vector<vector<int>>& GetGraph() {
    return matrix;
  }

  vector<vector<int>>& GetG() {
    return g;
  }

  void Solve_2() {
    for (int vert = 0; vert < n; ++vert) {
      int v = vert;
      vector<bool> l_vis(n, false), r_vis(n, false);
      vector<std::pair<int, int>> m(n);
      for (int j = 0; j < n; ++j) {
        m[j] = std::make_pair(vert, j);
      }
      vector<int> way(n, -1);
      while (true) {
        l_vis[v] = true;
        int delta = INF;
        int from = -1, to = -1;
        for (int j = 0; j < n; ++j) {
          if (r_vis[j]) {
            continue;
          }
          if (Get(m[j].first, m[j].second) < delta) {
            delta = Get(m[j].first, m[j].second);
            from = m[j].first;
            to = m[j].second;
          }
        }

        for (int j = 0; j < n; ++j) {
          if (l_vis[j]) {
            AddRow(j, -delta);
          }
          if (r_vis[j]) {
            AddCol(j, delta);
          }
        }
        r_vis[to] = true;

        way[to] = from;
//        cout << v + 1 << ' ';
//        cout << vert + 1 << ", new strong edge: " << from + 1 << ' ' << to + 1 << '\n';
        if (match[to] == -1) {
          // Мы нашли увеличивающий путь с концом в to; и его надо развернуть
          int a = from;
          int b = to;
          while (true) {
            int old_m = match[b];
            int old_mlr = match_lr[a];
            match[b] = a;
            match_lr[a] = b;
            if (old_mlr == -1) {
              break;
            }
            b = old_mlr;
            a = way[b];
          }

//          int a = from;
//          int b = way[0];
//          for (int k = 0; k < way.size(); ++k) {
//            int old = match[b];
//            match[b] = a;
//            if (old == -1) {
//              break;
//            }
////            cout << k + 1 << ' ' << way.size() << " <-\n";
//            b = way[k + 1];
//            a = old;
//          }
//          PrintMatch();
          break;
        } else {
          l_vis[match[to]] = true;
//          cout << "visited " << match[to] + 1 << '\n';
//          Print(r_vis);
//          Print(m);
//          v = match[to];
          for (int j = 0; j < n; ++j) {
//            m[j] = std::min(m[j], Get(match[to], j));
            if (Get(m[j].first, m[j].second) > Get(match[to], j)) {
              m[j] = std::make_pair(match[to], j);
            }
          }
//          PrinGraph();
        }
      }
    }
  }

  void PrintMatch() {
    cout << "---------------------------\n";
    for (int i = 0; i < n; ++i) {
      if (match[i] != -1)
        cout << match[i] + 1 << ' ' << i + 1 << '\n';
    }
    cout << "---------------------------\n";
  }

  void PrinGraph() {
    cout << "**************************\n";
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        cout << Get(i, j) << ' ';
      }
      cout << '\n';
    }
    cout << "**************************\n";
  }

  void Solve() {
    vector<int> m(n);
    for (int j = 0; j < n; ++j) {
      m[j] = matrix[0][j];
    }

    while (SizePar() < n) {
//      cout << "AlgK\n";
      AlgKyhn();

      int delta = INF;
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (used_left[i] && !used_right[j]) {
            delta = std::min(delta, matrix[i][j]);
          }
        }
      }

//      cout << "delta = " << delta << '\n';

      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (used_left[i]) {
            matrix[i][j] -= delta;
            if (matrix[i][j] == 0 && delta != 0) {
              cout << "new strong edge: " << i + 1 << ' ' << j + 1 << '\n';
            }
          }
          if (used_right[j]) {
            matrix[i][j] += delta;
          }
        }
      }

    }
  }

  void Show() {
//    for (int b : match) {
//      cout << b << ' ';
//    }
//    cout << '\n';

    int s = 0;
    for (int i = 0; i < n; ++i) {
      if (match[i] != -1) {
        s += g[match[i]][i];
      }
    }
    cout << s << '\n';
    for (int i = 0; i < n; ++i) {
      if (match[i] != -1) {
        cout << match[i] + 1 << ' ' << i + 1 << '\n';
      }
    }
  }

 private:
  int n;
  vector<vector<int>> matrix, g;
  vector<bool> used_left, used_right, used;
  vector<int> potential_row, potential_column;
  vector<int> match, match_lr;
//  vector<std::set<int>> g;

  void AddRow(int i, int delta) {
    potential_row[i] += delta;
  }

  void AddCol(int j, int delta) {
    potential_column[j] += delta;
  }

  int Get(int i, int j) {
    return matrix[i][j] + potential_row[i] + potential_column[j];
  }

  int SizePar() {
    int ans = 0;
    for (int i : match) {
      if (i != -1) {
        ++ans;
      }
    }
//    cout << "ans = " << ans << '\n';
    return ans;
  }

  void DFS(int v) {
    used_left[v] = true;
    for (int to = 0; to < n; ++to) {
      if (Get(v, to) != 0) {
        continue;
      }
      used_right[to] = true;
      if (match[to] == -1) {
        continue;
      }
//      cout << "match[to] = " << match[to] << '\n';
      if (!used_left[match[to]]) {
        DFS(match[to]);
      }
    }
  }

  bool augment(int v) {
    if (used[v]) {
      return false;
    }
    used[v] = true;
    for (int to = 0; to < n; ++to) {
      if (Get(v, to) != 0) {
        continue;
      }
      if (match[to] == -1 || augment(match[to])) {
        match[to] = v;
        return true;
      }
    }
    return false;
  }

  void AlgKyhn() {
    used_left.assign(used_left.size(), false);
    used_right.assign(used_right.size(), false);
    used.assign(used.size(), false);
    match.assign(match.size(), -1);
    match_lr.assign(match_lr.size(), -1);
    for (int v = 0; v < used.size(); ++v) {
      if (augment(v)) {
        used.assign(used.size(), false);
      }
    }
    for (int i = 0; i < n; ++i) {
      if (match[i] != -1) {
        match_lr[match[i]] = i;
      }
    }

//    cout << "old used_left:\n";
//    for (bool b : used_left) {
//      cout << b << ' ';
//    }
//    cout << '\n';
//
//    cout << "old used_right:\n";
//    for (bool b : used_right) {
//      cout << b << ' ';
//    }
//    cout << '\n';

    PaintVert();

//    cout << "new used_left:\n";
//    for (bool b : used_left) {
//      cout << b << ' ';
//    }
//    cout << '\n';
//
//    cout << "new used_right:\n";
//    for (bool b : used_right) {
//      cout << b << ' ';
//    }
//    cout << '\n';
  }

  void PaintVert() {
    used_left.assign(used_left.size(), false);
    used_right.assign(used_right.size(), false);
    for (int i = 0; i < n; ++i) {
      if (match_lr[i] == -1) {
        DFS(i);
      }
    }
  }
};

int main() {
  int n;
  cin >> n;
  Alg alg(n);
  vector<vector<int>>& g = alg.GetGraph();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cin >> g[i][j];
      alg.GetG()[i][j] = g[i][j];
    }
  }
  alg.Solve_2();
//  alg.Solve();
  alg.Show();
}
