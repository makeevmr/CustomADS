#include <gtest/gtest.h>

#include "expect_equality.hpp"
#include "algorithms/sieve_of_eratosthenes/sieve_of_eratosthenes.hpp"

TEST(SieveOfEratosthenes, Test1) {
  std::vector<bool> expected_result = {false, false, true, true, false,
                                       true,  false, true, false};
  ads::expectVectorEquality(ads::createEratoSieve(8), expected_result);
}

TEST(SieveOfEratosthenes, Test2) {
  std::vector<bool> expected_result = {false, false, true};
  ads::expectVectorEquality(ads::createEratoSieve(2), expected_result);
}

TEST(SieveOfEratosthenes, Test3) {
  std::vector<bool> expected_result = {false, false, true,  true,  false, true,
                                       false, true,  false, false, false, true,
                                       false, true,  false, false, false};
  ads::expectVectorEquality(ads::createEratoSieve(16), expected_result);
}

TEST(SieveOfEratosthenes, ExpectThrow) {
  EXPECT_THROW(static_cast<void>(ads::createEratoSieve(0)), std::runtime_error);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
