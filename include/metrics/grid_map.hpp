#pragma once
#include <array>
#include <concepts>

#include "metrics/kinematic_types.hpp"

namespace metrics {

template <typename T>
concept GridMapConfig =
    std::same_as<decltype(T::SIDE_LENGTH), double const> and (T::SIDE_LENGTH > 0.0) and
    std::same_as<decltype(T::CELL_SIZE), double const> and (T::CELL_SIZE > 0.0) and (T::SIDE_LENGTH > T::CELL_SIZE);

template <typename ELEMENT, GridMapConfig CONFIG> struct FixedSizeMovingGridMap {
  static constexpr auto SIDE_LENGTH_CELLS = static_cast<int>(CONFIG::SIDE_LENGTH / CONFIG::CELL_SIZE);
  static constexpr auto SIDE_LENGTH       = SIDE_LENGTH_CELLS * static_cast<double>(CONFIG::CELL_SIZE);
  static constexpr auto CELL_SIZE         = static_cast<double>(CONFIG::CELL_SIZE);
  static constexpr auto NUMBER_OF_CELLS   = SIDE_LENGTH_CELLS * SIDE_LENGTH_CELLS;

  constexpr bool move(DiscretePosition2 const &delta) noexcept {
    if (delta.x() == 0 and delta.y() == 0) {
      return false;
    }
  }

  [[nodiscard]] ELEMENT const &at(DiscretePosition2 const &position) const noexcept;
  [[nodiscard]] ELEMENT &at(DiscretePosition2 const &position) noexcept;

private:
  [[nodiscard]] int index(Position2 const &position) const noexcept;

  DiscretePosition2 origin_ = {0, 0};
  std::array<ELEMENT, NUMBER_OF_CELLS> data_;
};
} // namespace metrics
