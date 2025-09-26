#include <gtest/gtest.h>

#include "algorithms/sieve_of_eratosthenes/sieve_of_eratosthenes.hpp"

void testVectorEquality(const std::vector<bool>& computed_vec,
                        const std::vector<bool>& expected_vec) {
  EXPECT_EQ(computed_vec.size(), expected_vec.size());
  for (std::size_t i = 0; i < computed_vec.size(); ++i) {
    EXPECT_EQ(computed_vec[i], expected_vec[i]);
  }
}

TEST(SieveOfEratosthenes, Test1) {
  std::vector<bool> expected_result = {false, false, true, true, false,
                                       true,  false, true, false};
  testVectorEquality(ads::createEratoSieve(8), expected_result);
}

TEST(SieveOfEratosthenes, Test2) {
  std::vector<bool> expected_result = {false, false, true};
  testVectorEquality(ads::createEratoSieve(2), expected_result);
}

TEST(SieveOfEratosthenes, Test3) {
  std::vector<bool> expected_result = {false, false, true,  true,  false, true,
                                       false, true,  false, false, false, true,
                                       false, true,  false, false, false};
  testVectorEquality(ads::createEratoSieve(16), expected_result);
}

TEST(SieveOfEratosthenes, ExpectThrow) {
  EXPECT_THROW(static_cast<void>(ads::createEratoSieve(0)), std::runtime_error);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
