#ifndef BATTLE_QUEUE_RING
#define BATTLE_QUEUE_RING
#include <stdexcept>
#include <type_traits>

// Queue implementation on a ring buffer
// If N > 0 then allocate memory on stack
template <typename T, std::size_t N>
class Queue {
private:
    T data[N];
    std::size_t size;
    std::size_t top_pointer;
    std::size_t back_pointer;

    using reference = T &;
    using const_reference = const T &;

public:
    Queue() noexcept : size(0), top_pointer(0), back_pointer(0) {}

    Queue(const Queue<T, N> &other) = default;

    Queue &operator=(const Queue<T, N> &other) = default;

    Queue(Queue<T, N> &&other) noexcept = default;

    Queue &operator=(Queue<T, N> &&other) noexcept = default;

    ~Queue() = default;

    // Element access
    [[nodiscard]] reference front() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        return data[top_pointer];
    }

    [[nodiscard]] const_reference front() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        return data[top_pointer];
    }

    [[nodiscard]] reference back() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        std::size_t index = back_pointer == 0 ? N - 1 : back_pointer - 1;
        return data[index];
    }

    [[nodiscard]] const_reference back() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        std::size_t index = back_pointer == 0 ? N - 1 : back_pointer - 1;
        return data[index];
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return size == 0; }

    [[nodiscard]] std::size_t getSize() const noexcept { return size; }

    // Modifiers
    void pop() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        ++top_pointer %= N;
        --size;
    }

    // Rewrite data if the queue is full
    void push(const T &value) {
        data[back_pointer] = value;
        if (size == N) {
            ++top_pointer %= N;
            ++back_pointer %= N;
        } else {
            ++back_pointer %= N;
            ++size;
        }
    }

    void push(T &&value) {
        data[back_pointer] = std::move(value);
        if (size == N) {
            ++top_pointer %= N;
            ++back_pointer %= N;
        } else {
            ++back_pointer %= N;
            ++size;
        }
    }
};

// If N == 0 then allocate memory on heap
template <typename T>
class Queue<T, 0> {
private:
    T *data;
    std::size_t size;
    std::size_t capacity;
    std::size_t top_pointer;
    std::size_t back_pointer;

    using reference = T &;
    using const_reference = const T &;

    void swap(Queue<T, 0> &other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
        std::swap(top_pointer, other.top_pointer);
        std::swap(back_pointer, other.back_pointer);
    }

    void free(std::size_t destructor_calls) noexcept {
        if (!std::is_trivially_destructible_v<T>) {
            std::size_t destruction_index = top_pointer;
            std::size_t destroyed_objects = 0;
            while (destroyed_objects < destructor_calls) {
                (data + destruction_index)->~T();
                ++destruction_index %= capacity;
                ++destroyed_objects;
            }
        }
        ::operator delete(data);
    }

    void uninitializedCopy(const Queue<T, 0> &other) {
        std::size_t copied_objects = 0;
        std::size_t other_data_index = other.top_pointer;
        try {
            for (; copied_objects < other.size; ++copied_objects) {
                new (data + copied_objects) T(other.data[other_data_index]);
                ++other_data_index %= other.capacity;
            }
        } catch (...) {
            free(copied_objects);
            throw;
        }
    }

    void resize() {
        if (capacity == 0) {
            ::operator delete(data);
            capacity = 1;
            data = reinterpret_cast<T *>(::operator new(sizeof(T) * capacity));
        } else {
            std::size_t new_capacity = capacity * 2;
            T *new_data = reinterpret_cast<T *>(::operator new(sizeof(T) * new_capacity));
            std::size_t copied_objects = 0;
            std::size_t old_data_pointer = top_pointer;
            if (std::is_move_constructible_v<T>) {
                while (copied_objects < size) {
                    new (new_data + copied_objects) T(std::move(data[old_data_pointer]));
                    ++old_data_pointer %= capacity;
                    ++copied_objects;
                }
                ::operator delete(data);
            } else {
                try {
                    while (copied_objects < size) {
                        new (new_data + copied_objects) T(data[old_data_pointer]);
                        ++old_data_pointer %= capacity;
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
                    for (std::size_t i = 0; i < size; ++i) {
                        (data + i)->~T();
                    }
                }
                ::operator delete(data);
            }
            data = new_data;
            capacity = new_capacity;
            top_pointer = 0;
            back_pointer = size;
        }
    }

public:
    explicit Queue<T, 0>(std::size_t capacity)
        : data(reinterpret_cast<T *>(::operator new(sizeof(T) * capacity))), size(0), capacity(capacity),
          top_pointer(0), back_pointer(0) {}

    Queue(const Queue<T, 0> &other)
        : data(reinterpret_cast<T *>(::operator new(sizeof(T) * other.capacity))), size(other.size),
          capacity(other.capacity), top_pointer(0), back_pointer(other.size) {
        uninitializedCopy(other);
    }

    Queue &operator=(const Queue<T, 0> &other) {
        if (this != &other) {
            T *new_data = reinterpret_cast<T *>(::operator new(sizeof(T) * other.capacity));
            std::size_t copied_objects = 0;
            std::size_t other_data_index = other.top_pointer;
            try {
                for (; copied_objects < other.size; ++copied_objects) {
                    new (new_data + copied_objects) T(other.data[other_data_index]);
                    ++other_data_index %= other.capacity;
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
            free(size);
            data = new_data;
            size = other.size;
            capacity = other.capacity;
            top_pointer = 0;
            back_pointer = size;
        }
        return *this;
    }

    Queue(Queue<T, 0> &&other) noexcept : data(nullptr), size(0), capacity(0), top_pointer(0), back_pointer(0) {
        swap(other);
    }

    Queue &operator=(Queue<T, 0> &&other) noexcept {
        if (this != &other) {
            free(size);
            data = nullptr;
            size = 0;
            capacity = 0;
            top_pointer = 0;
            back_pointer = 0;
            swap(other);
        }
        return *this;
    }

    ~Queue() { free(size); }

    // Element access
    [[nodiscard]] reference front() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        return data[top_pointer];
    }

    [[nodiscard]] const_reference front() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        return data[top_pointer];
    }

    [[nodiscard]] reference back() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        std::size_t index = back_pointer == 0 ? capacity - 1 : back_pointer - 1;
        return data[index];
    }

    [[nodiscard]] const_reference back() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        std::size_t index = back_pointer == 0 ? capacity - 1 : back_pointer - 1;
        return data[index];
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return size == 0; }

    [[nodiscard]] std::size_t getSize() const noexcept { return size; }

    // Modifiers
    void pop() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (!std::is_trivially_destructible_v<T>) {
            (data + top_pointer)->~T();
        }
        ++top_pointer %= capacity;
        --size;
    }

    void push(const T &value) {
        if (size == capacity) {
            resize();
        }
        new (data + back_pointer) T(value);
        ++back_pointer %= capacity;
        ++size;
    }

    void push(T &&value) {
        if (size == capacity) {
            resize();
        }
        new (data + back_pointer) T(std::move(value));
        ++back_pointer %= capacity;
        ++size;
    }
};

#endif // BATTLE_QUEUE_RING
