#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief ワーシャルフロイド法による全点間最短経路
 * @details O(V^3)で全点間の最短経路を計算。負閉路の検出も行う
 * @param dp 距離行列（dp[i][i]=0, dp[s][t]=コスト, その他kInf64で初期化）
 * @return 負閉路が存在しなければtrue、存在すればfalse
 */
bool WarshallFloyd(std::vector<std::vector<ll>>& dp) {
  const size_t v = dp.size();
  for (size_t i = 0; i < v; ++i) {
    for (size_t from = 0; from < v; ++from) {
      for (size_t to = 0; to < v; ++to) {
        if ((dp[from][i] < kInf64) && (dp[i][to] < kInf64)) {
          dp[from][to] = std::min(dp[from][to], (dp[from][i] + dp[i][to]));
        }
      }
    }
  }
  for (size_t i = 0; i < v; ++i) {
    if (dp[i][i] < 0) {
      return false;
    }
  }
  return true;
}

}  // namespace
