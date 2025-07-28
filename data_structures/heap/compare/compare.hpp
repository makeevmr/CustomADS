#ifndef CUSTOMADS_DATA_STRUCTURES_HEAP_COMPARE_COMPARE_HPP_
#define CUSTOMADS_DATA_STRUCTURES_HEAP_COMPARE_COMPARE_HPP_

template <typename T>
class MoreCompare {
public:
  bool operator()(const T& left, const T& right);
};

#endif  // CUSTOMADS_DATA_STRUCTURES_HEAP_COMPARE_COMPARE_HPP_
