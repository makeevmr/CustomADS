#include <gtest/gtest.h>

#include "algorithms/euclidean/euclidean.hpp"

// TODO: add tests
TEST(Euclidean, TestGCD) {
  EXPECT_EQ(ads::gcd(30, 24), 6);
  EXPECT_EQ(ads::gcd(30, 0), 30);
  EXPECT_EQ(ads::gcd(30, 100), 10);
}

TEST(Euclidean, TestLCM) {
  EXPECT_EQ(ads::lcm(3, 24), 24);
  EXPECT_EQ(ads::lcm(30, 4), 60);
  EXPECT_EQ(ads::lcm(30, 15), 30);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
