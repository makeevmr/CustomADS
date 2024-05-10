#ifndef MIN_MAX_QUEUE_QUEUE
#define MIN_MAX_QUEUE_QUEUE
#include "../Stack/Stack.h"

// Queue implementation using two stacks
template <typename T>
class Queue {
private:
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;

    size_type size;
    Stack<T> push_stack;
    Stack<T> pop_stack;

public:
    explicit Queue(const size_type &capacity)
        : size(0), push_stack(Stack<T>(capacity)), pop_stack(Stack<T>(capacity)) {}

    Queue(const Queue<T> &other) = default;

    Queue &operator=(const Queue<T> &other) = default;

    Queue(Queue<T> &&other) noexcept = default;

    Queue &operator=(Queue<T> &&other) noexcept = default;

    ~Queue() = default;

    // Element access
    [[nodiscard]] reference front() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (pop_stack.empty()) {
            return push_stack.bottom();
        }
        return pop_stack.top();
    }

    [[nodiscard]] const_reference front() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (pop_stack.empty()) {
            return push_stack.bottom();
        }
        return pop_stack.top();
    }

    [[nodiscard]] reference back() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (!push_stack.empty()) {
            return push_stack.top();
        }
        return pop_stack.bottom();
    }

    [[nodiscard]] const_reference back() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (!push_stack.empty()) {
            return push_stack.top();
        }
        return pop_stack.bottom();
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return size == 0; }

    [[nodiscard]] std::size_t getSize() const noexcept { return size; }

    // Modifiers
    void pop() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (pop_stack.empty()) {
            Stack<T>::template MoveStackContent<T>::get(pop_stack, push_stack);
        }
        pop_stack.pop();
        --size;
    }

    void push(const T &value) {
        push_stack.push(value);
        ++size;
    }

    void push(T &&value) {
        push_stack.push(value);
        ++size;
    }
};

template <>
class Queue<MinMaxNode> {
private:
    using reference = MinMaxNode &;
    using const_reference = const MinMaxNode &;
    using size_type = std::size_t;

    size_type size;
    Stack<MinMaxNode> push_stack;
    Stack<MinMaxNode> pop_stack;

public:
    explicit Queue(const size_type &capacity)
        : size(0), push_stack(Stack<MinMaxNode>(capacity)), pop_stack(Stack<MinMaxNode>(capacity)) {}

    Queue(const Queue<MinMaxNode> &other) = default;

    Queue &operator=(const Queue<MinMaxNode> &other) = default;

    Queue(Queue<MinMaxNode> &&other) noexcept = default;

    Queue &operator=(Queue<MinMaxNode> &&other) noexcept = default;

    ~Queue() = default;

    // Element access
    [[nodiscard]] reference front() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (pop_stack.empty()) {
            return push_stack.bottom();
        }
        return pop_stack.top();
    }

    [[nodiscard]] const_reference front() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (pop_stack.empty()) {
            return push_stack.bottom();
        }
        return pop_stack.top();
    }

    [[nodiscard]] reference back() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (!push_stack.empty()) {
            return push_stack.top();
        }
        return pop_stack.bottom();
    }

    [[nodiscard]] const_reference back() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (!push_stack.empty()) {
            return push_stack.top();
        }
        return pop_stack.bottom();
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return size == 0; }

    [[nodiscard]] std::size_t getSize() const noexcept { return size; }

    // Requests
    [[nodiscard]] int getMaxDiff() const {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (push_stack.empty()) {
            const MinMaxNode &pop_stack_top = pop_stack.top();
            return pop_stack_top.max_value - pop_stack_top.min_value;
        }
        if (pop_stack.empty()) {
            const MinMaxNode &push_stack_top = push_stack.top();
            return push_stack_top.max_value - push_stack_top.min_value;
        }
        const MinMaxNode &pop_stack_top = pop_stack.top();
        const MinMaxNode &push_stack_top = push_stack.top();
        int max_value =
            push_stack_top.max_value > pop_stack_top.max_value ? push_stack_top.max_value : pop_stack_top.max_value;
        int min_value =
            push_stack_top.min_value < pop_stack_top.min_value ? push_stack_top.min_value : pop_stack_top.min_value;
        return max_value - min_value;
    }

    // Modifiers
    void pop() {
        if (size == 0) {
            throw std::length_error("Empty queue");
        }
        if (pop_stack.empty()) {
            Stack<MinMaxNode>::template MoveStackContent<MinMaxNode>::get(pop_stack, push_stack);
        }
        pop_stack.pop();
        --size;
    }

    void push(int value) {
        if (push_stack.empty()) {
            push_stack.push(MinMaxNode{value, value, value});
        } else {
            MinMaxNode &&node_to_push = {value, value, value};
            const MinMaxNode &top_node = push_stack.top();
            if (top_node.min_value < node_to_push.min_value) {
                node_to_push.min_value = top_node.min_value;
            }
            if (top_node.max_value > node_to_push.max_value) {
                node_to_push.max_value = top_node.max_value;
            }
            push_stack.push(node_to_push);
        }
        ++size;
    }

    void resize(size_type new_capacity) {
        pop_stack.resize(new_capacity);
        push_stack.resize(new_capacity);
    }
};

#endif // MIN_MAX_QUEUE_QUEUE
