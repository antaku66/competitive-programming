#ifndef INCLUDED_HEADER
#include "../header.hpp"
#endif

namespace {

/**
 * @brief モジュロ演算クラス（テンプレート版）
 * @details 自動的にモジュロを取り、四則演算をサポート
 * @tparam Mod 法となる値（素数を想定）
 */
template <ll Mod>
class mint {
  ll value_;  ///< 値（0 <= value_ < Mod）

 public:
  /**
   * @brief コンストラクタ
   * @param value_ 初期値（自動的にMod正規化される）
   */
  mint(ll value_ = 0) : value_((value_ % Mod + Mod) % Mod) {}

  /**
   * @brief 値を取得
   * @return 内部の値
   */
  ll val() const { return value_; }

  /**
   * @brief べき乗（繰り返し二乗法）
   * @param t 指数
   * @return this^t mod Mod
   */
  mint pow(ll t) const {
    if (!t) {
      return 1;
    }

    mint a = pow(t >> 1);
    a *= a;
    if (t & 1) {
      a *= *this;
    }
    return a;
  }

  /**
   * @brief モジュロ逆元（フェルマーの小定理）
   * @details Modが素数の場合のみ有効
   * @return this^(-1) mod Mod
   */
  mint inv() const { return pow(Mod - 2); }

  /// @brief 単項マイナス演算子
  mint operator-() const { return mint(-value_); }

  /// @brief 加算代入演算子
  mint& operator+=(const mint& a) {
    if ((value_ += a.value_) >= Mod) {
      value_ -= Mod;
    }
    return *this;
  }

  /// @brief 減算代入演算子
  mint& operator-=(const mint& a) {
    if ((value_ += Mod - a.value_) >= Mod) {
      value_ -= Mod;
    }
    return *this;
  }

  /// @brief 乗算代入演算子
  mint& operator*=(const mint& a) {
    (value_ *= a.value_) %= Mod;
    return *this;
  }

  /// @brief 加算演算子
  mint operator+(const mint& a) const {
    mint res(*this);
    return res += a;
  }

  /// @brief 減算演算子
  mint operator-(const mint& a) const {
    mint res(*this);
    return res -= a;
  }

  /// @brief 乗算演算子
  mint operator*(const mint& a) const {
    mint res(*this);
    return res *= a;
  }

  /// @brief 除算代入演算子
  mint& operator/=(const mint& a) { return (*this) *= a.inv(); }

  /// @brief 除算演算子
  mint operator/(const mint& a) const {
    mint res(*this);
    return res /= a;
  }

  /// @brief 出力ストリーム演算子
  friend ostream& operator<<(ostream& os, const mint& m) {
    os << m.value_;
    return os;
  }
};

/// @brief 10^9+7 用のmint
using mint1 = mint<kMod1>;

/// @brief 998244353 用のmint
using mint2 = mint<kMod2>;

}  // namespace
