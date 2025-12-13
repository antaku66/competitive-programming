#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

#define CAN_RESTORE_STATE

/**
 * @brief シミュレーテッドアニーリング用の状態コンセプト
 * @details 状態クラスが満たすべき要件を定義
 */
template <typename S>
concept AnnealingState = requires(S state) {
  { state.GetScore() } -> convertible_to<double>;  // 大きいほど良いスコア
  { state.Transition() } -> same_as<void>;         // 状態の遷移
#ifdef CAN_RESTORE_STATE
  { state.Restore() } -> same_as<void>;  // 状態の復元
#endif
};

/**
 * @brief シミュレーテッドアニーリングのオプション設定
 */
struct AnnealOptions {
  /**
   * @brief コンストラクタ
   * @param time_limit_ms 制限時間（ミリ秒）
   * @param start_temp 開始温度
   * @param end_temp 終了温度
   * @param time_measure_interval タイムアウトチェック間隔
   */
  AnnealOptions(double time_limit_ms = 1900, double start_temp = 1e3, double end_temp = 1e-3,
                int time_measure_interval = 1000)
      : time_limit_ms_(time_limit_ms),
        start_temp_(start_temp),
        end_temp_(end_temp),
        time_measure_interval_(time_measure_interval) {}

  double time_limit_ms_;       ///< 制限時間（ミリ秒）
  double start_temp_;          ///< 開始温度
  double end_temp_;            ///< 終了温度
  int time_measure_interval_;  ///< タイムアウトチェック間隔
};

/**
 * @brief シミュレーテッドアニーリングの統計情報
 */
struct AnnealStats {
  size_t trials_ = 0;  ///< 試行回数

  // 改善・悪化・同値の回数
  size_t improved_cnt_ = 0;  ///< スコア改善回数
  size_t worsened_cnt_ = 0;  ///< スコア悪化回数
  size_t equal_cnt_ = 0;     ///< スコア同値回数

  // 受け入れ回数
  size_t accepted_total_ = 0;  ///< 受け入れ総数
  size_t rejected_total_ = 0;  ///< 拒否総数

  // 受け入れた改善・悪化・同値の回数
  size_t accepted_improved_ = 0;  ///< 受け入れた改善回数
  size_t accepted_worsened_ = 0;  ///< 受け入れた悪化回数
  size_t accepted_equal_ = 0;     ///< 受け入れた同値回数

  // 拒否した改善・悪化・同値の回数
  size_t rejected_improved_ = 0;  ///< 拒否した改善回数
  size_t rejected_worsened_ = 0;  ///< 拒否した悪化回数
  size_t rejected_equal_ = 0;     ///< 拒否した同値回数

  /**
   * @brief 試行結果をカウント
   * @param score_diff スコアの差分
   * @param accept 受け入れたかどうか
   */
  void Count(double score_diff, bool accept) {
    ++trials_;
    if (score_diff > 0) {
      ++improved_cnt_;
    } else if (score_diff < 0) {
      ++worsened_cnt_;
    } else {
      ++equal_cnt_;
    }
    if (accept) {
      ++accepted_total_;
      if (score_diff > 0) {
        ++accepted_improved_;
      } else if (score_diff < 0) {
        ++accepted_worsened_;
      } else {
        ++accepted_equal_;
      }
    } else {
      ++rejected_total_;
      if (score_diff > 0) {
        ++rejected_improved_;
      } else if (score_diff < 0) {
        ++rejected_worsened_;
      } else {
        ++rejected_equal_;
      }
    }
  }

  /**
   * @brief 統計情報を出力
   * @param os 出力ストリーム
   */
  void Print(ostream& os) const {
    os << "annealing count: " << trials_ << "\n"
       << "trials (improved/worsened/equal): " << improved_cnt_ << " / " << worsened_cnt_ << " / " << equal_cnt_ << "\n"
       << "accepted total: " << accepted_total_ << "  (improved: " << accepted_improved_
       << ", worsened: " << accepted_worsened_ << ", equal: " << accepted_equal_ << ")\n"
       << "rejected total: " << rejected_total_ << "  (improved: " << rejected_improved_
       << ", worsened: " << rejected_worsened_ << ", equal: " << rejected_equal_ << ")\n";
  }
};

/**
 * @brief シミュレーテッドアニーリングによる最適化
 * @tparam S 状態クラス（AnnealingStateコンセプトを満たす必要あり）
 * @param now_state 初期状態
 * @param options オプション設定
 * @return 最良の状態
 */
template <AnnealingState S>
S Annealing(S now_state, const AnnealOptions& options) {
  Timer timer(options.time_limit_ms_);
  AnnealStats stats;

  S best_state = now_state;
  double now_score = now_state.GetScore();
  double best_score = now_score;

  auto should_check_timeout = [&]() -> bool {
    return stats.trials_ > 0 && stats.trials_ % options.time_measure_interval_ == 0;
  };

  while (true) {
    if (should_check_timeout() && timer.Timeout()) {
      break;
    }

#ifdef CAN_RESTORE_STATE
    S& next_state = now_state;
#else
    S next_state = now_state;
#endif
    next_state.Transition();

    double next_score = next_state.GetScore();
    if (next_score > best_score) {
      best_state = next_state;
      best_score = next_score;
    }

    double score_diff = next_score - now_score;
    double temp = options.start_temp_ + (options.end_temp_ - options.start_temp_) * timer.Progress();
    bool accept = (score_diff > 0) || (exp(score_diff / temp) > RandomValue<double>(0.0, 1.0));

    if (accept) {
#ifndef CAN_RESTORE_STATE
      swap(now_state, next_state);
#endif
      now_score = next_score;
    } else {
#ifdef CAN_RESTORE_STATE
      now_state.Restore();
#endif
    }

    stats.Count(score_diff, accept);
  }

  stats.Print(cerr);

  return best_state;
}

}  // namespace
