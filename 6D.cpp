#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using std::cin, std::cout, std::vector;

int main() {
  int n, k;
  cin >> n >> k;
  --n;
  int l = 10'000'000 / (k == 0 ? 10 : k);
  vector<vector<int>> dp(2, vector<int>(l + 1));
  for (int plane = 0; plane <= k; ++plane) {
    for (int trying = 0; trying <= l; ++trying) {
      if (plane == 0 || trying == 0) {
        dp[plane % 2][trying] = 0;
        continue;
      }
      if (plane == 1) {
        dp[plane][trying] = trying;
        continue;
      }
      dp[plane % 2][trying] = dp[plane % 2][trying - 1] + dp[1 - plane % 2][trying - 1] + 1;
    }
  }
  bool success = false;
  for (int j = 0; j <= n; ++j) {
    if (dp[k % 2][j] >= n) {
      cout << j << ' ';
      success = true;
      break;
    }
  }
  if (!success) {
    cout << -1 << '\n';
  }
}
