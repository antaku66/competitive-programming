#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 最近共通祖先（LCA）クエリ
 * @details バイナリリフティングによる実装。前処理O(n log n)時間・空間
 */
class LowestCommonAncestor {
 public:
  /**
   * @brief コンストラクタ
   * @param graph 木の隣接リスト
   * @param root 根のインデックス（デフォルト: 0）
   */
  LowestCommonAncestor(const vector<vector<int>>& graph, int root = 0) { Init(graph, root); }

  /**
   * @brief uとvの最近共通祖先を求める
   * @param u 頂点1
   * @param v 頂点2
   * @return uとvのLCA
   */
  int Query(int u, int v) {
    if (dists_[u] < dists_[v]) {
      swap(u, v);  // uの方が深いとする
    }
    // LCAまでの距離を同じにする
    int diff = dists_[u] - dists_[v];
    for (int k = parents_.size() - 1; k >= 0; --k) {
      if (diff >= pow(2, k)) {
        u = parents_[k][u];
        diff -= pow(2, k);
      }
    }
    // 二分探索でLCAを求める
    if (u == v) {
      return u;
    }
    for (int k = static_cast<int>(parents_.size()) - 1; k >= 0; --k) {
      if (parents_[k][u] != parents_[k][v]) {
        u = parents_[k][u];
        v = parents_[k][v];
      }
    }
    return parents_[0][u];
  }

  /**
   * @brief 頂点vの根からの距離を取得
   * @param v 頂点
   * @return 根からの距離
   */
  int GetDist(int v) { return dists_[v]; }

 private:
  /**
   * @brief 初期化
   * @param graph 木の隣接リスト
   * @param root 根のインデックス
   */
  void Init(const vector<vector<int>>& graph, int root = 0) {
    int max_log = 1;
    while (pow(2, max_log) < graph.size()) {
      ++max_log;
    }
    parents_.assign(max_log, vector<int>(graph.size(), -1));
    dists_.assign(graph.size(), -1);
    DFS(graph, root, -1, 0);
    for (int k = 0; k + 1 < max_log; ++k) {
      for (size_t v = 0; v < graph.size(); ++v) {
        if (parents_[k][v] < 0) {
          parents_[k + 1][v] = -1;
        } else {
          parents_[k + 1][v] = parents_[k][parents_[k][v]];
        }
      }
    }
  }

  /**
   * @brief DFSで根からの距離と親を計算
   * @param graph 木の隣接リスト
   * @param now 現在の頂点
   * @param parent 親の頂点
   * @param dist 根からの距離
   */
  void DFS(const vector<vector<int>>& graph, int now, int parent, int dist) {
    parents_[0][now] = parent;
    dists_[now] = dist;
    for (auto to : graph[now]) {
      if (to != parent) {
        DFS(graph, to, now, dist + 1);
      }
    }
  }

  vector<vector<int>> parents_;  ///< parents_[k][u] := uの2^k先の親
  vector<int> dists_;            ///< 根からの距離
};

}  // namespace
