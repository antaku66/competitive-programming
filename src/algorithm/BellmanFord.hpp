#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 辺の構造体
 */
struct Edge {
  int from_;  ///< 始点
  int to_;    ///< 終点
  ll cost_;   ///< コスト
};

/**
 * @brief ベルマンフォード法による単一始点最短経路
 * @details 負辺を含むグラフでも使用可能。負閉路の検出も行う
 * @param edges 辺のリスト
 * @param dist 各頂点への最短距離（kInf64で初期化しておくこと）
 * @param start_index 始点のインデックス
 * @return 負閉路が存在しなければtrue、存在すればfalse
 */
bool BellmanFord(const std::vector<Edge>& edges, std::vector<ll>& dist, int start_index) {
  dist[start_index] = 0;
  for (size_t i = 0; i < dist.size(); ++i) {
    bool changed = false;
    for (const auto& edge : edges) {
      if (dist[edge.from_] == kInf64) continue;

      const ll d = dist[edge.from_] + edge.cost_;
      if (d < dist[edge.to_]) {
        dist[edge.to_] = d;
        changed = true;
      }
    }
    if (!changed) {
      return true;
    }
  }
  return false;
}

}  // namespace