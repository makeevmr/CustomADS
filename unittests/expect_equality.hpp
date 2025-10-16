#ifndef CUSTOMADS_SRC_UNITTESTS_EXPECT_EQUALITY_HPP_
#define CUSTOMADS_SRC_UNITTESTS_EXPECT_EQUALITY_HPP_

#include <vector>

namespace ads {

template <typename T>
void expectVectorEquality(const std::vector<T>& computed_vec,
                          const std::vector<T>& expected_vec) {
  EXPECT_EQ(computed_vec.size(), expected_vec.size());
  for (std::size_t i = 0; i < computed_vec.size(); ++i) {
    EXPECT_EQ(computed_vec[i], expected_vec[i]);
  }
}

}  // namespace ads

#endif  // CUSTOMADS_SRC_UNITTESTS_EXPECT_EQUALITY_HPP_
