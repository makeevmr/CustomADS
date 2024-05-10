#ifndef MIN_MAX_QUEUE_STACK
#define MIN_MAX_QUEUE_STACK
#include "../MinMaxNode/MinMaxNode.h"
#include <stdexcept>
#include <type_traits>

// Stack implementation with T* dynamic array
template <typename T>
class Stack {
private:
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;

    T *data;
    size_type size;
    size_type capacity;

    void swap(Stack<T> &other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);
    }

    static void free(T *data_to_free, size_type destructor_calls) noexcept {
        if (!std::is_trivially_destructible_v<T>) {
            size_type destroyed_objects = 0;
            while (destroyed_objects < destructor_calls) {
                (data_to_free + destroyed_objects)->~T();
                ++destroyed_objects;
            }
        }
        ::operator delete(data_to_free);
    }

    static void uninitializedCopy(T *copy_to, const Stack<T> &copy_from) {
        size_type copied_objects = 0;
        try {
            for (; copied_objects < copy_from.size; ++copied_objects) {
                new (copy_to + copied_objects) T(copy_from.data[copied_objects]);
            }
        } catch (...) {
            free(copy_to, copied_objects);
            throw;
        }
    }

public:
    explicit Stack(const size_type &capacity = 100)
        : data(reinterpret_cast<T *>(::operator new(sizeof(T) * capacity))), size(0), capacity(capacity) {}

    Stack(const Stack<T> &other)
        : data(reinterpret_cast<T *>(::operator new(sizeof(T) * other.capacity))), size(other.size),
          capacity(other.capacity) {
        uninitializedCopy(data, other);
    }

    Stack &operator=(const Stack<T> &other) {
        if (this != &other) {
            T *new_data = reinterpret_cast<T *>(::operator new(sizeof(T) * other.capacity));
            uninitializedCopy(new_data, other);
            free(data, size);
            data = new_data;
            size = other.size;
            capacity = other.capacity;
        }
        return *this;
    }

    Stack(Stack<T> &&other) noexcept : data(nullptr), size(0), capacity(0) { swap(other); }

    Stack &operator=(Stack<T> &&other) noexcept {
        if (this != &other) {
            free(data, size);
            data = nullptr;
            size = 0;
            capacity = 0;
            swap(other);
        }
        return *this;
    }

    ~Stack() { free(data, size); }

    // Element access
    [[nodiscard]] reference top() {
        if (size == 0) {
            throw std::length_error("Empty stack");
        }
        return data[size - 1];
    }

    [[nodiscard]] const_reference top() const {
        if (size == 0) {
            throw std::length_error("Empty stack");
        }
        return data[size - 1];
    }

    // Necessary for efficient implementation of a queue on 2 stacks
    [[nodiscard]] reference bottom() {
        if (size == 0) {
            throw std::length_error("Empty stack");
        }
        return data[0];
    }

    [[nodiscard]] const_reference bottom() const {
        if (size == 0) {
            throw std::length_error("Empty stack");
        }
        return data[0];
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return size == 0; }

    [[nodiscard]] size_type getSize() const noexcept { return size; }

    // Modifiers
    void resize(size_type new_capacity) {
        T *new_data = reinterpret_cast<T *>(::operator new(sizeof(T) * new_capacity));
        if (std::is_move_constructible_v<T>) {
            size_type copied_objects = 0;
            while (copied_objects < size) {
                new (new_data + copied_objects) T(std::move(data[copied_objects]));
                ++copied_objects;
            }
            ::operator delete(data);
        } else {
            uninitializedCopy(new_data, *this);
            free(data, size);
        }
        data = new_data;
        capacity = new_capacity;
    }

    void pop() {
        if (size == 0) {
            throw std::length_error("Empty stack");
        }
        if (!std::is_trivially_destructible_v<T>) {
            (data + size - 1)->~T();
        }
        --size;
    }

    void push(const T &value) {
        if (size == capacity) {
            resize(capacity * 2 + 1);
        }
        new (data + size) T(value);
        ++size;
    }

    void push(const T &&value) {
        if (size == capacity) {
            resize(capacity * 2 + 1);
        }
        new (data + size) T(std::move(value));
        ++size;
    }

    // Move content of one push_stack to pop_stack
    template <typename Type>
    class MoveStackContent {
    public:
        static void get(Stack<Type> &move_to, Stack<Type> &move_from) {
            if (std::is_move_constructible_v<Type>) {
                while (!move_from.empty()) {
                    move_to.push(std::move(move_from.top()));
                    --move_from.size;
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
    static void get(Stack<MinMaxNode> &move_to, Stack<MinMaxNode> &move_from) {
        while (!move_from.empty()) {
            if (move_to.empty()) {
                MinMaxNode &move_from_top = move_from.top();
                move_to.push(MinMaxNode{move_from_top.self_value, move_from_top.self_value, move_from_top.self_value});
            } else {
                MinMaxNode &move_to_top = move_to.top();
                MinMaxNode &move_from_top = move_from.top();
                move_to.push(MinMaxNode{move_from_top.self_value,
                                        move_to_top.min_value < move_from_top.self_value ? move_to_top.min_value
                                                                                         : move_from_top.self_value,
                                        move_to_top.max_value > move_from_top.self_value ? move_to_top.max_value
                                                                                         : move_from_top.self_value});
            }
            move_from.pop();
        }
    }
};

#endif // MIN_MAX_QUEUE_STACK
