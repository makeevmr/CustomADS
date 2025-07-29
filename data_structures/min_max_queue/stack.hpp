#ifndef CUSTOMADS_DATA_STRUCTURES_MIN_MAX_QUEUE_STACK_HPP_
#define CUSTOMADS_DATA_STRUCTURES_MIN_MAX_QUEUE_STACK_HPP_

#include <stdexcept>
#include <type_traits>

#include "min_max_node.hpp"

namespace ads {

// Stack implementation with T* dynamic array
template <typename T>
class Stack {
private:
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  T* data_;
  size_type size_;
  size_type capacity_;

  void swap(Stack<T>& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  static void free(T* data_to_free, size_type destructor_calls) noexcept {
    if (!std::is_trivially_destructible_v<T>) {
      size_type destroyed_objects = 0;
      while (destroyed_objects < destructor_calls) {
        (data_to_free + destroyed_objects)->~T();
        ++destroyed_objects;
      }
    }
    ::operator delete(data_to_free);
  }

  static void uninitializedCopy(T* copy_to, const Stack<T>& copy_from) {
    size_type copied_objects = 0;
    try {
      for (; copied_objects < copy_from.size_; ++copied_objects) {
        new (copy_to + copied_objects) T(copy_from.data_[copied_objects]);
      }
    } catch (...) {
      free(copy_to, copied_objects);
      throw;
    }
  }

public:
  explicit Stack(const size_type& capacity = 100)
      : data_(reinterpret_cast<T*>(::operator new(sizeof(T) * capacity))),
        size_(0),
        capacity_(capacity) {}

  Stack(const Stack<T>& other)
      : data_(
            reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity_))),
        size_(other.size_),
        capacity_(other.capacity_) {
    uninitializedCopy(data_, other);
  }

  Stack& operator=(const Stack<T>& other) {
    if (this != &other) {
      T* new_data =
          reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity_));
      uninitializedCopy(new_data, other);
      free(data_, size_);
      data_ = new_data;
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    return *this;
  }

  Stack(Stack<T>&& other) noexcept
      : data_(nullptr),
        size_(0),
        capacity_(0) {
    swap(other);
  }

  Stack& operator=(Stack<T>&& other) noexcept {
    if (this != &other) {
      free(data_, size_);
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      swap(other);
    }
    return *this;
  }

  ~Stack() {
    free(data_, size_);
  }

  // Element access
  [[nodiscard]] reference top() {
    if (size_ == 0) {
      throw std::length_error("Empty stack");
    }
    return data_[size_ - 1];
  }

  [[nodiscard]] const_reference top() const {
    if (size_ == 0) {
      throw std::length_error("Empty stack");
    }
    return data_[size_ - 1];
  }

  // Necessary for efficient implementation of a queue on 2 stacks
  [[nodiscard]] reference bottom() {
    if (size_ == 0) {
      throw std::length_error("Empty stack");
    }
    return data_[0];
  }

  [[nodiscard]] const_reference bottom() const {
    if (size_ == 0) {
      throw std::length_error("Empty stack");
    }
    return data_[0];
  }

  // Capacity
  [[nodiscard]] bool empty() const noexcept {
    return size_ == 0;
  }

  [[nodiscard]] size_type getSize() const noexcept {
    return size_;
  }

  // Modifiers
  void resize(size_type new_capacity) {
    T* new_data =
        reinterpret_cast<T*>(::operator new(sizeof(T) * new_capacity));
    if (std::is_move_constructible_v<T>) {
      size_type copied_objects = 0;
      while (copied_objects < size_) {
        new (new_data + copied_objects) T(std::move(data_[copied_objects]));
        ++copied_objects;
      }
      ::operator delete(data_);
    } else {
      uninitializedCopy(new_data, *this);
      free(data_, size_);
    }
    data_ = new_data;
    capacity_ = new_capacity;
  }

  void pop() {
    if (size_ == 0) {
      throw std::length_error("Empty stack");
    }
    if (!std::is_trivially_destructible_v<T>) {
      (data_ + size_ - 1)->~T();
    }
    --size_;
  }

  void push(const T& value) {
    if (size_ == capacity_) {
      resize(capacity_ * 2 + 1);
    }
    new (data_ + size_) T(value);
    ++size_;
  }

  void push(const T&& value) {
    if (size_ == capacity_) {
      resize(capacity_ * 2 + 1);
    }
    new (data_ + size_) T(std::move(value));
    ++size_;
  }

  // Move content of one push_stack to pop_stack
  template <typename Type>
  class MoveStackContent {
  public:
    static void get(Stack<Type>& move_to, Stack<Type>& move_from) {
      if (std::is_move_constructible_v<Type>) {
        while (!move_from.empty()) {
          move_to.push(std::move(move_from.top()));
          --move_from.size_;
        }
      } else {
        while (!move_from.empty()) {
          move_to.push(move_from.top());
          move_from.pop();
        }
      }
    }
  };
};

// Specialization for a problem condition
template <>
template <>
class Stack<MinMaxNode>::MoveStackContent<MinMaxNode> {
public:
  static void get(Stack<MinMaxNode>& move_to, Stack<MinMaxNode>& move_from) {
    while (!move_from.empty()) {
      if (move_to.empty()) {
        MinMaxNode& move_from_top = move_from.top();
        move_to.push(MinMaxNode{move_from_top.self_value,
                                move_from_top.self_value,
                                move_from_top.self_value});
      } else {
        MinMaxNode& move_to_top = move_to.top();
        MinMaxNode& move_from_top = move_from.top();
        move_to.push(MinMaxNode{move_from_top.self_value,
                                move_to_top.min_value < move_from_top.self_value
                                    ? move_to_top.min_value
                                    : move_from_top.self_value,
                                move_to_top.max_value > move_from_top.self_value
                                    ? move_to_top.max_value
                                    : move_from_top.self_value});
      }
      move_from.pop();
    }
  }
};

}  // namespace ads

#endif  // CUSTOMADS_DATA_STRUCTURES_MIN_MAX_QUEUE_STACK_HPP_
