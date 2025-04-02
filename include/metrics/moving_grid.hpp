#pragma once
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <optional>

#include "metrics/kinematic_types.hpp"
#include "metrics/modulo.hpp"

namespace metrics {

template <typename T>
concept GridMapConfig =
    std::same_as<decltype(T::SIDE_LENGTH), double const> and (T::SIDE_LENGTH > 0.0) and
    std::same_as<decltype(T::CELL_SIZE), double const> and (T::CELL_SIZE > 0.0) and (T::SIDE_LENGTH > T::CELL_SIZE);

template <typename ELEMENT, int SIDE_SIZE> struct MovingGrid {
  static constexpr auto SIZE            = SIDE_SIZE;
  static constexpr auto NUMBER_OF_CELLS = SIZE * SIZE;

  [[nodiscard]] constexpr ELEMENT const &at(DiscretePosition2 external_index) const noexcept {
    return data_[static_cast<std::size_t>(externalIndexToFlatIndex(external_index))];
  }

  [[nodiscard]] constexpr ELEMENT &at(DiscretePosition2 external_index) noexcept {
    return const_cast<ELEMENT &>(static_cast<MovingGrid const &>(*this).at(external_index));
  }

  [[nodiscard]] constexpr std::optional<std::reference_wrapper<ELEMENT const>>
  safeAt(DiscretePosition2 external_index) const noexcept {
    if (external_index.x() < 0 or SIZE <= external_index.x() or external_index.y() < 0 or SIZE <= external_index.y()) {
      return std::nullopt;
    }
    return {at(external_index)};
  }
  [[nodiscard]] constexpr std::optional<std::reference_wrapper<ELEMENT>>
  safeAt(DiscretePosition2 external_index) noexcept {
    if (external_index.x() < 0 or SIZE <= external_index.x() or external_index.y() < 0 or SIZE <= external_index.y()) {
      return std::nullopt;
    }
    return {at(external_index)};
  }

  constexpr bool move(DiscretePosition2 const &delta) noexcept {
    if (delta.x() == 0 and delta.y() == 0) {
      return false;
    }

    if (std::abs(delta.x()) >= SIZE or std::abs(delta.y()) >= SIZE) {
      for (auto &elem : data_) {
        elem = {};
      }
      origin_ = mod(origin_ + delta, SIZE);
      return true;
    }

    if (delta.x() > 0) {
      for (int step = 0; step < delta.x(); ++step) {
        for (int y = 0; y < SIZE; ++y) {
          data_[static_cast<std::size_t>(internalIndexToFlatIndex({origin_.x(), y}))] = {};
        }
        origin_.x() += 1;
        origin_.x() = (origin_.x() >= SIZE) ? (origin_.x() - SIZE) : origin_.x();
      }
    } else {
      for (int step = 0; step < -delta.x(); ++step) {
        origin_.x() -= 1;
        origin_.x() = (origin_.x() < 0) ? (origin_.x() + SIZE) : origin_.x();
        for (int y = 0; y < SIZE; ++y) {
          data_[static_cast<std::size_t>(internalIndexToFlatIndex({origin_.x(), y}))] = {};
        }
      }
    }

    if (delta.y() > 0) {
      for (int step = 0; step < delta.y(); ++step) {
        for (int x = 0; x < SIZE; ++x) {
          data_[static_cast<std::size_t>(internalIndexToFlatIndex({x, origin_.y()}))] = {};
        }
        origin_.y() += 1;
        origin_.y() = (origin_.y() >= SIZE) ? (origin_.y() - SIZE) : origin_.y();
      }
    } else {
      for (int step = 0; step < -delta.y(); ++step) {
        origin_.y() -= 1;
        origin_.y() = (origin_.y() < 0) ? (origin_.y() + SIZE) : origin_.y();
        for (int x = 0; x < SIZE; ++x) {
          data_[static_cast<std::size_t>(internalIndexToFlatIndex({x, origin_.y()}))] = {};
        }
      }
    }

    return true;
  }

private:
  [[nodiscard]] constexpr int externalIndexToFlatIndex(DiscretePosition2 const &external_index) const noexcept {
    auto internal_index = external_index + origin_;
    internal_index.x()  = internal_index.x() % SIZE;
    internal_index.y()  = internal_index.y() % SIZE;
    return internalIndexToFlatIndex(internal_index);
  }

  [[nodiscard]] constexpr int internalIndexToFlatIndex(DiscretePosition2 const &internal_index) const noexcept {
    return internal_index.x() + (SIZE * internal_index.y());
  }

  DiscretePosition2 origin_ = {0, 0};
  std::array<ELEMENT, NUMBER_OF_CELLS> data_;
};
} // namespace metrics
