#ifndef CUSTOMADS_DATA_STRUCTURES_SIMPLE_SEGMENT_TREE_SIMPLE_SEGMENT_TREE_HPP_
#define CUSTOMADS_DATA_STRUCTURES_SIMPLE_SEGMENT_TREE_SIMPLE_SEGMENT_TREE_HPP_

#include <vector>

namespace ads {

template <typename T, typename BinaryOperation, T kNeutralElement>
class SimpleSegmentTree {
public:
  explicit SimpleSegmentTree(const std::vector<T>& base_array)
      : bin_operation_(),
        base_array_size_(base_array.size()),
        segment_tree_(4 * base_array.size()) {
    if (!base_array.empty()) {
      build(base_array, 0ULL, 0ULL, base_array.size() - 1);
    }
  }

  T segmentQuery(const size_t& left, const size_t& right) {
    if (base_array_size_ == 0ULL) {
      return kNeutralElement;
    }
    return calcSubtree(0ULL, 0ULL, base_array_size_ - 1, left, right);
  }

private:
  void build(const std::vector<T>& base_array, const size_t& vertex_ind,
             const size_t& left, const size_t& right) {
    if (left == right) {
      segment_tree_[vertex_ind] = base_array[left];
    } else {
      const size_t& middle_vertex_ind = (left + right) / 2;
      const size_t left_child = vertex_ind * 2 + 1;
      const size_t right_child = vertex_ind * 2 + 2;
      build(base_array, left_child, left, middle_vertex_ind);
      build(base_array, right_child, middle_vertex_ind + 1, right);
      segment_tree_[vertex_ind] =
          bin_operation_(segment_tree_[left_child], segment_tree_[right_child]);
    }
  }

  T calcSubtree(const size_t& vertex_ind, const size_t& segment_left,
                const size_t& segment_right, const size_t& query_left,
                const size_t& query_right) {
    if (query_left > query_right) {
      return kNeutralElement;
    }
    if ((segment_left == query_left) && (segment_right == query_right)) {
      return segment_tree_[vertex_ind];
    }
    const size_t& segment_middle = (segment_left + segment_right) / 2;
    const size_t left_child = vertex_ind * 2 + 1;
    const size_t right_child = vertex_ind * 2 + 2;
    return bin_operation_(
        calcSubtree(left_child, segment_left, segment_middle, query_left,
                    std::min(query_right, segment_middle)),
        calcSubtree(right_child, segment_middle + 1, segment_right,
                    std::max(query_left, segment_middle + 1), query_right));
  }

  BinaryOperation bin_operation_;
  size_t base_array_size_;
  std::vector<T> segment_tree_;
};

}  // namespace ads

#endif  // CUSTOMADS_DATA_STRUCTURES_SIMPLE_SEGMENT_TREE_SIMPLE_SEGMENT_TREE_HPP_
