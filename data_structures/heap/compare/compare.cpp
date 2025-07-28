#include "compare.hpp"

template <typename T>
bool MoreCompare<T>::operator()(const T& left, const T& right) {
  return left > right;
}

template class MoreCompare<int>;
