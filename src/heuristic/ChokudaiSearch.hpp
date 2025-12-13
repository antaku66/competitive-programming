#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

struct Action;

/**
 * @brief チョクダイサーチ用の状態コンセプト
 * @details 状態クラスが満たすべき要件を定義
 */
template <typename State>
concept ChokudaiStateful = requires(State state, const Action& action) {
  { state.Advance(action) } -> same_as<void>;                     // アクションで状態を進める
  { state.EvaluateScore() } -> same_as<void>;                     // スコアを評価
  { state.GetLegalActions() } -> convertible_to<vector<Action>>;  // 合法手の取得
  { state.IsDone() } -> same_as<bool>;                            // 終了判定
  { state.first_action_ } -> same_as<Action>;                     // 最初に採用したアクション
  { state < state } -> std::convertible_to<bool>;                 // スコアの比較（大きいほど良い状態）
};

/**
 * @brief チョクダイサーチによる最良アクションの探索
 * @details ビームサーチの拡張版（反復深化型）。各深さレベルでビームを維持し、タイムアウトまで繰り返し探索を行う
 * @tparam State 状態クラス（ChokudaiStatefulコンセプトを満たす必要あり）
 * @param state 初期状態
 * @param beam_width ビーム幅（各層で処理する状態数）
 * @param beam_depth 探索深さ
 * @param time_limit_ms 制限時間（ミリ秒）
 * @return 最良の初手アクション
 */
template <ChokudaiStateful State>
Action ChokudaiSearch(const State& state, int beam_width, int beam_depth, double time_limit_ms) {
  Timer timer(time_limit_ms);

  // コピーコストを削減するためポインタを使用
  using StatePtr = shared_ptr<State>;
  struct PtrGreater {
    bool operator()(const StatePtr& lhs, const StatePtr& rhs) const { return *lhs < *rhs; }
  };

  // 最大深さまで探索した後、最小深さから未使用の状態を使って再探索するため配列に保存
  vector<priority_queue<StatePtr, vector<StatePtr>, PtrGreater>> beams(beam_depth + 1);
  beams[0].emplace(make_shared<State>(state));

  // 時間の許す限り再探索
  while (!timer.Timeout()) {
    // 指定した深さまで探索
    for (int depth = 0; depth < beam_depth; ++depth) {
      auto& now_beam = beams[depth];
      auto& next_beam = beams[depth + 1];

      // 現在の層の上位beam_width件から次の一手を探索
      for (int width = 0; width < beam_width && !now_beam.empty(); ++width) {
        StatePtr now_state = now_beam.top();
        if (now_state->IsDone()) {
          break;
        }
        now_beam.pop();

        for (const Action& action : now_state->GetLegalActions()) {
          StatePtr next_state = make_shared<State>(*now_state);
          next_state->Advance(action);
          next_state->EvaluateScore();

          // 最初に採用した一手を保存
          if (depth == 0) {
            next_state->first_action_ = action;
          }
          next_beam.push(move(next_state));
        }
      }
    }
  }

  // 最大深さから逆順にベストな一手を探す
  for (int depth = beam_depth; depth >= 0; --depth) {
    if (!beams[depth].empty()) {
      return beams[depth].top()->first_action_;
    }
  }
  throw runtime_error("No valid action found in ChokudaiSearch");
}

}  // namespace
