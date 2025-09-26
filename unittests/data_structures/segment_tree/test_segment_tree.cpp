#include <algorithm>
#include <limits>

#include <gtest/gtest.h>

#include "data_structures/segment_tree/segment_tree.hpp"

template <typename T>
struct Sum {
  T operator()(const T& left, const T& right) const noexcept {
    return left + right;
  }
};

template <typename T>
struct Max {
  T operator()(const T& left, const T& right) const noexcept {
    return std::max(left, right);
  }
};

template <typename T>
struct Min {
  T operator()(const T& left, const T& right) const noexcept {
    return std::min(left, right);
  }
};

TEST(SegmentTree, CreateTree) {
  std::vector<int> vec = {1, 2, 3, 7, 10};
  ads::SegmentTree<int, Sum<int>, 0> segment_tree(vec);
  EXPECT_EQ(3, segment_tree.segmentQuery(0ULL, 1ULL));
  EXPECT_EQ(20, segment_tree.segmentQuery(2ULL, 4ULL));
}

TEST(SegmentTree, UpdateTree) {
  std::vector<int> vec = {1, 2, 3, 7, 10};
  ads::SegmentTree<int, Sum<int>, 0> segment_tree(vec);
  segment_tree.indexUpdate(0, 10);
  segment_tree.indexUpdate(1, 7);
  EXPECT_EQ(20, segment_tree.segmentQuery(0ULL, 2ULL));
  EXPECT_EQ(20, segment_tree.segmentQuery(2ULL, 4ULL));
}

TEST(SegmentTree, ThrowError) {
  std::vector<int> empty_vec;
  EXPECT_THROW((ads::SegmentTree<int, Sum<int>, 0>(empty_vec)),
               std::runtime_error);
  std::vector<int> vec = {1, 2, 3, 7, 10};
  ads::SegmentTree<int, Sum<int>, 0> segment_tree(vec);
  EXPECT_THROW(static_cast<void>(segment_tree.segmentQuery(8ULL, 3ULL)),
               std::range_error);
  EXPECT_THROW(static_cast<void>(segment_tree.segmentQuery(2ULL, 7ULL)),
               std::range_error);
  EXPECT_THROW(static_cast<void>(segment_tree.indexUpdate(5ULL, 8)),
               std::range_error);
}

TEST(SegmentTree, MaxOperator) {
  std::vector<int> vec = {-4, -10, 15, 25, 6, 2, 3, 7, 10, 0, -45};
  const std::size_t vec_size = vec.size();
  ads::SegmentTree<int, Max<int>, std::numeric_limits<int>::min()> segment_tree(
      vec);
  EXPECT_EQ(segment_tree.segmentQuery(0ULL, vec_size - 1), 25);
  EXPECT_EQ(segment_tree.segmentQuery(8ULL, vec_size - 1), 10);
  EXPECT_EQ(segment_tree.segmentQuery(0ULL, 6ULL), 25);
  EXPECT_EQ(segment_tree.segmentQuery(4ULL, 7ULL), 7);
  EXPECT_NO_THROW(segment_tree.indexUpdate(0ULL, 30));
  EXPECT_NO_THROW(segment_tree.indexUpdate(8ULL, 6));
  EXPECT_EQ(segment_tree.segmentQuery(0ULL, 6ULL), 30);
  EXPECT_EQ(segment_tree.segmentQuery(7ULL, vec_size - 1), 7);
}

TEST(SegmentTree, MinOperator) {
  std::vector<int> vec = {-4, -10, 15, 25, 6, 2, 3, 7, 10, 0, -45};
  const std::size_t vec_size = vec.size();
  ads::SegmentTree<int, Min<int>, std::numeric_limits<int>::max()> segment_tree(
      vec);
  EXPECT_EQ(segment_tree.segmentQuery(0ULL, vec_size - 1), -45);
  EXPECT_EQ(segment_tree.segmentQuery(8ULL, vec_size - 1), -45);
  EXPECT_EQ(segment_tree.segmentQuery(0ULL, 6ULL), -10);
  EXPECT_EQ(segment_tree.segmentQuery(4ULL, 7ULL), 2);
  EXPECT_NO_THROW(segment_tree.indexUpdate(0ULL, -30));
  EXPECT_NO_THROW(segment_tree.indexUpdate(9ULL, -50));
  EXPECT_EQ(segment_tree.segmentQuery(0ULL, 6ULL), -30);
  EXPECT_EQ(segment_tree.segmentQuery(7ULL, vec_size - 1), -50);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
