#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

struct Action;

/**
 * @brief ビームサーチ用の状態コンセプト
 * @details 状態クラスが満たすべき要件を定義
 */
template <typename State>
concept BeamSearchStateful = requires(State state, const Action& action) {
  { state.Advance(action) } -> same_as<void>;                     // アクションで状態を進める
  { state.EvaluateScore() } -> same_as<void>;                     // スコアを評価
  { state.GetLegalActions() } -> convertible_to<vector<Action>>;  // 合法手の取得
  { state.IsDone() } -> same_as<bool>;                            // 終了判定
  { state.first_action_ } -> same_as<Action>;                     // 最初に採用したアクション
  { state < state } -> std::convertible_to<bool>;                 // スコアの比較（大きいほど良い状態）
};

/**
 * @brief ビームサーチによる最良アクションの探索
 * @tparam State 状態クラス（BeamSearchStatefulコンセプトを満たす必要あり）
 * @param state 初期状態
 * @param beam_width ビーム幅（各層で保持する状態数）
 * @param beam_depth 探索深さ
 * @param time_limit_ms 制限時間（ミリ秒）
 * @return 最良の初手アクション
 */
template <BeamSearchStateful State>
Action BeamSearch(const State& state, size_t beam_width, size_t beam_depth, double time_limit_ms) {
  Timer timer(time_limit_ms);

  // コピーコストを削減するためポインタを使用
  using StatePtr = shared_ptr<State>;
  struct PtrGreater {
    bool operator()(const StatePtr& lhs, const StatePtr& rhs) const { return *lhs < *rhs; }
  };
  struct PtrLess {
    bool operator()(const StatePtr& lhs, const StatePtr& rhs) const { return *lhs > *rhs; }
  };

  priority_queue<StatePtr, vector<StatePtr>, PtrGreater> now_beam;
  now_beam.emplace(make_shared<State>(state));
  StatePtr best_state = now_beam.top();

  // 時間の許す限り指定した深さまで探索
  for (size_t depth = 0; depth < beam_depth && !timer.Timeout(); ++depth) {
    // 低いスコアを破棄するために最小ソート
    priority_queue<StatePtr, vector<StatePtr>, PtrLess> next_beam;

    // 現在の層の上位beam_width件から次の一手を探索
    for (size_t width = 0; width < beam_width && !now_beam.empty(); ++width) {
      StatePtr now_state = now_beam.top();
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

        // ソートコスト削減のため上位beam_width件の状態のみ保持
        if (next_beam.size() > beam_width) {
          next_beam.pop();
        }
      }
    }
    if (next_beam.empty()) {
      break;
    }

    // next_beamは最小ヒープなので、最大ヒープに詰め替えて次層へ
    now_beam = priority_queue<StatePtr, vector<StatePtr>, PtrGreater>(make_move_iterator(next_beam.c.begin()),
                                                                      make_move_iterator(next_beam.c.end()));
    best_state = now_beam.top();

    if (best_state->IsDone()) {
      break;
    }
  }

  return best_state->first_action_;
}

}  // namespace