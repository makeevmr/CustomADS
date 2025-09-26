#include <gtest/gtest.h>

#include "data_structures/segment_tree/segment_tree.hpp"

template <typename T>
struct Sum {
  T operator()(const T& left, const T& right) const noexcept {
    return left + right;
  }
};

// TODO: add more tests
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

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
