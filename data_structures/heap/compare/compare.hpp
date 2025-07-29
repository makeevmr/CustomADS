#ifndef CUSTOMADS_DATA_STRUCTURES_HEAP_COMPARE_COMPARE_HPP_
#define CUSTOMADS_DATA_STRUCTURES_HEAP_COMPARE_COMPARE_HPP_

namespace ads {

template <typename T>
class MoreCompare {
public:
  bool operator()(const T& left, const T& right);
};

}  // namespace ads

#endif  // CUSTOMADS_DATA_STRUCTURES_HEAP_COMPARE_COMPARE_HPP_
