#include <gtest/gtest.h>
#include <string>

#include "metrics/moving_array.hpp"

namespace metrics {
struct Metric {
  char value = 'x';
};

constexpr auto SIZE = 4;
using TestArray     = MovingArray<Metric, SIZE>;

namespace {
[[nodiscard]] std::string toString(TestArray const &array) noexcept {
  std::string out;
  for (int i = 0; i < TestArray::SIZE; ++i) {
    out += array.at(i).value;
  }
  return out;
}

[[nodiscard]] TestArray filledArray() noexcept {
  TestArray out;
  out.at(0) = Metric{'a'};
  out.at(1) = Metric{'b'};
  out.at(2) = Metric{'c'};
  out.at(3) = Metric{'d'};
  return out;
}

[[nodiscard]] TestArray otherFilledArray() noexcept {
  TestArray out;
  out.at(0) = Metric{'g'};
  out.at(1) = Metric{'h'};
  out.at(2) = Metric{'i'};
  out.at(3) = Metric{'j'};
  return out;
}

} // namespace

// abcdefgh|ijkl|mnopqrst
TEST(MovingArray, DefaultValues) {
  TestArray const array;
  ASSERT_EQ(toString(array), "xxxx");
}

TEST(MovingArray, ChangeValues) {
  auto array = filledArray();
  ASSERT_EQ(toString(array), "abcd");
}

TEST(MovingArray, MoveRight) {
  auto array = filledArray();
  array.move(1);
  ASSERT_EQ(toString(array), "bcdx");
}

TEST(MovingArray, MoveRightTwoTimes) {
  auto array = filledArray();
  array.move(1);
  array.move(1);
  ASSERT_EQ(toString(array), "cdxx");
}

TEST(MovingArray, MoveRightByTwo) {
  auto array = filledArray();
  array.move(2);
  ASSERT_EQ(toString(array), "cdxx");
}

TEST(MovingArray, MoveRightALot) {
  auto array = filledArray();
  array.move(6);
  ASSERT_EQ(toString(array), "xxxx");
}

TEST(MovingArray, MoveLeft) {
  auto array = filledArray();
  array.move(-1);
  ASSERT_EQ(toString(array), "xabc");
}

TEST(MovingArray, MoveLeftTwoTimes) {
  auto array = filledArray();
  array.move(-1);
  array.move(-1);
  ASSERT_EQ(toString(array), "xxab");
}

TEST(MovingArray, MoveLeftByTwo) {
  auto array = filledArray();
  array.move(-2);
  ASSERT_EQ(toString(array), "xxab");
}

TEST(MovingArray, MoveLeftALot) {
  auto array = filledArray();
  array.move(-6);
  ASSERT_EQ(toString(array), "xxxx");
}

TEST(MovingArray, SlideRightOnAlphabet) {
  auto array = filledArray();
  array.move(1);
  array.at(SIZE - 1) = {'e'};
  ASSERT_EQ(toString(array), "bcde");
  array.move(1);
  array.at(SIZE - 1) = {'f'};
  ASSERT_EQ(toString(array), "cdef");
  array.move(1);
  array.at(SIZE - 1) = {'g'};
  ASSERT_EQ(toString(array), "defg");
  array.move(1);
  array.at(SIZE - 1) = {'h'};
  ASSERT_EQ(toString(array), "efgh");
  array.move(1);
  array.at(SIZE - 1) = {'i'};
  ASSERT_EQ(toString(array), "fghi");
}

TEST(MovingArray, SlideLeftOnAlphabet) {
  auto array = otherFilledArray();
  ASSERT_EQ(toString(array), "ghij");
  array.move(-1);
  array.at(0) = {'f'};
  ASSERT_EQ(toString(array), "fghi");
  array.move(-1);
  array.at(0) = {'e'};
  ASSERT_EQ(toString(array), "efgh");
  array.move(-1);
  array.at(0) = {'d'};
  ASSERT_EQ(toString(array), "defg");
  array.move(-1);
  array.at(0) = {'c'};
  ASSERT_EQ(toString(array), "cdef");
  array.move(-1);
  array.at(0) = {'b'};
  ASSERT_EQ(toString(array), "bcde");
  array.move(-1);
  array.at(0) = {'a'};
  ASSERT_EQ(toString(array), "abcd");
}

} // namespace metrics
