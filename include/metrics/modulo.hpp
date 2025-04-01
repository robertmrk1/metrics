#pragma once

namespace metrics {
[[nodiscard]] constexpr int mod(int dividend, int divisor) noexcept {
  int const remainder = dividend % divisor;
  return remainder < 0 ? remainder + divisor : remainder;
}
} // namespace metrics
