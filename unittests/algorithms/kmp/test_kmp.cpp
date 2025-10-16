#include <gtest/gtest.h>

#include "expect_equality.hpp"
#include "algorithms/kmp/kmp.hpp"

// TODO: add tests
TEST(KMP, Test1) {
  ads::expectVectorEquality(ads::kmpSubstrSearch("ababcabcababc", "abc"),
                            {2, 5, 10});
  ads::expectVectorEquality(ads::kmpSubstrSearch("aaaaa", "aa"), {0, 1, 2, 3});
  ads::expectVectorEquality(ads::kmpSubstrSearch("abcdef", "gh"), {});
  ads::expectVectorEquality(ads::kmpSubstrSearch("", "a"), {});
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
