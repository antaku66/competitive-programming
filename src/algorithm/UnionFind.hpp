#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief Union-Find（素集合データ構造）
 * @details 経路圧縮とunion by rankによる高速化を実装
 */
struct UnionFind {
  /**
   * @brief コンストラクタ
   * @param n 要素数
   */
  UnionFind(int n) : parents_(n, -1), sizes_(n, 1) {}

  /**
   * @brief 要素xが属するグループの根を返す
   * @param x 要素のインデックス
   * @return 根のインデックス
   */
  int GetRoot(int x) {
    if (parents_[x] == -1) {
      return x;
    } else {
      return parents_[x] = GetRoot(parents_[x]);  // 経路圧縮
    }
  }

  /**
   * @brief 2つの要素が同じグループに属するか判定
   * @param x 要素1
   * @param y 要素2
   * @return 同じグループならtrue
   */
  bool IsSame(int x, int y) { return GetRoot(x) == GetRoot(y); }

  /**
   * @brief 2つの要素が属するグループを統合
   * @param x 要素1
   * @param y 要素2
   * @return 統合が行われたらtrue、既に同じグループならfalse
   */
  bool Unite(int x, int y) {
    // 根まで移動する
    x = GetRoot(x), y = GetRoot(y);
    if (x == y) {
      return false;
    }

    // union by rank（y側のサイズが小さくなるようにする）
    if (sizes_[x] < sizes_[y]) {
      swap(x, y);
    }

    // yをxの子とする
    parents_[y] = x;
    sizes_[x] += sizes_[y];
    return true;
  }

  /**
   * @brief 要素xが属するグループのサイズを返す
   * @param x 要素のインデックス
   * @return グループのサイズ
   */
  int GetSize(int x) { return sizes_[GetRoot(x)]; }

 private:
  vector<int> parents_;  ///< 各要素の親（-1は根を表す）
  vector<int> sizes_;    ///< 各グループのサイズ
};

}  // namespace
