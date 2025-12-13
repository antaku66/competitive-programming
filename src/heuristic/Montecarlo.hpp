#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

struct Action;

/**
 * @brief モンテカルロ法用の状態コンセプト
 * @details 状態クラスが満たすべき要件を定義
 */
template <typename State>
concept MontecarloStateful = requires(State state, const Action& action) {
  { state.GetScore() } -> convertible_to<double>;          // スコアを取得（大きいほど良い状態）
  { state.GetLegalActions() } -> same_as<vector<Action>>;  // 選択可能なアクションのリストを取得
  { state.GetGoodAction() } -> same_as<Action>;            // 次の一手として良さそうなアクションを取得
  { state.Advance(action) } -> same_as<void>;              // 指定したアクションで状態を進める
  { state.IsDone() } -> same_as<bool>;                     // 終了状態かどうか確認
  { state.Update() } -> same_as<void>;                     // 予測不可パラメータ等について状態を更新
};

/**
 * @brief モンテカルロ法による最良アクションの探索
 * @details 各合法手から終盤までシミュレーションを行い、スコア平均が最大の手を選択
 * @tparam State 状態クラス（MontecarloStatefulコンセプトを満たす必要あり）
 * @param state 初期状態
 * @param time_limit_ms 制限時間（ミリ秒）
 * @return 最良のアクション
 */
template <MontecarloStateful State>
Action Montecarlo(const State& state, double time_limit_ms) {
  Timer timer(time_limit_ms);
  vector<Action> legal_actions = state.GetLegalActions();
  vector<double> scores(legal_actions.size(), 0.0);

  // 次の一手の全候補から最良のものを選ぶ
  while (!timer.Timeout()) {
    for (size_t i = 0; i < legal_actions.size(); ++i) {
      auto now_state = state;
      now_state.Advance(legal_actions[i]);
      now_state.Update();

      // 最後まで進行してスコアを加算
      while (!now_state.IsDone()) {
        now_state.Advance(now_state.GetGoodAction());
        now_state.Update();
      }
      scores[i] += now_state.GetScore();
    }
  }

  size_t best_index = max_element(scores.begin(), scores.end()) - scores.begin();
  return legal_actions[best_index];
}

}  // namespace
