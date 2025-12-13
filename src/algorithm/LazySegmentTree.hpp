#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 遅延セグメント木（区間更新・最大値/最小値クエリ）
 * @details 区間更新・区間min/maxクエリをO(log n)で処理
 */
class LazySegmentTree {
 public:
  /**
   * @brief コンストラクタ
   * @param n 要素数
   * @param getMax trueなら最大値、falseなら最小値を取得
   */
  LazySegmentTree(int n, bool get_max) : get_max_(get_max) {
    while (size_ < n) {
      size_ *= 2;
    }
    node_.resize(2 * size_ - 1, (get_max_ ? -kInf64 : kInf64));
    lazy_.resize(2 * size_ - 1, (get_max_ ? -kInf64 : kInf64));
    lazy_flag_.resize(2 * size_ - 1, false);
  }

  /**
   * @brief 区間更新
   * @param a 区間の左端（0-indexed, 含む）
   * @param b 区間の右端（0-indexed, 含まない）
   * @param x 更新値
   */
  void Update(int a, int b, ll x) { return UpdateImpl(a, b, x, 0, 0, size_); }

  /**
   * @brief 区間クエリ
   * @param a 区間の左端（0-indexed, 含む）
   * @param b 区間の右端（0-indexed, 含まない）
   * @return [a, b)の最大値または最小値
   */
  ll Query(int a, int b) { return QueryImpl(a, b, 0, 0, size_); }

 private:
  /**
   * @brief 遅延評価を実行
   * @param k ノードのインデックス
   * @param l ノードが表す区間の左端
   * @param r ノードが表す区間の右端
   */
  void EvalLazy(int k, int l, int r) {
    if (lazy_flag_[k]) {
      node_[k] = lazy_[k];
      if (r - l > 1) {
        lazy_[k * 2 + 1] = lazy_[k * 2 + 2] = lazy_[k];
        lazy_flag_[k * 2 + 1] = lazy_flag_[k * 2 + 2] = true;
      }
      lazy_flag_[k] = false;
    }
  }

  void UpdateImpl(int a, int b, ll x, int k, int l, int r) {
    EvalLazy(k, l, r);
    if (b <= l || r <= a) {
      return;
    }
    if (a <= l && r <= b) {
      lazy_[k] = x;
      lazy_flag_[k] = true;
      EvalLazy(k, l, r);
    } else {
      UpdateImpl(a, b, x, 2 * k + 1, l, (l + r) / 2);
      UpdateImpl(a, b, x, 2 * k + 2, (l + r) / 2, r);
      node_[k] = get_max_ ? max(node_[2 * k + 1], node_[2 * k + 2]) : min(node_[2 * k + 1], node_[2 * k + 2]);
    }
  }

  ll QueryImpl(int a, int b, int k, int l, int r) {
    EvalLazy(k, l, r);
    if (b <= l || r <= a) {
      return (get_max_ ? -kInf64 : kInf64);
    }
    if (a <= l && r <= b) {
      return node_[k];
    }
    ll vl = QueryImpl(a, b, 2 * k + 1, l, (l + r) / 2);
    ll vr = QueryImpl(a, b, 2 * k + 2, (l + r) / 2, r);
    return get_max_ ? max(vl, vr) : min(vl, vr);
  }

  vector<ll> node_;         ///< ノードの値
  vector<ll> lazy_;         ///< 遅延配列
  vector<bool> lazy_flag_;  ///< 遅延フラグ
  int size_ = 1;            ///< 葉の数（2のべき乗）
  bool get_max_;            ///< trueなら最大値、falseなら最小値
};

/**
 * @brief 遅延セグメント木（区間加算・区間合計クエリ）
 * @details 区間加算・区間合計クエリをO(log n)で処理
 */
class LazySumSegmentTree {
 public:
  /**
   * @brief コンストラクタ
   * @param n 要素数
   */
  LazySumSegmentTree(int n) {
    while (size_ < n) {
      size_ *= 2;
    }
    node_.resize(2 * size_ - 1, 0);
    lazy_.resize(2 * size_ - 1, 0);
  }

  /**
   * @brief 区間加算
   * @param a 区間の左端（0-indexed, 含む）
   * @param b 区間の右端（0-indexed, 含まない）
   * @param v 加算値
   */
  void Add(int a, int b, ll v) { AddImpl(a, b, v, 0, 0, size_); }

  /**
   * @brief 区間合計クエリ
   * @param a 区間の左端（0-indexed, 含む）
   * @param b 区間の右端（0-indexed, 含まない）
   * @return [a, b)の合計
   */
  ll Query(int a, int b) { return QueryImpl(a, b, 0, 0, size_); }

 private:
  /**
   * @brief 遅延評価を実行
   * @param k ノードのインデックス
   * @param l ノードが表す区間の左端
   * @param r ノードが表す区間の右端
   */
  void EvalLazy(int k, int l, int r) {
    // 遅延配列が空でない場合、自ノード及び子ノードへの値の伝播が起こる
    if (lazy_[k] != 0) {
      node_[k] += lazy_[k];

      // 最下段かどうかのチェック
      // 子は親の1/2の範囲であるため伝播させるときは半分
      if (r - l > 1) {
        lazy_[2 * k + 1] += lazy_[k] / 2;
        lazy_[2 * k + 2] += lazy_[k] / 2;
      }
      lazy_[k] = 0;  // 伝播が終わったので自ノードの遅延配列を空にする
    }
  }

  void AddImpl(int a, int b, ll x, int k, int l, int r) {
    EvalLazy(k, l, r);  // k番目のノードに対して遅延評価を行う
    if (b <= l || r <= a) {
      return;  // 範囲外なら何もしない
    }

    // 完全に被覆しているならば、遅延配列に値を入れた後に評価
    if (a <= l && r <= b) {
      lazy_[k] += (r - l) * x;
      EvalLazy(k, l, r);
    } else {
      // そうでなければ子の値を再帰的に計算して計算済みの値を貰う
      AddImpl(a, b, x, 2 * k + 1, l, (l + r) / 2);
      AddImpl(a, b, x, 2 * k + 2, (l + r) / 2, r);
      node_[k] = node_[2 * k + 1] + node_[2 * k + 2];
    }
  }

  ll QueryImpl(int a, int b, int k, int l, int r) {
    if (b <= l || r <= a) {
      return 0;
    }
    EvalLazy(k, l, r);  // 関数が呼び出されたら評価
    if (a <= l && r <= b) {
      return node_[k];
    }
    ll vl = QueryImpl(a, b, 2 * k + 1, l, (l + r) / 2);
    ll vr = QueryImpl(a, b, 2 * k + 2, (l + r) / 2, r);
    return vl + vr;
  }

  vector<ll> node_;  ///< ノードの値
  vector<ll> lazy_;  ///< 遅延配列
  int size_ = 1;     ///< 葉の数（2のべき乗）
};

}  // namespace
