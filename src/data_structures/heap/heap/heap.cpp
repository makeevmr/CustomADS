#include "heap.hpp"

#include <stdexcept>

#include "../compare/compare.hpp"

namespace ads {

// public

template <typename T, typename Compare>
Heap<T, Compare>::Heap()
    : data_(nullptr),
      size_(0),
      capacity_(0),
      comparator_(Compare()) {}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const size_type capacity)
    : data_(reinterpret_cast<value_type*>(
          ::operator new(sizeof(value_type) * capacity))),
      size_(0),
      capacity_(capacity),
      comparator_(Compare()) {}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(value_type* construct_from_data, size_type size)
    : data_(reinterpret_cast<value_type*>(
          ::operator new(sizeof(value_type) * size))),
      size_(size),
      capacity_(size),
      comparator_(Compare()) {
  uninitializedCopy(data_, construct_from_data, size);
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const Heap<T, Compare>& other)
    : data_(reinterpret_cast<value_type*>(
          ::operator new(sizeof(value_type) * other.capacity_))),
      size_(other.size_),
      capacity_(other.capacity_),
      comparator_(Compare()) {
  uninitializedCopy(data_, other);
}

template <typename T, typename Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(const Heap<T, Compare>& other) {
  if (this != &other) {
    value_type* new_data = reinterpret_cast<value_type*>(
        ::operator new(sizeof(value_type) * other.capacity_));
    uninitializedCopy(new_data, other);
    free(data_, size_);
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
  }
  return *this;
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(Heap&& other) noexcept
    : data_(nullptr),
      size_(0),
      capacity_(0),
      comparator_(Compare()) {
  swap(other);
}

template <typename T, typename Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(
    Heap<T, Compare>&& other) noexcept {
  if (this != &other) {
    free(data_, size_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    swap(other);
  }
  return *this;
}

template <typename T, typename Compare>
Heap<T, Compare>::~Heap() {
  free(data_, size_);
}

// Element access
template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::const_reference Heap<T, Compare>::top()
    const {
  if (size_ == 0) {
    throw std::length_error("Heap is empty");
  }
  return data_[0];
}

// Capacity
template <typename T, typename Compare>
[[nodiscard]] bool Heap<T, Compare>::empty() const noexcept {
  return size_ == 0;
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type Heap<T, Compare>::getSize()
    const noexcept {
  return size_;
}

// Modifiers
template <typename T, typename Compare>
void Heap<T, Compare>::push(const value_type& value) {
  if (size_ == capacity_) {
    this->resize();
  }
  new (data_ + size_) value_type(value);
  ++size_;
  siftingUp(size_ - 1);
}

template <typename T, typename Compare>
void Heap<T, Compare>::push(const value_type&& value) {
  if (size_ == capacity_) {
    this->resize();
  }
  new (data_ + size_) value_type(std::move(value));
  ++size_;
  siftingUp(size_ - 1);
}

template <typename T, typename Compare>
void Heap<T, Compare>::pop() {
  if (size_ == 0) {
    throw std::length_error("Heap is empty");
  }
  data_[0] = std::move(data_[size_ - 1]);
  if (!std::is_trivially_destructible_v<T>) {
    (data_ + size_ - 1)->~value_type();
  }
  --size_;
  siftingDown(0);
}

// private

template <typename T, typename Compare>
void Heap<T, Compare>::swap(Heap<T, Compare>& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T, typename Compare>
void Heap<T, Compare>::free(value_type* data_to_free,
                            size_type destructor_calls) noexcept {
  if (!std::is_trivially_destructible_v<value_type>) {
    size_type destroyed_objects = 0;
    while (destroyed_objects < destructor_calls) {
      (data_to_free + destroyed_objects)->~value_type();
      ++destroyed_objects;
    }
  }
  ::operator delete(data_to_free);
}

template <typename T, typename Compare>
void Heap<T, Compare>::uninitializedCopy(value_type* copy_to,
                                         const Heap<T, Compare>& copy_from) {
  size_type copied_objects = 0;
  try {
    for (; copied_objects < copy_from.size_; ++copied_objects) {
      new (copy_to + copied_objects)
          value_type(copy_from.data_[copied_objects]);
    }
  } catch (...) {
    free(copy_to, copied_objects);
    throw;
  }
}

template <typename T, typename Compare>
void Heap<T, Compare>::uninitializedCopy(value_type* copy_to,
                                         const value_type* copy_from,
                                         size_type size) {
  size_type copied_objects = 0;
  try {
    for (; copied_objects < size; ++copied_objects) {
      new (copy_to + copied_objects) value_type(copy_from[copied_objects]);
    }
  } catch (...) {
    free(copy_to, copied_objects);
    throw;
  }
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type Heap<T, Compare>::getLeft(
    size_type index) const noexcept {
  return 2 * index + 1;
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type Heap<T, Compare>::getRight(
    size_type index) const noexcept {
  return 2 * index + 2;
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type Heap<T, Compare>::getParent(
    size_type index) const noexcept {
  return (index - 1) / 2;
}

template <typename T, typename Compare>
void Heap<T, Compare>::resize() {
  size_type new_capacity = capacity_ > 0 ? capacity_ * 2 : capacity_ + 1;
  value_type* new_data = reinterpret_cast<value_type*>(
      ::operator new(sizeof(value_type) * new_capacity));
  if (std::is_move_constructible_v<value_type>) {
    size_type moved_objects = 0;
    while (moved_objects < size_) {
      new (new_data + moved_objects)
          value_type(std::move(data_[moved_objects]));
      ++moved_objects;
    }
    ::operator delete(data_);
  } else {
    uninitializedCopy(new_data, *this);
    free(data_, size_);
  }
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T, typename Compare>
void Heap<T, Compare>::siftingDown(size_type index) noexcept(
    std::is_nothrow_swappable_v<value_type>) {
  size_type iter_start_index = index;
  size_type iter_end_index = index;
  do {
    iter_start_index = iter_end_index;
    size_type left = getLeft(iter_start_index);
    size_type right = getRight(iter_start_index);
    if ((left < size_) && (comparator_(data_[left], data_[iter_end_index]))) {
      iter_end_index = left;
    }
    if ((right < size_) && (comparator_(data_[right], data_[iter_end_index]))) {
      iter_end_index = right;
    }
    if (iter_start_index != iter_end_index) {
      std::swap(data_[iter_start_index], data_[iter_end_index]);
    }
  } while (iter_start_index != iter_end_index);
}

template <typename T, typename Compare>
void Heap<T, Compare>::siftingUp(size_type index) noexcept(
    std::is_nothrow_swappable_v<value_type>) {
  bool is_sifting_complete = false;
  while ((index > 0) && !is_sifting_complete) {
    is_sifting_complete = true;
    size_type parent = getParent(index);
    if (comparator_(data_[index], data_[parent])) {
      std::swap(data_[index], data_[parent]);
      index = parent;
      is_sifting_complete = false;
    }
  }
}

template <typename T, typename Compare>
void Heap<T, Compare>::makeHeap() noexcept(std::is_nothrow_swappable_v<T>) {
  for (size_type i = (size_ / 2 - 1); i > 0; --i) {
    siftingDown(i);
  }
}

}  // namespace ads

template class ads::Heap<int, ads::MoreCompare<int>>;
