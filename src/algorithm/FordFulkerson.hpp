#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 残余グラフの辺
 */
struct FordFulkersonEdge {
  /**
   * @brief コンストラクタ
   * @param a 行き先
   * @param b 容量
   * @param c 逆辺のインデックス
   */
  FordFulkersonEdge(int a, int b, int c) : to_(a), cap_(b), rev_(c) {}
  int to_;   ///< 行き先
  int cap_;  ///< 容量
  int rev_;  ///< 逆辺のインデックス
};

/**
 * @brief フォード・ファルカーソン法による最大フロー
 * @details 残余グラフを使用して最大フローを計算
 */
class FordFulkerson {
 public:
  /**
   * @brief コンストラクタ
   * @param n 頂点数（0〜n-1まで）
   */
  FordFulkerson(int n) : size_(n) {
    graph_.resize(n);
    used_ = vector<bool>(n, false);
  }

  /**
   * @brief 辺を追加
   * @param a 始点
   * @param b 終点
   * @param c 容量
   */
  void AddEdge(int a, int b, int c) {
    int current_a = graph_[a].size();  // 現時点でのgraph_[a]の要素数
    int current_b = graph_[b].size();  // 現時点でのgraph_[b]の要素数
    graph_[a].push_back(FordFulkersonEdge{b, c, current_b});
    graph_[b].push_back(FordFulkersonEdge{a, 0, current_a});
  }

  /**
   * @brief 最大フローを計算
   * @param s 始点
   * @param t 終点
   * @return 最大フローの総流量
   */
  int Calc(int s, int t) {
    int total_flow = 0;
    while (true) {
      for (int i = 0; i < size_; ++i) {
        used_[i] = false;
      }
      int f = DFS(s, t, 1e9);

      // フローを流せなくなったら操作終了
      if (f == 0) {
        break;
      }
      total_flow += f;
    }
    return total_flow;
  }

 private:
  /**
   * @brief 深さ優先探索でフローを流す
   * @param pos 現在位置
   * @param goal 目的地
   * @param F 経路上の最小容量
   * @return 流したフローの量（流せない場合0）
   */
  int DFS(int pos, int goal, int F) {
    // ゴールに到着：フローを流せる！
    if (pos == goal) {
      return F;
    }
    used_[pos] = true;

    for (size_t i = 0; i < graph_[pos].size(); i++) {
      // 容量0の辺は使えない
      if (graph_[pos][i].cap_ == 0) {
        continue;
      }

      // 既に訪問した頂点に行っても意味がない
      if (used_[graph_[pos][i].to_]) {
        continue;
      }

      // 目的地までのパスを探す
      int flow = DFS(graph_[pos][i].to_, goal, min(F, graph_[pos][i].cap_));

      // フローを流せる場合、残余グラフの容量をflowだけ増減させる
      if (flow >= 1) {
        graph_[pos][i].cap_ -= flow;
        graph_[graph_[pos][i].to_][graph_[pos][i].rev_].cap_ += flow;
        return flow;
      }
    }

    // すべての辺を探索しても見つからなかった
    return 0;
  }

  size_t size_;                              ///< 頂点数
  vector<vector<FordFulkersonEdge>> graph_;  ///< 残余グラフ
  vector<bool> used_;                        ///< DFS訪問フラグ
};

}  // namespace
