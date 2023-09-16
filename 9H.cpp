#include <algorithm>
#include <iostream>
#include <vector>
using std::cin, std::cout, std::vector;

class Kuhn {
 public:
  vector<bool> used;
  vector<int> match;
  vector<vector<int>> g;
  vector<bool> taken_left;
  vector<bool> taken_right;

  explicit Kuhn(int n, int k) : used(n), match(k, -1), g(n),
                                taken_left(n, false), taken_right(n, false) {}

  bool augment(int v) {
    if (used[v]) {
//      if (taken[v]) {
//        cout << v << " !\n";
//      }
      return false;
    }
    used[v] = true;
    for (int to : g[v]) {
//      if (taken_left[to]) {
//        continue;
//      }
      if (match[to] == -1 || augment(match[to])) {
        if (match[to] != -1) {
          taken_left[match[to]] = false;
        }
        match[to] = v;
        taken_left[v] = true;
        taken_right[to] = true;
//        if (taken[to]) {
//          cout << "to = " << to << '\n';
//        }
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

const int BROKEN = -1;

int main() {
  int m, n, a, b;
  cin >> m >> n >> a >> b;
  int num_vertex = 0;
  vector<vector<int>> board(m, vector<int>(n));
  for (int s = 0; s < m; ++s) {
    for (int i = 0; i < n; ++i) {
      char value;
      cin >> value;
      if (value == '.') {
        board[s][i] = BROKEN;
      } else {
        board[s][i] = num_vertex++;
      }
    }
  }
  Kuhn kuhn(num_vertex, num_vertex);
  for (int s = 0; s < m; ++s) {
    for (int i = 0; i < n; ++i) {
      int v = board[s][i];
      if (v == BROKEN) {
        continue;
      }
//      cout << s << ' ' << i << ' ' << v << '\n';
      if (s - 1 >= 0 && board[s - 1][i] != BROKEN) {
        kuhn.g[v].push_back(board[s - 1][i]);
      }
      if (s + 1 < m && board[s + 1][i] != BROKEN) {
        kuhn.g[v].push_back(board[s + 1][i]);
      }
      if (i - 1 >= 0 && board[s][i - 1] != BROKEN) {
        kuhn.g[v].push_back(board[s][i - 1]);
      }
      if (i + 1 < n && board[s][i + 1] != BROKEN) {
        kuhn.g[v].push_back(board[s][i + 1]);
      }
    }
  }
//  cout << "here\n";
//  for (const auto& v : board) {
//    for (int i : v) {
//      cout << i << ' ';
//    }
//    cout << '\n';
//  }
//  for (int i = 0; i < kuhn.g.size(); ++i) {
//    cout << i << ": ";
//    for (int j = 0; j < kuhn.g[i].size(); ++j) {
//      cout << kuhn.g[i][j] << ' ';
//    }
//    cout << '\n';
//  }
  kuhn.AlgKyhn();
  int cnt = 0;
  for (int i = 0; i < kuhn.match.size(); ++i) {
    if (kuhn.match[i] != -1) {
//      cout << kuhn.match[i] << ' ' << i << '\n';
      ++cnt;
    }
  }
  cnt /= 2;
//  cout << cnt << '\n';
  cout << std::min(a * cnt + (num_vertex - cnt * 2) * b, num_vertex * b) << '\n';

}
