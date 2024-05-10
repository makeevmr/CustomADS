#ifndef IS_BTREE_MAP
#define IS_BTREE_MAP

#include <cstddef>
#include <iterator>

// Implementation of map using AA Tree
// The comparator must satisfy strict weak ordering relation
// TODO try to implement custom Allocator
template <typename Key, typename T, typename Compare>
class Map {
private:
    struct Node;
    class Iterator;
    class ConstIterator;

public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

    Map()
        : root(nullptr), b_iter(Iterator(nullptr)), e_iter(Iterator(nullptr)), size(0),
          comparator(Compare()) {}

    Map(const Map &other)
        : root(other.root == nullptr ? nullptr : new Node(*other.root)),
          b_iter(Iterator(beginNode(root))), e_iter(nullptr), size(other.size),
          comparator(Compare()) {}

    Map &operator=(const Map &other) {
        if (this != &other) {
            delete root;
            root = nullptr;
            size = 0;
            b_iter = Iterator(nullptr);
            if (other.root != nullptr) {
                root = new Node(*other.root);
                b_iter = Iterator(beginNode(root));
                size = other.size;
            }
        }
        return *this;
    }

    Map(Map &&other) noexcept
        : root(other.root), b_iter(other.b_iter), e_iter(nullptr), size(other.size),
          comparator(Compare()) {
        other.root = nullptr;
        other.b_iter = Iterator(nullptr);
        other.size = 0;
    }

    Map &operator=(Map &&other) noexcept {
        if (this != &other) {
            root = other.root;
            b_iter = other.b_iter;
            size = other.size;
            other.root = nullptr;
            other.b_iter = Iterator(nullptr);
            other.size = 0;
        }
        return *this;
    }

    ~Map() {
        delete root;
        root = nullptr;
        b_iter = Iterator(nullptr);
        size = 0;
    }

    // Iterators
    [[nodiscard]] iterator begin() noexcept { return b_iter; }

    [[nodiscard]] const_iterator begin() const noexcept { return ConstIterator(b_iter); }

    [[nodiscard]] const_iterator cbegin() const noexcept { return ConstIterator(b_iter); }

    [[nodiscard]] iterator end() noexcept { return e_iter; }

    [[nodiscard]] const_iterator end() const noexcept { return ConstIterator(e_iter); }

    [[nodiscard]] const_iterator cend() const noexcept { return ConstIterator(e_iter); }

    // Capacity
    [[nodiscard]] bool empty() const noexcept { return size == 0; }

    [[nodiscard]] size_type getSize() const noexcept { return size; }

    // Modifiers

    // Return a pair consisting of an iterator to the inserted element (or to the element that
    // prevented the insertion) and a bool value set to true if and only if the insertion took
    // place.
    std::pair<iterator, bool> insert(const_reference value) {
        Node *parent = findParent(value.first);
        if ((parent != nullptr) && (parent->value->first == value.first)) {
            return std::pair{Iterator(parent), false};
        }
        Node *new_node = new Node({new value_type(value), nullptr, nullptr, parent, 1});
        ++size;
        if (parent == nullptr) {
            root = new_node;
            b_iter = Iterator(root);
        } else {
            if (comparator(value.first, b_iter->first)) {
                b_iter = Iterator(new_node);
            }
            Node *rebalance_node = nullptr;
            if (comparator(value.first, parent->value->first)) {
                parent->left = new_node;
                rebalance_node = parent;
            } else {
                parent->right = new_node;
                rebalance_node = parent->parent;
            }
            int unchanged_nodes = 0;
            bool is_tree_changed = false;
            while ((rebalance_node != nullptr) && (unchanged_nodes < 3)) {
                is_tree_changed = rebalance_node != skew(rebalance_node);
                is_tree_changed = is_tree_changed || rebalance_node != split(rebalance_node);
                if (!is_tree_changed) {
                    ++unchanged_nodes;
                } else {
                    unchanged_nodes = 0;
                }
                rebalance_node = rebalance_node->parent;
            }
        }
        return std::pair{Iterator(new_node), true};
    }

    std::pair<iterator, bool> insert(value_type &&value) {
        Node *parent = findParent(value.first);
        if ((parent != nullptr) && (parent->value->first == value.first)) {
            return std::pair{Iterator(parent), false};
        }
        Node *new_node = new Node({new value_type(std::move(value)), nullptr, nullptr, parent, 1});
        ++size;
        if (parent == nullptr) {
            root = new_node;
            b_iter = Iterator(root);
        } else {
            if (comparator(value.first, b_iter->first)) {
                b_iter = Iterator(new_node);
            }
            Node *rebalance_node = nullptr;
            if (comparator(value.first, parent->value->first)) {
                parent->left = new_node;
                rebalance_node = parent;
            } else {
                parent->right = new_node;
                rebalance_node = parent->parent;
            }
            int unchanged_nodes = 0;
            bool is_tree_changed = false;
            while ((rebalance_node != nullptr) && (unchanged_nodes < 3)) {
                is_tree_changed = rebalance_node != skew(rebalance_node);
                is_tree_changed = is_tree_changed || rebalance_node != split(rebalance_node);
                if (!is_tree_changed) {
                    ++unchanged_nodes;
                } else {
                    unchanged_nodes = 0;
                }
                rebalance_node = rebalance_node->parent;
            }
        }
        return std::pair{Iterator(new_node), true};
    }

    // TODO change function signature
    void erase(const key_type &erased_key) noexcept {
        if (root == nullptr) {
            return;
        }
        Node *current_node = findNode(erased_key);
        if (current_node != nullptr) {
            Node *rebalance_node = nullptr;
            if (current_node->left == nullptr && current_node->right == nullptr) {
                rebalance_node = trivialNodeErase(current_node, nullptr);
            } else if (current_node->left != nullptr && current_node->right == nullptr) {
                rebalance_node = trivialNodeErase(current_node, current_node->left);
            } else if (current_node->left == nullptr && current_node->right != nullptr) {
                rebalance_node = trivialNodeErase(current_node, current_node->right);
            } else {
                Node *next_node = next(current_node);
                Node *right_child = next_node->right;
                rebalance_node = next_node->parent;
                if (right_child != nullptr) {
                    right_child->parent = rebalance_node;
                }
                if (rebalance_node->left == next_node) {
                    rebalance_node->left = right_child;
                } else {
                    rebalance_node->right = right_child;
                }
                std::swap(current_node->value, next_node->value);
                current_node->value->second = std::move(next_node->value->second);
                --size;
                next_node->left = nullptr;
                next_node->right = nullptr;
                next_node->parent = nullptr;
                delete next_node;
            }
            bool is_level_changed = true;
            while ((rebalance_node != nullptr) && is_level_changed) {
                size_type init_level = rebalance_node->level;
                decreaseNodeLevel(rebalance_node);
                is_level_changed = init_level != rebalance_node->level;
                if (is_level_changed) {
                    rebalance_node = skew(rebalance_node);
                    if (rebalance_node->right != nullptr) {
                        skew(rebalance_node->right);
                        if (rebalance_node->right->right != nullptr) {
                            skew(rebalance_node->right->right);
                        }
                    }
                    rebalance_node = split(rebalance_node);
                    if (rebalance_node->right != nullptr) {
                        split(rebalance_node->right);
                    }
                }
                rebalance_node = rebalance_node->parent;
            }
        }
    }

    // Lookup
    iterator find(const key_type &search_key) noexcept { return Iterator(findNode(search_key)); }

    const_iterator find(const key_type &search_key) const noexcept {
        return ConstIterator(findNode(search_key));
    }

    [[nodiscard]] bool contains(const key_type &key) const noexcept {
        return findNode(key) != nullptr;
    }

private:
    Node *root;
    iterator b_iter;
    iterator e_iter;
    size_type size;
    key_compare comparator;

    struct Node {
        Map::pointer value;
        Node *left;
        Node *right;
        Node *parent;
        Map::size_type level;

        Node(Map::pointer value, Node *left, Node *right, Node *parent, Map::size_type level)
            : value(value), left(left), right(right), parent(parent), level(level) {}

        Node(const Node &other)
            : value(new value_type(*(other.value))), left(nullptr), right(nullptr), parent(nullptr),
              level(other.level) {
            if (other.left != nullptr) {
                left = new Node(*other.left);
                left->parent = this;
            }
            if (other.right != nullptr) {
                right = new Node(*other.right);
                right->parent = this;
            }
        }

        ~Node() {
            delete value;
            value = nullptr;
            delete left;
            delete right;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            level = 0;
        }
    };

    class Iterator {
    private:
        friend Map::ConstIterator;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = Map::difference_type;
        using value_type = Map::value_type;
        using reference = Map::reference;
        using pointer = Map::pointer;

        explicit Iterator(Node *ptr) noexcept : ptr(ptr){};

        Iterator(const Iterator &other) noexcept : ptr(other.ptr){};

        Iterator &operator=(const Iterator &other) &noexcept {
            if (this != &other) {
                ptr = other.ptr;
            }
            return *this;
        }

        ~Iterator(){};

        [[nodiscard]] reference operator*() const noexcept { return *(ptr->value); }

        [[nodiscard]] pointer operator->() const noexcept { return ptr->value; }

        Iterator &operator++() {
            ptr = Map::next(ptr);
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator &operator--() {
            ptr = Map::prev(ptr);
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        [[nodiscard]] bool operator==(const Iterator &right) const noexcept {
            return ptr == right.ptr;
        }

        [[nodiscard]] bool operator!=(const Iterator &right) const noexcept {
            return ptr != right.ptr;
        }

    private:
        Node *ptr;
    };

    class ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = Map::difference_type;
        using value_type = Map::value_type;
        using reference = Map::const_reference;
        using pointer = Map::const_pointer;

        explicit ConstIterator(Node *ptr) noexcept : ptr(ptr){};

        explicit ConstIterator(const Iterator &other) noexcept : ptr(other.ptr){};

        ConstIterator(const ConstIterator &other) noexcept : ptr(other.ptr){};

        ConstIterator &operator=(const ConstIterator &other) &noexcept {
            if (this != &other) {
                ptr = other.ptr;
            }
            return *this;
        }

        ~ConstIterator(){};

        [[nodiscard]] reference operator*() const noexcept { return *(ptr->value); }

        [[nodiscard]] pointer operator->() const noexcept { return ptr->value; }

        ConstIterator &operator++() {
            ptr = Map::next(ptr);
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        ConstIterator &operator--() {
            ptr = Map::prev(ptr);
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator tmp = *this;
            --(*this);
            return tmp;
        }

        [[nodiscard]] bool operator==(const ConstIterator &right) const noexcept {
            return ptr == right.ptr;
        }

        [[nodiscard]] bool operator!=(const ConstIterator &right) const noexcept {
            return ptr != right.ptr;
        }

    private:
        Node *ptr;
    };

    Node *skew(Node *node) noexcept {
        if ((node->left == nullptr) || (node->level != node->left->level)) {
            return node;
        }
        Node *left_node = node->left;
        node->left = left_node->right;
        if (left_node->right != nullptr) {
            left_node->right->parent = node;
        }
        left_node->right = node;
        left_node->parent = node->parent;
        if (node->parent != nullptr) {
            if (node->parent->left == node) {
                node->parent->left = left_node;
            } else {
                node->parent->right = left_node;
            }
        }
        node->parent = left_node;
        if (root == node) {
            root = left_node;
        }
        return left_node;
    }

    Node *split(Node *node) noexcept {
        if (node->right == nullptr || node->right->right == nullptr ||
            node->level != node->right->right->level) {
            return node;
        }
        Node *right_node = node->right;
        node->right = right_node->left;
        if (right_node->left != nullptr) {
            right_node->left->parent = node;
        }
        right_node->left = node;
        right_node->parent = node->parent;
        if (node->parent != nullptr) {
            if (node->parent->left == node) {
                node->parent->left = right_node;
            } else {
                node->parent->right = right_node;
            }
        }
        node->parent = right_node;
        if (root == node) {
            root = right_node;
        }
        ++right_node->level;
        return right_node;
    }

    // Deleting a node in case of less than two children
    // return parent of erased node
    [[nodiscard]] Node *trivialNodeErase(Node *node_to_erase, Node *child) noexcept {
        if (child != nullptr) {
            child->parent = node_to_erase->parent;
        }
        Node *parent = nullptr;
        if (root != node_to_erase) {
            parent = node_to_erase->parent;
            if (parent->left == node_to_erase) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        } else {
            root = child;
        }
        if (b_iter == Iterator(node_to_erase)) {
            ++b_iter;
        }
        --size;
        node_to_erase->left = nullptr;
        node_to_erase->right = nullptr;
        node_to_erase->parent = nullptr;
        delete node_to_erase;
        return parent;
    }

    void static decreaseNodeLevel(Node *node) noexcept {
        size_type left_diff = node->left != nullptr ? node->level - node->left->level : node->level;
        size_type right_diff =
            node->right != nullptr ? node->level - node->right->level : node->level;
        if (left_diff > 1 || right_diff > 1) {
            if (node->right != nullptr && node->right->level == node->level) {
                --node->right->level;
            }
            --node->level;
        }
    }

    [[nodiscard]] static Node *next(const Node *node) noexcept {
        if (node->right != nullptr) {
            Node *current_node = node->right;
            while (current_node->left != nullptr) {
                current_node = current_node->left;
            }
            return current_node;
        }
        Node *parent = node->parent;
        while (parent != nullptr && (parent->right == node)) {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }

    [[nodiscard]] static Node *prev(const Node *node) noexcept {
        if (node->left != nullptr) {
            Node *current_node = node->left;
            while (current_node->right != nullptr) {
                current_node = current_node->right;
            }
            return current_node;
        }
        Node *parent = node->parent;
        while (parent != nullptr && (parent->left == node)) {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }

    // return a pointer to the node containing the key, return nullptr if such a key does not exist
    // TODO add noexcept condition for Compare class
    [[nodiscard]] Node *findNode(const key_type &search_key) const noexcept {
        Node *node = root;
        while (node != nullptr) {
            const key_type key = node->value->first;
            if (!(comparator(key, search_key)) && !(comparator(search_key, key))) {
                break;
            }
            if (comparator(search_key, key)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return node;
    }

    // return a pointer to the node containing the key, otherwise return the parent of the new
    // inserted node (return nullptr if map is empty)
    [[nodiscard]] Node *findParent(const key_type &search_key) const noexcept {
        Node *parent = nullptr;
        Node *node = root;
        while (node != nullptr) {
            parent = node;
            const key_type key = node->value->first;
            if (!(comparator(key, search_key)) && !(comparator(search_key, key))) {
                break;
            }
            if (comparator(search_key, key)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        if (node != nullptr || size == 0) {
            return node;
        }
        return parent;
    }

    // Pointer to leftmost node
    [[nodiscard]] static Node *beginNode(Node *node) noexcept {
        if (node == nullptr) {
            return nullptr;
        }
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
};

#endif // IS_BTREE_MAP
