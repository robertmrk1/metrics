#pragma once
#include "metrics/kinematic_types.hpp"
namespace metrics {
[[nodiscard]] constexpr int mod(int dividend, int divisor) noexcept {
  int const remainder = dividend % divisor;
  return remainder < 0 ? remainder + divisor : remainder;
}

[[nodiscard]] constexpr DiscretePosition2 mod(DiscretePosition2 dividend, int divisor) noexcept {
  return {mod(dividend.x(), divisor), mod(dividend.y(), divisor)};
}

} // namespace metrics
