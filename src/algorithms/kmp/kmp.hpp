#ifndef CUSTOMADS_SRC_ALGORITHMS_KMP_KMP_HPP_
#define CUSTOMADS_SRC_ALGORITHMS_KMP_KMP_HPP_

#include <vector>
#include <string>

namespace ads {

[[nodiscard]] std::vector<std::size_t> kmpSubstrSearch(
    const std::string& text, const std::string& substr);

}  // namespace ads

#endif  // CUSTOMADS_SRC_ALGORITHMS_KMP_KMP_HPP_
