#ifndef CUSTOMADS_DATA_STRUCTURES_QUEUE_QUEUE_HPP_
#define CUSTOMADS_DATA_STRUCTURES_QUEUE_QUEUE_HPP_

#include <stdexcept>
#include <type_traits>

// Queue implementation on a ring buffer
// If N > 0 then allocate memory on stack
template <typename T, std::size_t N>
class Queue {
private:
  T data_[N];
  std::size_t size_;
  std::size_t top_pointer_;
  std::size_t back_pointer_;

  using reference = T&;
  using const_reference = const T&;

public:
  Queue() noexcept
      : size_(0),
        top_pointer_(0),
        back_pointer_(0) {}

  Queue(const Queue<T, N>& other) = default;

  Queue& operator=(const Queue<T, N>& other) = default;

  Queue(Queue<T, N>&& other) noexcept = default;

  Queue& operator=(Queue<T, N>&& other) noexcept = default;

  ~Queue() = default;

  // Element access
  [[nodiscard]] reference front() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    return data_[top_pointer_];
  }

  [[nodiscard]] const_reference front() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    return data_[top_pointer_];
  }

  [[nodiscard]] reference back() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    std::size_t index = back_pointer_ == 0 ? N - 1 : back_pointer_ - 1;
    return data_[index];
  }

  [[nodiscard]] const_reference back() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    std::size_t index = back_pointer_ == 0 ? N - 1 : back_pointer_ - 1;
    return data_[index];
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
    ++top_pointer_ %= N;
    --size_;
  }

  // Rewrite data if the queue is full
  void push(const T& value) {
    data_[back_pointer_] = value;
    if (size_ == N) {
      ++top_pointer_ %= N;
      ++back_pointer_ %= N;
    } else {
      ++back_pointer_ %= N;
      ++size_;
    }
  }

  void push(T&& value) {
    data_[back_pointer_] = std::move(value);
    if (size_ == N) {
      ++top_pointer_ %= N;
      ++back_pointer_ %= N;
    } else {
      ++back_pointer_ %= N;
      ++size_;
    }
  }
};

// If N == 0 then allocate memory on heap
template <typename T>
class Queue<T, 0> {
private:
  T* data_;
  std::size_t size_;
  std::size_t capacity_;
  std::size_t top_pointer_;
  std::size_t back_pointer_;

  using reference = T&;
  using const_reference = const T&;

  void swap(Queue<T, 0>& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(top_pointer_, other.top_pointer_);
    std::swap(back_pointer_, other.back_pointer_);
  }

  void free(std::size_t destructor_calls) noexcept {
    if (!std::is_trivially_destructible_v<T>) {
      std::size_t destruction_index = top_pointer_;
      std::size_t destroyed_objects = 0;
      while (destroyed_objects < destructor_calls) {
        (data_ + destruction_index)->~T();
        ++destruction_index %= capacity_;
        ++destroyed_objects;
      }
    }
    ::operator delete(data_);
  }

  void uninitializedCopy(const Queue<T, 0>& other) {
    std::size_t copied_objects = 0;
    std::size_t other_data_index = other.top_pointer_;
    try {
      for (; copied_objects < other.size_; ++copied_objects) {
        new (data_ + copied_objects) T(other.data_[other_data_index]);
        ++other_data_index %= other.capacity_;
      }
    } catch (...) {
      free(copied_objects);
      throw;
    }
  }

  void resize() {
    if (capacity_ == 0) {
      ::operator delete(data_);
      capacity_ = 1;
      data_ = reinterpret_cast<T*>(::operator new(sizeof(T) * capacity_));
    } else {
      std::size_t new_capacity = capacity_ * 2;
      T* new_data =
          reinterpret_cast<T*>(::operator new(sizeof(T) * new_capacity));
      std::size_t copied_objects = 0;
      std::size_t old_data_pointer = top_pointer_;
      if (std::is_move_constructible_v<T>) {
        while (copied_objects < size_) {
          new (new_data + copied_objects) T(std::move(data_[old_data_pointer]));
          ++old_data_pointer %= capacity_;
          ++copied_objects;
        }
        ::operator delete(data_);
      } else {
        try {
          while (copied_objects < size_) {
            new (new_data + copied_objects) T(data_[old_data_pointer]);
            ++old_data_pointer %= capacity_;
            ++copied_objects;
          }
        } catch (...) {
          if (!std::is_trivially_destructible_v<T>) {
            for (std::size_t j = 0; j < copied_objects; ++j) {
              (new_data + j)->~T();
            }
          }
          ::operator delete(new_data);
          throw;
        }
        if (!std::is_trivially_destructible_v<T>) {
          for (std::size_t i = 0; i < size_; ++i) {
            (data_ + i)->~T();
          }
        }
        ::operator delete(data_);
      }
      data_ = new_data;
      capacity_ = new_capacity;
      top_pointer_ = 0;
      back_pointer_ = size_;
    }
  }

public:
  explicit Queue<T, 0>(std::size_t capacity)
      : data_(reinterpret_cast<T*>(::operator new(sizeof(T) * capacity))),
        size_(0),
        capacity_(capacity),
        top_pointer_(0),
        back_pointer_(0) {}

  Queue(const Queue<T, 0>& other)
      : data_(
            reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity_))),
        size_(other.size_),
        capacity_(other.capacity_),
        top_pointer_(0),
        back_pointer_(other.size_) {
    uninitializedCopy(other);
  }

  Queue& operator=(const Queue<T, 0>& other) {
    if (this != &other) {
      T* new_data =
          reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity_));
      std::size_t copied_objects = 0;
      std::size_t other_data_index = other.top_pointer_;
      try {
        for (; copied_objects < other.size_; ++copied_objects) {
          new (new_data + copied_objects) T(other.data_[other_data_index]);
          ++other_data_index %= other.capacity_;
        }
      } catch (...) {
        if (!std::is_trivially_destructible_v<T>) {
          for (std::size_t j = 0; j < copied_objects; ++copied_objects) {
            (new_data + j)->~T();
          }
        }
        ::operator delete(new_data);
        throw;
      }
      free(size_);
      data_ = new_data;
      size_ = other.size_;
      capacity_ = other.capacity_;
      top_pointer_ = 0;
      back_pointer_ = size_;
    }
    return *this;
  }

  Queue(Queue<T, 0>&& other) noexcept
      : data_(nullptr),
        size_(0),
        capacity_(0),
        top_pointer_(0),
        back_pointer_(0) {
    swap(other);
  }

  Queue& operator=(Queue<T, 0>&& other) noexcept {
    if (this != &other) {
      free(size_);
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      top_pointer_ = 0;
      back_pointer_ = 0;
      swap(other);
    }
    return *this;
  }

  ~Queue() {
    free(size_);
  }

  // Element access
  [[nodiscard]] reference front() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    return data_[top_pointer_];
  }

  [[nodiscard]] const_reference front() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    return data_[top_pointer_];
  }

  [[nodiscard]] reference back() {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    std::size_t index = back_pointer_ == 0 ? capacity_ - 1 : back_pointer_ - 1;
    return data_[index];
  }

  [[nodiscard]] const_reference back() const {
    if (size_ == 0) {
      throw std::length_error("Empty queue");
    }
    std::size_t index = back_pointer_ == 0 ? capacity_ - 1 : back_pointer_ - 1;
    return data_[index];
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
    if (!std::is_trivially_destructible_v<T>) {
      (data_ + top_pointer_)->~T();
    }
    ++top_pointer_ %= capacity_;
    --size_;
  }

  void push(const T& value) {
    if (size_ == capacity_) {
      resize();
    }
    new (data_ + back_pointer_) T(value);
    ++back_pointer_ %= capacity_;
    ++size_;
  }

  void push(T&& value) {
    if (size_ == capacity_) {
      resize();
    }
    new (data_ + back_pointer_) T(std::move(value));
    ++back_pointer_ %= capacity_;
    ++size_;
  }
};

#endif  // CUSTOMADS_DATA_STRUCTURES_QUEUE_QUEUE_HPP_
