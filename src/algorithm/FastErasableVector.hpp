#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief 高速削除ベクタ
 * @details 末尾要素とスワップして削除することでO(1)削除を実現。順序は保持されない
 * @tparam T 要素の型
 */
template <class T>
class FastErasableVector {
 public:
  /**
   * @brief 要素を末尾に追加
   * @param value 追加する値
   * @return 追加された要素のインデックス
   */
  template <class U>
  size_t Push(U&& value) {
    data_.emplace_back(forward<U>(value));
    return data_.size() - 1;
  }

  /**
   * @brief 指定インデックスの要素を削除（O(1)）
   * @param i 削除するインデックス
   */
  void EraseAt(size_t i) {
    if (i >= data_.size()) {
      throw std::out_of_range("erase_at: index out of range");
    }
    size_t last = data_.size() - 1;
    if (i != last) {
      swap(data_[i], data_[last]);
    }
    data_.pop_back();
  }

  /**
   * @brief インデックスアクセス
   * @param i インデックス
   * @return i番目の要素への参照
   */
  T& operator[](size_t i) { return data_[i]; }
  const T& operator[](size_t i) const { return data_[i]; }

  /**
   * @brief サイズ取得
   * @return 要素数
   */
  size_t size() const noexcept { return data_.size(); }

  /**
   * @brief 空判定
   * @return 空ならtrue
   */
  bool empty() const noexcept { return data_.empty(); }

  /**
   * @brief 全要素削除
   */
  void clear() { data_.clear(); }

  /**
   * @brief メモリ確保
   * @param n 確保するサイズ
   */
  void reserve(size_t n) { data_.reserve(n); }

 private:
  vector<T> data_;  ///< 内部データ
};

}  // namespace
