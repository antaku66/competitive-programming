#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief ダイクストラ法による単一始点最短経路
 * @details 負辺が存在する場合は使用不可（BellmanFord法を使用）
 * @param graph 隣接リスト（各辺は{to, cost}のペア）
 * @param dists 各頂点への最短距離（kInf64で初期化しておくこと）
 * @param s 始点のインデックス
 */
void Dijkstra(const vector<vector<pair<int, ll>>>& graph, vector<ll>& dists, int s) {
  using Pair = pair<ll, int>;  // <cost, from>
  priority_queue<Pair, vector<Pair>, greater<Pair>> q;
  q.emplace((dists[s] = 0), s);

  while (!q.empty()) {
    ll dist = q.top().first;
    int from = q.top().second;
    q.pop();

    if (dists[from] < dist) continue;

    for (const auto& edge : graph[from]) {
      auto to = edge.first;
      auto cost = edge.second;
      const ll d = (dists[from] + cost);

      if (d < dists[to]) {
        q.emplace((dists[to] = d), to);
      }
    }
  }
}

}  // namespace
