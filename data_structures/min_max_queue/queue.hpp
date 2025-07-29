#ifndef CUSTOMADS_DATA_STRUCTURES_MIN_MAX_QUEUE_QUEUE_HPP_
#define CUSTOMADS_DATA_STRUCTURES_MIN_MAX_QUEUE_QUEUE_HPP_

#include "stack.hpp"

namespace ads {

// Queue implementation using two stacks
template <typename T>
class Queue {
private:
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  size_type size_;
  Stack<T> push_stack_;
  Stack<T> pop_stack_;

public:
  explicit Queue(const size_type& capacity)
      : size_(0),
        push_stack_(Stack<T>(capacity)),
        pop_stack_(Stack<T>(capacity)) {}

  Queue(const Queue<T>& other) = default;

  Queue& operator=(const Queue<T>& other) = default;

  Queue(Queue<T>&& other) noexcept = default;

  Queue& operator=(Queue<T>&& other) noexcept = default;

  ~Queue() = default;

  // Element access
  [[nodiscard]] reference front() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (pop_stack_.empty()) {
      return push_stack_.bottom();
    }
    return pop_stack_.top();
  }

  [[nodiscard]] const_reference front() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (pop_stack_.empty()) {
      return push_stack_.bottom();
    }
    return pop_stack_.top();
  }

  [[nodiscard]] reference back() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (!push_stack_.empty()) {
      return push_stack_.top();
    }
    return pop_stack_.bottom();
  }

  [[nodiscard]] const_reference back() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (!push_stack_.empty()) {
      return push_stack_.top();
    }
    return pop_stack_.bottom();
  }

  // Capacity
  [[nodiscard]] bool empty() const noexcept {
    return size_ == 0;
  }

  [[nodiscard]] std::size_t getSize() const noexcept {
    return size_;
  }

  // Modifiers
  void pop() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (pop_stack_.empty()) {
      Stack<T>::template MoveStackContent<T>::get(pop_stack_, push_stack_);
    }
    pop_stack_.pop();
    --size_;
  }

  void push(const T& value) {
    push_stack_.push(value);
    ++size_;
  }

  void push(T&& value) {
    push_stack_.push(value);
    ++size_;
  }
};

template <>
class Queue<MinMaxNode> {
private:
  using reference = MinMaxNode&;
  using const_reference = const MinMaxNode&;
  using size_type = std::size_t;

  size_type size_;
  Stack<MinMaxNode> push_stack_;
  Stack<MinMaxNode> pop_stack_;

public:
  explicit Queue(const size_type& capacity)
      : size_(0),
        push_stack_(Stack<MinMaxNode>(capacity)),
        pop_stack_(Stack<MinMaxNode>(capacity)) {}

  Queue(const Queue<MinMaxNode>& other) = default;

  Queue& operator=(const Queue<MinMaxNode>& other) = default;

  Queue(Queue<MinMaxNode>&& other) noexcept = default;

  Queue& operator=(Queue<MinMaxNode>&& other) noexcept = default;

  ~Queue() = default;

  // Element access
  [[nodiscard]] reference front() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (pop_stack_.empty()) {
      return push_stack_.bottom();
    }
    return pop_stack_.top();
  }

  [[nodiscard]] const_reference front() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (pop_stack_.empty()) {
      return push_stack_.bottom();
    }
    return pop_stack_.top();
  }

  [[nodiscard]] reference back() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (!push_stack_.empty()) {
      return push_stack_.top();
    }
    return pop_stack_.bottom();
  }

  [[nodiscard]] const_reference back() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (!push_stack_.empty()) {
      return push_stack_.top();
    }
    return pop_stack_.bottom();
  }

  // Capacity
  [[nodiscard]] bool empty() const noexcept {
    return size_ == 0;
  }

  [[nodiscard]] std::size_t getSize() const noexcept {
    return size_;
  }

  // Requests
  [[nodiscard]] int getMaxDiff() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (push_stack_.empty()) {
      const MinMaxNode& pop_stack_top = pop_stack_.top();
      return pop_stack_top.max_value - pop_stack_top.min_value;
    }
    if (pop_stack_.empty()) {
      const MinMaxNode& push_stack_top = push_stack_.top();
      return push_stack_top.max_value - push_stack_top.min_value;
    }
    const MinMaxNode& pop_stack_top = pop_stack_.top();
    const MinMaxNode& push_stack_top = push_stack_.top();
    int max_value = push_stack_top.max_value > pop_stack_top.max_value
                        ? push_stack_top.max_value
                        : pop_stack_top.max_value;
    int min_value = push_stack_top.min_value < pop_stack_top.min_value
                        ? push_stack_top.min_value
                        : pop_stack_top.min_value;
    return max_value - min_value;
  }

  // Modifiers
  void pop() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    if (pop_stack_.empty()) {
      Stack<MinMaxNode>::template MoveStackContent<MinMaxNode>::get(
          pop_stack_, push_stack_);
    }
    pop_stack_.pop();
    --size_;
  }

  void push(int value) {
    if (push_stack_.empty()) {
      push_stack_.push(MinMaxNode{value, value, value});
    } else {
      MinMaxNode&& node_to_push = {value, value, value};
      const MinMaxNode& top_node = push_stack_.top();
      if (top_node.min_value < node_to_push.min_value) {
        node_to_push.min_value = top_node.min_value;
      }
      if (top_node.max_value > node_to_push.max_value) {
        node_to_push.max_value = top_node.max_value;
      }
      push_stack_.push(node_to_push);
    }
    ++size_;
  }

  void resize(size_type new_capacity) {
    pop_stack_.resize(new_capacity);
    push_stack_.resize(new_capacity);
  }
};

}  // namespace ads

#endif  // CUSTOMADS_DATA_STRUCTURES_MIN_MAX_QUEUE_QUEUE_HPP_
