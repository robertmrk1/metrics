#pragma once
#include <concepts>

namespace metrics {

template <typename T>
concept Numeric = std::same_as<T, int> or std::same_as<T, double> or std::same_as<T, float>;

template <Numeric TYPE> struct Vector2 {
  constexpr Vector2(TYPE x, TYPE y) noexcept : x_(x), y_(y) {}

  [[nodiscard]] constexpr TYPE x() const noexcept { return x_; }
  [[nodiscard]] constexpr TYPE &x() noexcept { return x_; }
  [[nodiscard]] constexpr TYPE y() const noexcept { return y_; }
  [[nodiscard]] constexpr TYPE &y() noexcept { return y_; }

  [[nodiscard]] constexpr Vector2 operator+(Vector2 const &rhs) const noexcept {
    return {x() + rhs.x(), y() + rhs.y()};
  }

  [[nodiscard]] constexpr Vector2 operator-(Vector2 const &rhs) const noexcept {
    return {x() - rhs.x(), y() - rhs.y()};
  }

  template <Numeric OUT_TYPE> [[nodiscard]] constexpr Vector2<OUT_TYPE> cast() const noexcept {
    return {static_cast<OUT_TYPE>(x()), static_cast<OUT_TYPE>(y())};
  }

private:
  TYPE x_;
  TYPE y_;
};

using Position2         = Vector2<double>;
using DiscretePosition2 = Vector2<int>;

} // namespace metrics
