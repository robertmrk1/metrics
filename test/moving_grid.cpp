#include <gtest/gtest.h>
#include <string>

#include "metrics/moving_grid.hpp"

namespace metrics {
struct Metric {
  char value = 'x';
};

constexpr auto SIZE = 3;
using TestGrid      = MovingGrid<Metric, SIZE>;

namespace {
[[nodiscard]] std::string toString(TestGrid const &grid) noexcept {
  std::string out;
  for (int y = TestGrid::SIZE - 1; y >= 0; --y) {
    for (int x = 0; x < TestGrid::SIZE; ++x) {
      out += grid.at({x, y}).value;
    }
    out += '\n';
  }
  return out;
}

[[nodiscard]] TestGrid filledGrid() noexcept {
  TestGrid grid;
  grid.at({0, 0}) = {'g'};
  grid.at({1, 0}) = {'h'};
  grid.at({2, 0}) = {'i'};
  grid.at({0, 1}) = {'d'};
  grid.at({1, 1}) = {'e'};
  grid.at({2, 1}) = {'f'};
  grid.at({0, 2}) = {'a'};
  grid.at({1, 2}) = {'b'};
  grid.at({2, 2}) = {'c'};
  return grid;
}

} // namespace

TEST(MovingGrid, DefaultValues) {
  TestGrid const grid;
  ASSERT_EQ(toString(grid), "xxx\n"
                            "xxx\n"
                            "xxx\n");
}

TEST(MovingArray, ChangeValues) {
  auto grid = filledGrid();
  ASSERT_EQ(toString(grid), "abc\n"
                            "def\n"
                            "ghi\n");
}

TEST(MovingArray, MoveRight) {
  auto grid = filledGrid();
  grid.move({1, 0});
  ASSERT_EQ(toString(grid), "bcx\n"
                            "efx\n"
                            "hix\n");
}

TEST(MovingArray, MoveRightTwoTimes) {
  auto grid = filledGrid();
  grid.move({1, 0});
  ASSERT_EQ(toString(grid), "bcx\n"
                            "efx\n"
                            "hix\n");
  grid.move({1, 0});
  ASSERT_EQ(toString(grid), "cxx\n"
                            "fxx\n"
                            "ixx\n");
}

TEST(MovingArray, MoveRightByTwo) {
  auto grid = filledGrid();
  grid.move({2, 0});
  ASSERT_EQ(toString(grid), "cxx\n"
                            "fxx\n"
                            "ixx\n");
}

TEST(MovingArray, MoveLeft) {
  auto grid = filledGrid();
  grid.move({-1, 0});
  ASSERT_EQ(toString(grid), "xab\n"
                            "xde\n"
                            "xgh\n");
}

TEST(MovingArray, MoveLeftTwoTimes) {
  auto grid = filledGrid();
  grid.move({-1, 0});
  ASSERT_EQ(toString(grid), "xab\n"
                            "xde\n"
                            "xgh\n");
  grid.move({-1, 0});
  ASSERT_EQ(toString(grid), "xxa\n"
                            "xxd\n"
                            "xxg\n");
}

TEST(MovingArray, MoveLeftByTwo) {
  auto grid = filledGrid();
  grid.move({-1, 0});
  ASSERT_EQ(toString(grid), "xab\n"
                            "xde\n"
                            "xgh\n");
  grid.move({-1, 0});
  ASSERT_EQ(toString(grid), "xxa\n"
                            "xxd\n"
                            "xxg\n");
}

TEST(MovingArray, MoveUp) {
  auto grid = filledGrid();
  grid.move({0, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "abc\n"
                            "def\n");
}

TEST(MovingArray, MoveUpTwoTimes) {
  auto grid = filledGrid();
  grid.move({0, 1});
  grid.move({0, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "xxx\n"
                            "abc\n");
}

TEST(MovingArray, MoveUpByTwo) {
  auto grid = filledGrid();
  grid.move({0, 2});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "xxx\n"
                            "abc\n");
}

TEST(MovingArray, MoveDown) {
  auto grid = filledGrid();
  grid.move({0, -1});
  ASSERT_EQ(toString(grid), "def\n"
                            "ghi\n"
                            "xxx\n");
}

TEST(MovingArray, MoveDownTwoTimes) {
  auto grid = filledGrid();
  grid.move({0, -1});
  grid.move({0, -1});
  ASSERT_EQ(toString(grid), "ghi\n"
                            "xxx\n"
                            "xxx\n");
}

TEST(MovingArray, MoveDownByTwo) {
  auto grid = filledGrid();
  grid.move({0, -2});
  ASSERT_EQ(toString(grid), "ghi\n"
                            "xxx\n"
                            "xxx\n");
}

TEST(MovingArray, MoveRightUp) {
  auto grid = filledGrid();
  grid.move({1, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "bcx\n"
                            "efx\n");
}

TEST(MovingArray, MoveLeftUp) {
  auto grid = filledGrid();
  grid.move({-1, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "xab\n"
                            "xde\n");
}

TEST(MovingArray, MoveRightDown) {
  auto grid = filledGrid();
  grid.move({1, -1});
  ASSERT_EQ(toString(grid), "efx\n"
                            "hix\n"
                            "xxx\n");
}

TEST(MovingArray, MoveLeftDown) {
  auto grid = filledGrid();
  grid.move({-1, -1});
  ASSERT_EQ(toString(grid), "xde\n"
                            "xgh\n"
                            "xxx\n");
}

TEST(MovingArray, MoveRightUpAndRightUp) {
  auto grid = filledGrid();
  grid.move({1, 1});
  grid.move({1, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "xxx\n"
                            "cxx\n");
}

TEST(MovingArray, MoveLeftDownThenNewValueAndRepeat) {
  auto grid = filledGrid();
  grid.move({-1, -1});
  grid.at({0, 0}) = {'k'};
  ASSERT_EQ(toString(grid), "xde\n"
                            "xgh\n"
                            "kxx\n");
  grid.move({-1, -1});
  grid.at({0, 0}) = {'l'};
  ASSERT_EQ(toString(grid), "xxg\n"
                            "xkx\n"
                            "lxx\n");

  grid.move({-1, -1});
  grid.at({0, 0}) = {'m'};
  ASSERT_EQ(toString(grid), "xxk\n"
                            "xlx\n"
                            "mxx\n");
  grid.move({-1, -1});
  grid.at({0, 0}) = {'n'};
  ASSERT_EQ(toString(grid), "xxl\n"
                            "xmx\n"
                            "nxx\n");
}

TEST(MovingArray, MoveLeftUpALot) {
  auto grid = filledGrid();
  grid.move({-3, 3});
  grid.at({0, 2}) = {'a'};
  grid.at({1, 1}) = {'b'};
  grid.at({2, 0}) = {'c'};
  ASSERT_EQ(toString(grid), "axx\n"
                            "xbx\n"
                            "xxc\n");
  grid.move({-1, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "xax\n"
                            "xxb\n");
}

TEST(MovingArray, MoveRightTwiceUpOnce) {
  auto grid = filledGrid();
  grid.move({2, 1});
  ASSERT_EQ(toString(grid), "xxx\n"
                            "cxx\n"
                            "fxx\n");
}

} // namespace metrics
