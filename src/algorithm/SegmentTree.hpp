#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief セグメント木（最大値/最小値クエリ）
 * @details 点更新・区間min/maxクエリをO(log n)で処理
 */
class SegmentTree {
 public:
  /**
   * @brief コンストラクタ
   * @param n 要素数
   * @param getMax trueなら最大値、falseなら最小値を取得
   */
  SegmentTree(ll n, bool get_max) : get_max_(get_max) {
    while (size_ < n) {
      size_ *= 2;
    }
    data_.resize(2 * size_, (get_max_ ? 0 : kInf64));
  }

  /**
   * @brief 点更新
   * @param pos 更新位置（0-indexed）
   * @param x 更新値
   */
  void Update(ll pos, ll x) {
    pos = pos + size_;
    data_[pos] = x;
    while (pos >= 1) {
      pos = pos / 2;
      data_[pos] = get_max_ ? max(data_[pos * 2], data_[pos * 2 + 1]) : min(data_[pos * 2], data_[pos * 2 + 1]);
    }
  }

  /**
   * @brief 区間クエリ
   * @param a 区間の左端（0-indexed, 含む）
   * @param b 区間の右端（0-indexed, 含まない）
   * @return [a, b)の最大値または最小値
   */
  ll Query(ll a, ll b) { return DoQuery(a, b, 1, 0, size_); }

 private:
  /**
   * @brief 区間クエリの再帰実装
   * @param a クエリ区間の左端
   * @param b クエリ区間の右端
   * @param k 現在のノード
   * @param l data_[k]が表す区間の左端
   * @param r data_[k]が表す区間の右端
   * @return クエリ結果
   */
  ll DoQuery(ll a, ll b, ll k, ll l, ll r) {
    if (r <= a || b <= l) {  // 範囲外なら考えない
      return get_max_ ? 0 : kInf64;
    } else if (a <= l && r <= b) {  // 範囲内なので自身の値を返す
      return data_[k];
    } else {
      ll vl = DoQuery(a, b, k * 2, l, (l + r) / 2);
      ll vr = DoQuery(a, b, k * 2 + 1, (l + r) / 2, r);
      return get_max_ ? max(vl, vr) : min(vl, vr);
    }
  }

  vector<ll> data_;  ///< セグメント木の配列（1〜size_*2-1）
  size_t size_ = 1;  ///< 葉の数（2のべき乗）
  bool get_max_;     ///< trueなら最大値、falseなら最小値
};

/**
 * @brief セグメント木（区間合計クエリ）
 * @details 点更新・区間合計クエリをO(log n)で処理
 */
class SumSegmentTree {
 public:
  /**
   * @brief コンストラクタ
   * @param n 要素数
   */
  SumSegmentTree(ll n) {
    while (size_ < n) {
      size_ *= 2;
    }
    data_.resize(2 * size_, 0);
  }

  /**
   * @brief 点更新
   * @param pos 更新位置（0-indexed）
   * @param x 更新値
   */
  void Update(ll pos, ll x) {
    pos = pos + size_;
    data_[pos] = x;
    while (pos >= 1) {
      pos = pos / 2;
      data_[pos] = data_[pos * 2] + data_[pos * 2 + 1];
    }
  }

  /**
   * @brief 区間合計クエリ
   * @param a 区間の左端（0-indexed, 含む）
   * @param b 区間の右端（0-indexed, 含まない）
   * @return [a, b)の合計
   */
  ll Query(ll a, ll b) { return DoQuery(a, b, 1, 0, size_); }

 private:
  /**
   * @brief 区間クエリの再帰実装
   * @param a クエリ区間の左端
   * @param b クエリ区間の右端
   * @param k 現在のノード
   * @param l data_[k]が表す区間の左端
   * @param r data_[k]が表す区間の右端
   * @return クエリ結果
   */
  ll DoQuery(ll a, ll b, ll k, ll l, ll r) {
    if (r <= a || b <= l) {
      return 0;
    } else if (a <= l && r <= b) {
      return data_[k];
    } else {
      ll vl = DoQuery(a, b, k * 2, l, (l + r) / 2);
      ll vr = DoQuery(a, b, k * 2 + 1, (l + r) / 2, r);
      return vl + vr;
    }
  }

  vector<ll> data_;  ///< セグメント木の配列
  size_t size_ = 1;  ///< 葉の数（2のべき乗）
};

}  // namespace
