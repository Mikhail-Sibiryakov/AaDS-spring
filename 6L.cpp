#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using std::cin, std::cout, std::vector;

const int INF = 2'000'000'000;

struct Object {
  int weight;
  int cost;

  Object(int w, int c) : weight(w), cost(c) {}
};

int main() {
  int n, G, K;
  cin >> n >> G >> K;
  vector<vector<Object>> data(G + 1, vector<Object>());
  for (int i = 0; i < n; ++i) {
    int w, c, g;
    cin >> w >> c >> g;
    data[g].emplace_back(w, c);
  }

  vector<vector<int>> dp(G + 1, vector<int>(K + 1, -INF));
  for (int i = 0; i < K + 1; ++i) {
    if (i == 0) {
      dp[0][0] = 0;
    } else {
      dp[0][i] = -INF;
    }
  }
  for (int g = 1; g <= G; ++g) {
    for (int x = 0; x <= K; ++x) {
      dp[g][x] = dp[g - 1][x];
    }
    for (const auto& obj : data[g]) {
      for (int x = 0; x <= K; ++x) {
        if (x + obj.weight > K || dp[g - 1][x] == -INF) {
          continue;
        }
        dp[g][x + obj.weight] = std::max(dp[g - 1][x] + obj.cost, dp[g][x + obj.weight]);
      }
    }
  }
//  for (const auto& v : dp) {
//    for (int i : v) {
//      cout << i << ' ';
//    }
//    cout << '\n';
//  }
  int ans = -INF;
  for (int i : dp[G]) {
    ans = std::max(ans, i);
  }
  cout << ans << '\n';
}
