#define INCLUDED_HEADER

#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using pint = pair<int, int>;
using pll = pair<ll, ll>;

// 定数（constexpr はテンプレートパラメータに使用可能）
const vector<ll> kDy4 = {0, 1, 0, -1};
const vector<ll> kDx4 = {1, 0, -1, 0};
const vector<ll> kDy8 = {0, 1, 1, 1, 0, -1, -1, -1};
const vector<ll> kDx8 = {1, 1, 0, -1, -1, -1, 0, 1};
constexpr double kPi = 3.141592653589793;
constexpr ll kMod1 = 1000000007LL;
constexpr ll kMod2 = 998244353LL;
constexpr int kInf32 = (1LL << 30);
constexpr ll kInf64 = (1LL << 60);

// マクロ
#define REP(i, e) for (ll i = 0; i < static_cast<ll>(e); ++i)
#define rep(i, s, e) for (ll i = s; i < static_cast<ll>(e); ++i)
#define RREP(i, s) for (ll i = (s) - 1; i >= 0; --i)
#define ITREP(arr) for (auto it = arr.begin(); it != arr.end(); ++it)
#define ALL(x) (x).begin(), (x).end()
#define V2(T, name, n, m, ini) vector<vector<T>> name(n, vector<T>(m, ini))
#define V3(T, name, n, m, l, ini) vector<vector<vector<T>>> name(n, vector<vector<T>>(m, vector<T>(l, ini)))
#define V4(T, name, n, m, l, k, ini) \
  vector<vector<vector<vector<T>>>> name(n, vector<vector<vector<T>>>(m, vector<vector<T>>(l, vector<T>(k, ini))))
#define DIN(type, ...) \
  type __VA_ARGS__;    \
  Input(__VA_ARGS__);
#define VDIN(type, n, ...)  \
  vector<type> __VA_ARGS__; \
  InputVecs<type>(n, __VA_ARGS__);
#define GDIN(name, n, m)      \
  vector<vector<ll>> name(n); \
  REP(_, m) {                 \
    DIN(ll, u, v);            \
    name[--u].push_back(--v); \
    name[v].push_back(u);     \
  }
#define DGDIN(name, n, m)     \
  vector<vector<ll>> name(n); \
  REP(_, m) {                 \
    DIN(ll, u, v);            \
    name[--u].push_back(--v); \
  }

// ヘルパー関数
inline bool InGrid(ll y, ll x, ll h, ll w) { return 0 <= y && y < h && 0 <= x && x < w; }

template <typename... Args>
void Input(Args&... args) {
  (cin >> ... >> args);
}

template <typename T, typename... Vecs>
void InputVecs(ll n, Vecs&... vecs) {
  (vecs.resize(n), ...);
  REP(i, n)(cin >> ... >> vecs[i]);
}

// 乱数生成
template <typename T>
T RandomValue(T min, T max) {
  static thread_local mt19937_64 engine{random_device{}()};
  static_assert(is_arithmetic_v<T>, "'T' must be an arithmetic type");
  if (min > max) {
    throw invalid_argument("'min' must be less than or equal to 'max'");
  }
  if constexpr (is_integral_v<T>) {
    return uniform_int_distribution<T>(min, max)(engine);
  } else if constexpr (is_floating_point_v<T>) {
    return uniform_real_distribution<T>(min, max)(engine);
  }
}

// IO初期化
struct IOSInitializer {
  IOSInitializer() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
  }
} ios_initializer;

// タイマー
class Timer {
  using clock = chrono::high_resolution_clock;

 public:
  Timer(double limit_ms) : start_(clock::now()), limit_sec_(limit_ms / 1000.0) {}
  double Progress() const { return clamp(Elapsed() / limit_sec_, 0.0, 1.0); }
  bool Timeout() const { return Elapsed() >= limit_sec_; }

 private:
  clock::time_point start_;
  double limit_sec_;
  double Elapsed() const { return chrono::duration<double>(clock::now() - start_).count(); }
};