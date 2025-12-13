#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 強連結成分分解（Kosaraju's algorithm）
 * @details 有向グラフを強連結成分に分解し、縮約グラフの構築も可能
 */
class StronglyConnectedComponents {
 public:
  /**
   * @brief コンストラクタ
   * @param graph 隣接リスト形式の有向グラフ
   */
  StronglyConnectedComponents(const vector<vector<int>>& graph) : graph_(graph) {
    graph_rev_.resize(graph_.size());
    components_.assign(graph_.size(), -1);
    used_.resize(graph_.size());
    REP(now, graph_.size()) {
      for (auto to : graph_[now]) {
        graph_rev_[to].push_back(now);
      }
    }

    REP(v, graph_.size()) {
      if (!used_[v]) {
        DFS(v);
      }
    }
    reverse(ALL(orders_));
    for (auto v : orders_) {
      if (components_[v] == -1) {
        RDFS(v);
        ++components_num_;
      }
    }
  }

  /**
   * @brief 強連結成分をまとめて取得
   * @return 各強連結成分に属する頂点のリスト
   */
  vector<vector<int>> GetResult() {
    vector<vector<int>> scc(components_num_);
    REP(v, components_.size()) { scc[components_[v]].push_back(v); }
    return scc;
  }

  /**
   * @brief 強連結成分を1つのノードに潰したグラフ（DAG）を再構築
   * @return 縮約グラフの隣接リスト
   */
  vector<vector<int>> Rebuild() {
    vector<vector<int>> rebuilded_graph(components_num_);
    set<pair<int, int>> connected;
    REP(now, components_num_) {
      for (auto to : graph_[now]) {
        if (components_[now] != components_[to] && !connected.count({now, to})) {
          connected.insert({now, to});
          rebuilded_graph[components_[now]].push_back(components_[to]);
        }
      }
    }
    return rebuilded_graph;
  }

 private:
  /**
   * @brief 順方向DFS（帰りがけ順を記録）
   * @param now 現在の頂点
   */
  void DFS(int now) {
    used_[now] = true;
    for (auto to : graph_[now]) {
      if (!used_[to]) {
        DFS(to);
      }
    }
    orders_.push_back(now);
  }

  /**
   * @brief 逆方向DFS（強連結成分のラベル付け）
   * @param now 現在の頂点
   */
  void RDFS(int now) {
    components_[now] = components_num_;
    for (auto to : graph_rev_[now]) {
      if (components_[to] == -1) {
        RDFS(to);
      }
    }
  }

  int components_num_ = 0;         ///< 強連結成分の数
  vector<vector<int>> graph_;      ///< 元のグラフ
  vector<vector<int>> graph_rev_;  ///< 逆辺グラフ
  vector<int> orders_;             ///< DFSの帰りがけ順
  vector<int> components_;         ///< 各頂点が属する強連結成分の番号
  vector<bool> used_;              ///< DFS訪問フラグ
};

}  // namespace
