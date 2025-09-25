#include <gtest/gtest.h>

#include "data_structures/segment_tree/segment_tree.hpp"

struct IntSum {
  int operator()(const int& left, const int& right) const noexcept {
    return left + right;
  }
};

TEST(SegmentTree, SimpleTest) {
  std::vector<int> vec = {1, 2, 3};
  ads::SegmentTree<int, IntSum, 0> segment_tree(vec);
  EXPECT_EQ(3, segment_tree.segmentQuery(0ULL, 1ULL));
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
