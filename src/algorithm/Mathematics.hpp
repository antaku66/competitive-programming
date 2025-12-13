#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 切り捨ての割算
 * @details 商が負数になる場合は数直線上で左側の値を返す
 * @param x 被除数
 * @param m 除数
 * @return 切り捨て商
 */
ll Floor(ll x, ll m) {
  ll r = (x % m + m) % m;
  return (x - r) / m;
}

/**
 * @brief 10進数をN進数に変換
 * @param n 変換する10進数
 * @param N 変換先の基数
 * @return N進数の文字列表現
 */
string BaseTenToN(ll n, int N) {
  if (n == 0) {
    return "0";
  }
  string str = "";
  while (n) {
    str = to_string(n % N) + str;
    n /= N;
  }
  return str;
}

/**
 * @brief N進数を10進数に変換
 * @param str N進数の文字列表現
 * @param N 変換元の基数
 * @return 10進数の値
 */
ll BasenToTen(string str, int N) {
  ll n = 0, m = 1;
  for (ll i = str.size() - 1; i >= 0; --i) {
    n += (str[i] - '0') * m;
    m *= N;
  }
  return n;
}

/**
 * @brief べき乗のモジュロ演算
 * @param a 底
 * @param b 指数
 * @param m 法
 * @return a^b mod m
 */
ll Power(ll a, ll b, ll m) {
  ll p = a, answer = 1;
  for (int i = 0; i < 60; i++) {
    ll wari = (1LL << i);
    if ((b / wari) % 2 == 1) {
      answer = (answer * p) % m;
    }
    p = (p * p) % m;
  }
  return answer;
}

/**
 * @brief 除算のモジュロ演算
 * @details フェルマーの小定理を使用（mは素数である必要あり）
 * @param a 被除数
 * @param b 除数
 * @param m 法（素数）
 * @return (a / b) mod m
 */
ll Division(ll a, ll b, ll m) { return (a * Power(b, m - 2, m)) % m; }

/**
 * @brief 二項係数のモジュロ演算
 * @param n 全体の数
 * @param r 選ぶ数
 * @param m 法（素数）
 * @return nCr mod m
 */
ll nCr(ll n, ll r, ll m) {
  if (n < r) {
    return 0;
  }
  r = min(r, n - r);
  ll a = 1, b = 1;
  for (ll i = 0; i < r; ++i) {
    a = (a * (n - i)) % m;
    b = (b * (r - i)) % m;
  }
  return Division(a, b, m);
}

/**
 * @brief 重複組合せのモジュロ演算
 * @param n 種類の数
 * @param r 選ぶ数
 * @param m 法（素数）
 * @return nHr mod m
 */
ll nHr(ll n, ll r, ll m) { return nCr(n + r - 1, n - 1, m); }

/**
 * @brief 二項係数（オーバーフローに注意）
 * @param n 全体の数
 * @param r 選ぶ数
 * @return nCr
 */
ll nCr(ll n, ll r) {
  if (n < r) {
    return 0;
  }
  r = min(r, n - r);
  ll a = 1, b = 1;
  for (ll i = 0; i < r; ++i) {
    a = a * (n - i);
    b = b * (r - i);
  }
  return a / b;
}

/**
 * @brief 素数判定
 * @param num 判定する数
 * @return 素数ならtrue
 */
bool IsPrime(ll num) {
  if (num == 2) {
    return true;
  }
  for (ll i = 2; i * i <= num; ++i) {
    if (num % i == 0) {
      return false;
    }
  }
  return true;
}

/**
 * @brief 素因数分解
 * @param n 分解する数
 * @return {素因数, 指数}のペアのベクタ
 */
vector<pair<ll, ll>> PrimeFactorize(ll n) {
  vector<pair<ll, ll>> res;
  for (ll a = 2; a * a <= n; ++a) {
    if (n % a != 0) {
      continue;
    }
    ll ex = 0;  // 指数

    while (n % a == 0) {
      ++ex;
      n /= a;  // 割れる限り割り続ける
    }
    res.push_back({a, ex});
  }
  if (n != 1) {
    res.push_back({n, 1});  // 最後に残った数
  }
  return res;
}

/**
 * @brief エラトステネスの篩によるn以下の素数列挙
 * @param n 上限値
 * @return n以下の素数のベクタ
 */
std::vector<int> GetPrimes(int n) {
  std::vector<bool> is_prime(n + 1, true);
  std::vector<int> primes;
  for (int i = 2; i <= n; ++i) {
    if (is_prime[i]) {
      for (int j = 2 * i; j <= n; j += i) {
        is_prime[j] = false;
      }
      primes.emplace_back(i);
    }
  }
  return primes;
}

/**
 * @brief 最大公約数（ユークリッドの互除法）
 * @param a 整数1
 * @param b 整数2
 * @return gcd(a, b)
 */
ll GCD(ll a, ll b) {
  if (a % b == 0) {
    return b;
  } else {
    return GCD(b, a % b);
  }
}

/**
 * @brief 最小公倍数
 * @param a 整数1
 * @param b 整数2
 * @return lcm(a, b)
 */
ll LCM(ll a, ll b) { return a * (b / GCD(a, b)); }

/**
 * @brief 拡張ユークリッドの互除法
 * @details ax + by = gcd(a, b) を満たす (x, y) を求める
 * @param a 整数1
 * @param b 整数2
 * @param x 出力：xの値
 * @param y 出力：yの値
 * @return gcd(a, b)
 */
ll ExtGCD(ll a, ll b, ll& x, ll& y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  ll d = ExtGCD(b, a % b, y, x);
  y -= a / b * x;
  return d;
}

/**
 * @brief 最長増加部分列（LIS）の長さ
 * @tparam Type 要素の型
 * @param v 入力配列
 * @return LISの長さ
 */
template <class Type>
size_t LIS(const std::vector<Type>& v) {
  std::vector<Type> dp;
  for (const auto& elem : v) {
    auto it = std::lower_bound(dp.begin(), dp.end(), elem);
    if (it == dp.end()) {
      dp.push_back(elem);
    } else {
      *it = elem;
    }
  }
  return dp.size();
}

}  // namespace
