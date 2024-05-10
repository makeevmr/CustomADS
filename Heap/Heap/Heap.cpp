#include "Heap.h"

template <typename T, typename Compare>
void Heap<T, Compare>::swap(Heap<T, Compare> &other) noexcept {
    std::swap(data, other.data);
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
}

template <typename T, typename Compare>
void Heap<T, Compare>::free(value_type *data_to_free, size_type destructor_calls) noexcept {
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
void Heap<T, Compare>::uninitializedCopy(value_type *copy_to, const Heap<T, Compare> &copy_from) {
    size_type copied_objects = 0;
    try {
        for (; copied_objects < copy_from.size; ++copied_objects) {
            new (copy_to + copied_objects) value_type(copy_from.data[copied_objects]);
        }
    } catch (...) {
        free(copy_to, copied_objects);
        throw;
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::uninitializedCopy(value_type *copy_to, const value_type *copy_from,
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
[[nodiscard]] typename Heap<T, Compare>::size_type
Heap<T, Compare>::getLeft(size_type index) const noexcept {
    return 2 * index + 1;
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type
Heap<T, Compare>::getRight(size_type index) const noexcept {
    return 2 * index + 2;
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type
Heap<T, Compare>::getParent(size_type index) const noexcept {
    return (index - 1) / 2;
}

template <typename T, typename Compare>
void Heap<T, Compare>::resize() {
    size_type new_capacity = capacity > 0 ? capacity * 2 : capacity + 1;
    value_type *new_data =
        reinterpret_cast<value_type *>(::operator new(sizeof(value_type) * new_capacity));
    if (std::is_move_constructible_v<value_type>) {
        size_type moved_objects = 0;
        while (moved_objects < size) {
            new (new_data + moved_objects) value_type(std::move(data[moved_objects]));
            ++moved_objects;
        }
        ::operator delete(data);
    } else {
        uninitializedCopy(new_data, *this);
        free(data, size);
    }
    data = new_data;
    capacity = new_capacity;
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
        if ((left < size) && (comparator(data[left], data[iter_end_index]))) {
            iter_end_index = left;
        }
        if ((right < size) && (comparator(data[right], data[iter_end_index]))) {
            iter_end_index = right;
        }
        if (iter_start_index != iter_end_index) {
            std::swap(data[iter_start_index], data[iter_end_index]);
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
        if (comparator(data[index], data[parent])) {
            std::swap(data[index], data[parent]);
            index = parent;
            is_sifting_complete = false;
        }
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::makeHeap() noexcept(std::is_nothrow_swappable_v<T>) {
    for (size_type i = (size / 2 - 1); i > 0; --i) {
        siftingDown(i);
    }
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap() : data(nullptr), size(0), capacity(0), comparator(Compare()) {}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const size_type capacity)
    : data(reinterpret_cast<value_type *>(::operator new(sizeof(value_type) * capacity))), size(0),
      capacity(capacity), comparator(Compare()) {}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(value_type *construct_from_data, size_type size)
    : data(reinterpret_cast<value_type *>(::operator new(sizeof(value_type) * size))), size(size),
      capacity(size), comparator(Compare()) {
    uninitializedCopy(data, construct_from_data, size);
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const Heap<T, Compare> &other)
    : data(reinterpret_cast<value_type *>(::operator new(sizeof(value_type) * other.capacity))),
      size(other.size), capacity(other.capacity), comparator(Compare()) {
    uninitializedCopy(data, other);
}

template <typename T, typename Compare>
Heap<T, Compare> &Heap<T, Compare>::operator=(const Heap<T, Compare> &other) {
    if (this != &other) {
        value_type *new_data =
            reinterpret_cast<value_type *>(::operator new(sizeof(value_type) * other.capacity));
        uninitializedCopy(new_data, other);
        free(data, size);
        data = new_data;
        size = other.size;
        capacity = other.capacity;
    }
    return *this;
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(Heap &&other) noexcept
    : data(nullptr), size(0), capacity(0), comparator(Compare()) {
    swap(other);
}

template <typename T, typename Compare>
Heap<T, Compare> &Heap<T, Compare>::operator=(Heap<T, Compare> &&other) noexcept {
    if (this != &other) {
        free(data, size);
        data = nullptr;
        size = 0;
        capacity = 0;
        swap(other);
    }
    return *this;
}

template <typename T, typename Compare>
Heap<T, Compare>::~Heap() {
    free(data, size);
}

// Element access
template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::const_reference Heap<T, Compare>::top() const {
    if (size == 0) {
        throw std::length_error("Heap is empty");
    }
    return data[0];
}

// Capacity
template <typename T, typename Compare>
[[nodiscard]] bool Heap<T, Compare>::empty() const noexcept {
    return size == 0;
}

template <typename T, typename Compare>
[[nodiscard]] typename Heap<T, Compare>::size_type Heap<T, Compare>::getSize() const noexcept {
    return size;
}

// Modifiers
template <typename T, typename Compare>
void Heap<T, Compare>::push(const value_type &value) {
    if (size == capacity) {
        this->resize();
    }
    new (data + size) value_type(value);
    ++size;
    siftingUp(size - 1);
}

template <typename T, typename Compare>
void Heap<T, Compare>::push(const value_type &&value) {
    if (size == capacity) {
        this->resize();
    }
    new (data + size) value_type(std::move(value));
    ++size;
    siftingUp(size - 1);
}

template <typename T, typename Compare>
void Heap<T, Compare>::pop() {
    if (size == 0) {
        throw std::length_error("Heap is empty");
    }
    data[0] = std::move(data[size - 1]);
    if (!std::is_trivially_destructible_v<T>) {
        (data + size - 1)->~value_type();
    }
    --size;
    siftingDown(0);
}

template class Heap<int, MoreCompare<int>>;
