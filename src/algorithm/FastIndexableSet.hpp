#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief インデックスアクセス可能な集合（重複なし）
 * @details O(1)で挿入・削除・検索・インデックスアクセスが可能
 * @tparam T 要素の型
 * @tparam Hash ハッシュ関数
 * @tparam Eq 等価比較関数
 */
template <class T, class Hash = hash<T>, class Eq = equal_to<T>>
class FastIndexableSet {
 public:
  /**
   * @brief 値の存在確認
   * @param value 検索する値
   * @return 存在すればtrue
   */
  bool contains(const T& value) const { return index_.find(value) != index_.end(); }

  /**
   * @brief 値を追加（重複は無視）
   * @param value 追加する値
   * @return 追加できればtrue、既に存在すればfalse
   */
  template <class U>
  bool insert(U&& value) {
    if (auto it = index_.find(value); it != index_.end()) {
      return false;
    }
    values_.emplace_back(forward<U>(value));
    index_[values_.back()] = values_.size() - 1;
    return true;
  }

  /**
   * @brief 値で削除
   * @param value 削除する値
   * @return 削除できればtrue、存在しなければfalse
   */
  bool erase(const T& value) {
    auto it = index_.find(value);
    if (it == index_.end()) {
      return false;
    }
    size_t i = it->second;
    size_t last = values_.size() - 1;

    if (i != last) {
      // 末尾要素をiへ移動
      T& moved = values_[last];
      values_[i] = move(moved);
      index_[values_[i]] = i;  // 移動した要素の新インデックスを更新
    }
    values_.pop_back();
    index_.erase(it);
    return true;
  }

  /**
   * @brief インデックスアクセス
   * @param i インデックス
   * @return i番目の要素への参照
   */
  T& operator[](size_t i) { return values_[i]; }
  const T& operator[](size_t i) const { return values_[i]; }

  /**
   * @brief サイズ取得
   * @return 要素数
   */
  size_t size() const noexcept { return values_.size(); }

  /**
   * @brief 空判定
   * @return 空ならtrue
   */
  bool empty() const noexcept { return values_.empty(); }

  /**
   * @brief 追加最適化用のメモリ確保
   * @param n 確保するサイズ
   */
  void reserve(size_t n) {
    values_.reserve(n);
    index_.reserve(n);
  }

 private:
  vector<T> values_;                          ///< 順序付き配列
  unordered_map<T, size_t, Hash, Eq> index_;  ///< 値から位置へのマップ
};

/**
 * @brief インデックスアクセス可能なマルチセット（重複許可）
 * @details O(1)で挿入・削除・検索・インデックスアクセスが可能
 * @tparam T 要素の型
 * @tparam Hash ハッシュ関数
 * @tparam Eq 等価比較関数
 */
template <class T, class Hash = hash<T>, class Eq = equal_to<T>>
class FastIndexableMultiSet {
 public:
  /**
   * @brief 値を追加（末尾に挿入、重複可）
   * @param value 追加する値
   * @return 割り当てられたインデックス
   */
  template <class U>
  size_t insert(U&& value) {
    values_.emplace_back(forward<U>(value));
    size_t i = values_.size() - 1;
    index_[values_[i]].insert(i);
    return i;
  }

  /**
   * @brief 値が存在するか確認
   * @param value 検索する値
   * @return 存在すればtrue
   */
  bool contains(const T& value) const {
    auto it = index_.find(value);
    return it != index_.end() && !it->second.empty();
  }

  /**
   * @brief 値の出現回数（重複数）を取得
   * @param value 検索する値
   * @return 出現回数
   */
  size_t count(const T& value) const {
    auto it = index_.find(value);
    return (it == index_.end()) ? 0u : it->second.size();
  }

  /**
   * @brief 値を1つだけ削除
   * @param value 削除する値
   * @return 削除できればtrue、存在しなければfalse
   */
  bool EraseOne(const T& value) {
    auto it = index_.find(value);
    if (it == index_.end() || it->second.empty()) {
      return false;
    }
    // 値に対応するインデックス集合から1つ取り出す
    auto idx_it = it->second.begin();
    size_t i = *idx_it;
    // 実際のインデックス削除に委譲
    EraseAt(i);
    return true;
  }

  /**
   * @brief 値で全て削除
   * @param value 削除する値
   * @return 削除した個数
   */
  size_t EraseAll(const T& value) {
    size_t removed = 0;
    while (EraseOne(value)) {
      ++removed;
    }
    return removed;
  }

  /**
   * @brief インデックスで削除（O(1)期待値）
   * @param i 削除するインデックス
   */
  void EraseAt(size_t i) {
    if (i >= values_.size()) {
      throw out_of_range("erase_at: index out of range");
    }
    size_t last = values_.size() - 1;

    // i番目の値v_iのインデックス集合からiを削除
    T& v_i = values_[i];
    auto it_set_i = index_.find(v_i);
    // 不変条件：必ず存在し、iを含む
    it_set_i->second.erase(i);

    if (i != last) {
      // 末尾要素をiにムーブし、インデックス集合を更新
      T& v_last = values_[last];
      values_[i] = move(v_last);

      // 末尾要素のインデックス集合: lastを外し、iを追加
      auto& set_last = index_[values_[i]];  // values_[i]はv_lastになっている
      // 注意: v_iとv_lastが同じ値でも安全（同じ集合に対してerase/insert）
      set_last.erase(last);
      set_last.insert(i);
    }

    // 末尾を捨てる
    values_.pop_back();

    // v_i用の集合が空ならmapからまるごと削除（メモリ節約）
    if (it_set_i->second.empty()) {
      index_.erase(it_set_i);
    }
  }

  /**
   * @brief インデックスアクセス
   * @param i インデックス
   * @return i番目の要素への参照
   */
  T& operator[](size_t i) { return values_[i]; }
  const T& operator[](size_t i) const { return values_[i]; }

  /**
   * @brief 内部配列ビュー（必要なら）
   * @return 内部配列への参照
   */
  const vector<T>& data() const noexcept { return values_; }

  /**
   * @brief サイズ取得
   * @return 要素数
   */
  size_t size() const noexcept { return values_.size(); }

  /**
   * @brief 空判定
   * @return 空ならtrue
   */
  bool empty() const noexcept { return values_.empty(); }

  /**
   * @brief 事前確保（再配置回数を減らし、速度・安定性向上）
   * @param n 確保するサイズ
   */
  void reserve(size_t n) {
    values_.reserve(n);
    index_.reserve(n);
    // 各値ごとのunordered_setまでは事前確保しない（分布が不明のため）
  }

 private:
  vector<T> values_;                                         ///< 値の密配列
  unordered_map<T, unordered_set<size_t>, Hash, Eq> index_;  ///< 値からインデックス集合へのマップ
};

}  // namespace
