#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using std::cin, std::cout, std::vector, std::pair;

const int INF = 2'000'000'000;
const int MOD = 1'000'000'000 + 9;

struct Airport {
  int64_t x;
  int64_t y;
  int64_t k;

  Airport() = default;
  Airport(int64_t x, int64_t y, int64_t k) : x(x), y(y), k(k) {}
};

struct Matrix {
  vector<vector<int64_t>> m;
  Matrix(int64_t a, int64_t b, int64_t c, int64_t d) : m(2, vector<int64_t>(2)) {
    m[0][0] = a;
    m[0][1] = b;
    m[1][0] = c;
    m[1][1] = d;
  }

  Matrix operator*(const Matrix& other) {
    Matrix ans(0, 0, 0, 0);
    for (int s = 0; s < m.size(); ++s) {
      for (int c = 0; c < m.size(); ++c) {
        for (int i = 0; i < m.size(); ++i) {
          ans.m[s][c] += ((m[s][i] % MOD)* (other.m[i][c] % MOD)) % MOD;
          ans.m[s][c] %= MOD;
        }
      }
    }
    return ans;
  }
};

Matrix BinPow(const Matrix& a, int64_t n) {
  if (n == 0) {
    return {1, 0, 0, 1};
  }
  if (n == 1) {
    return a;
  }
  if (n % 2 == 0) {
    Matrix tmp = BinPow(a, n / 2);
    return tmp * tmp;
  }
  return BinPow(a, n - 1) * a;
}

int64_t Fib(int64_t n) {
  if (n == 0) {
    return 1;
  }
  Matrix base(1, 1, 1, 0);
  Matrix tmp = BinPow(base, n);
  return tmp.m[0][0] % MOD;
}

int main() {
  int n, q;
  cin >> n >> q;
  vector<Airport> air(n);
  vector<int64_t> lines(q);
  for (int i = 0; i < n; ++i) {
    int64_t x, y, k;
    cin >> x >> y >> k;
    air.emplace_back(x, y, k % MOD);
  }
  for (int j = 0; j < q; ++j) {
    cin >> lines[j];
  }

  for (auto c : lines) {
    int64_t ans = 0;
    for (const auto& airport : air) {
      if (airport.x + c > airport.y) {
        continue;
      }
      int64_t dst = airport.y - airport.x - c;
      ans += ((airport.k % MOD) * Fib(dst)) % MOD;
      ans %= MOD;
    }
    cout << ans << '\n';
  }
}
