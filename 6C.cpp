#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using std::cin, std::cout, std::vector;

const int INF = 2'000'000'000;

int GetRight(int a, int b, const vector<int>& pref, const vector<int>& home) {
  return pref[b + 1] - pref[a + 1] - (b - a) * home[a];
}

int GetRightReverse(int a, int b, const vector<int>& pref, const vector<int>& home) {
  int delta = home[b] - home[a];
  return (b - a + 1) * delta - GetRight(a, b, pref, home);
}

int GetDelta(int j, int ind, int r, const vector<int>& pref, const vector<int>& home) {
  return GetRight(j, r, pref, home) - GetRight(j, ind, pref, home);
}

int GetLeft(int i, const vector<int>& pref, const vector<int>& home) {
  return home[i] * (i + 1) - pref[i + 1];
}

int GetInd(int j, int i, const vector<int>& home) {
  int a = j, b = i;
  while (b - a > 1) {
    int m = (a + b) / 2;
    if (home[m] - home[j] <= home[i] - home[m]) {
      a = m;
    } else {
      b = m;
    }
  }
  return a;
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<int> home(n);
  for (int i = 0; i < n; ++i) {
    cin >> home[i];
  }
  vector<int> pref(home.size() + 1, 0);
  for (size_t i = 0; i < home.size(); ++i) {
    pref[i + 1] = pref[i] + home[i];
  }

  vector<vector<int>> dp(n, vector<int>(m + 1, INF));
  vector<vector<int>> path(n, vector<int>(m + 1, -1));
  for (int i = 0; i < n; ++i) {
    dp[i][1] = GetLeft(i, pref, home) + GetRight(i, n - 1, pref, home);
  }
  for (int cnt = 2; cnt <= m; ++cnt) {
    for (int j = cnt - 2; j < n; ++j) {
      for (int i = j + 1; i < n; ++i) {
        int ind = GetInd(j, i, home);
        int var = dp[j][cnt - 1];
        var -= GetDelta(j, ind, n - 1, pref, home);
        var += GetRightReverse(ind + 1, i, pref, home);
        var += GetRight(i, n - 1, pref, home);
//        dp[i][cnt] = std::min(dp[i][cnt], var);
        if (var < dp[i][cnt]) {
          dp[i][cnt] = var;
          path[i][cnt] = j;
        }
      }
    }
  }
  int ans = INF;
  int j = 0;
  int cnt = 0;
  for (int i = 0; i < n; ++i) {
//    cout << dp[i][m] << '\n';
    if (dp[i][m] < ans) {
      j = i;
      cnt = m;
      ans = dp[i][m];
    }
  }
  cout << ans << '\n';
//  cout << "j = " << j << '\n';
  vector<int> points{j};
//  cout << path[j][cnt] << '\n';
  while (j != -1) {
    points.push_back(path[j][cnt]);
    j = path[j][cnt];
    --cnt;
//    cout << j << ' ' << cnt << '\n';
  }
//  for (const auto& v : path) {
//    for (int i : v) {
//      cout << i << ' ';
//    }
//    cout << '\n';
//  }
  for (auto it = points.rbegin() + 1; it != points.rend(); ++it) {
    cout << home[*it] << ' ';
  }
}
