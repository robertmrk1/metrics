#pragma once
#include <array>
#include <cstdlib>
#include <functional>
#include <optional>

#include "metrics/modulo.hpp"

namespace metrics {

template <typename ELEMENT, int ARRAY_SIZE> struct FixedSizeMovingArray {
  static_assert(ARRAY_SIZE > 0, "ARRAY_SIZE must be positive");
  static constexpr auto SIZE = ARRAY_SIZE;
  [[nodiscard]] constexpr ELEMENT const &at(int external_index) const noexcept {
    return data_[static_cast<size_t>((origin_ + external_index) % SIZE)];
  }

  [[nodiscard]] constexpr ELEMENT &at(int external_index) noexcept {
    return const_cast<ELEMENT &>(static_cast<FixedSizeMovingArray const &>(*this).at(external_index));
  }

  [[nodiscard]] constexpr std::optional<std::reference_wrapper<ELEMENT const>>
  safeAt(int external_index) const noexcept {
    if (external_index < 0 or SIZE <= external_index) {
      return std::nullopt;
    }
    return {at(external_index)};
  }

  [[nodiscard]] constexpr std::optional<std::reference_wrapper<ELEMENT>> safeAt(int external_index) noexcept {
    if (external_index < 0 or SIZE <= external_index) {
      return std::nullopt;
    }
    return {at(external_index)};
  }

  constexpr bool move(int delta) {
    if (delta == 0) {
      return false;
    }

    if (std::abs(delta) >= SIZE) {
      for (auto &element : data_) {
        element = {};
      }
      origin_ = mod(origin_ + delta, SIZE);
    } else if (delta > 0) {
      for (int step = 0; step < delta; ++step) {
        data_[static_cast<size_t>(origin_)] = {};
        origin_                             = (origin_ + 1);
        origin_                             = (origin_ >= SIZE) ? (origin_ - SIZE) : origin_;
      }
    } else {
      for (int step = 0; step < -delta; ++step) {
        origin_                             = origin_ - 1;
        origin_                             = (origin_ < 0) ? (origin_ + SIZE) : origin_;
        data_[static_cast<size_t>(origin_)] = {};
      }
    }
    return true;
  }

private:
  int origin_ = 0;
  std::array<ELEMENT, ARRAY_SIZE> data_;
};
} // namespace metrics
