#ifndef CUSTOMADS_SRC_DATA_STRUCTURES_AHO_CORASICK_AUTOMATA_AHO_CORASICK_AUTOMATA_HPP_
#define CUSTOMADS_SRC_DATA_STRUCTURES_AHO_CORASICK_AUTOMATA_AHO_CORASICK_AUTOMATA_HPP_

#include <vector>
#include <string>
#include <queue>
#include <cstdint>
#include <limits>
#include <algorithm>

namespace ads {

// TODO: add tests
template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
class AhoCorasickAutomata {
private:
  struct OccurrenceInfo;

public:
  using occurrences = std::vector<OccurrenceInfo>;

  AhoCorasickAutomata()
      : next_str_num_(0),
        nodes_(std::vector<Node>(1)) {}

  void addString(const std::string& s) {
    std::size_t curr_node = 0;
    for (const char& symbol : s) {
      const std::size_t symbol_ind =
          static_cast<std::size_t>(symbol - kAlphaLeft);
      if (nodes_[curr_node].next_[symbol_ind] == kUndefinedFlag) {
        nodes_[curr_node].next_[symbol_ind] = nodes_.size();
        nodes_.emplace_back();
      }
      curr_node = nodes_[curr_node].next_[symbol_ind];
    }
    nodes_[curr_node].is_terminal_ = true;
    nodes_[curr_node].str_num_ = next_str_num_++;
  }

  // Return pairs[index of end position of string in text, string index]
  [[nodiscard]] occurrences findAllOccurrences(const std::string& text) {
    buildAutomata();
    std::size_t curr_node = 0;
    std::vector<OccurrenceInfo> occurences;
    const std::size_t text_size = text.size();
    for (std::size_t i = 0; i < text_size; ++i) {
      curr_node = nodes_[curr_node].next_[text[i] - kAlphaLeft];
      std::size_t traverse_back_node = curr_node;
      do {
        if (nodes_[traverse_back_node].is_terminal_) {
          occurences.push_back(
              OccurrenceInfo{.str_end_pos_ = i,
                             .str_num_ = nodes_[traverse_back_node].str_num_});
        }
        traverse_back_node = nodes_[traverse_back_node].to_terminal_link_;
      } while (traverse_back_node != kNoPathFlag);
    }
    return occurences;
  }

private:
  static constexpr std::int64_t kAlphaSize =
      static_cast<std::int64_t>(kAlphaRight - kAlphaLeft) + 1;
  static constexpr std::size_t kUndefinedFlag =
      std::numeric_limits<std::size_t>::max();
  static constexpr std::size_t kNoPathFlag = kUndefinedFlag - 1;

  // This function must be called after all strings was added
  // Aho-Corasick algorithm implementation
  // Lecture: https://www.youtube.com/watch?v=V7S80KpbQpk&list=LL&index=5&t=2s
  void buildAutomata() {
    nodes_[0].suffix_link_ = kNoPathFlag;
    nodes_[0].to_terminal_link_ = kNoPathFlag;
    for (char c = kAlphaLeft; c <= kAlphaRight; ++c) {
      if (nodes_[0].next_[c - kAlphaLeft] == kUndefinedFlag) {
        nodes_[0].next_[c - kAlphaLeft] = 0;
      }
    }
    std::queue<std::size_t> nodes_queue;
    nodes_queue.push(0);
    while (!nodes_queue.empty()) {
      std::size_t parent = nodes_queue.front();
      nodes_queue.pop();
      for (char c = kAlphaLeft; c <= kAlphaRight; ++c) {
        std::size_t child = nodes_[parent].next_[c - kAlphaLeft];
        if (nodes_[child].suffix_link_ != kUndefinedFlag) {
          continue;
        }
        nodes_[child].suffix_link_ =
            (parent == 0
                 ? 0
                 : nodes_[nodes_[parent].suffix_link_].next_[c - kAlphaLeft]);
        const std::size_t& suff_link_node = nodes_[child].suffix_link_;
        nodes_[child].to_terminal_link_ =
            (nodes_[suff_link_node].is_terminal_
                 ? suff_link_node
                 : nodes_[suff_link_node].to_terminal_link_);
        for (char d = kAlphaLeft; d <= kAlphaRight; ++d) {
          if (nodes_[child].next_[d - kAlphaLeft] != kUndefinedFlag) {
            continue;
          }
          nodes_[child].next_[d - kAlphaLeft] =
              nodes_[nodes_[child].suffix_link_].next_[d - kAlphaLeft];
        }
        nodes_queue.push(child);
      }
    }
  }

  struct Node {
    bool is_terminal_;
    std::size_t str_num_;
    std::size_t suffix_link_;
    std::size_t to_terminal_link_;
    std::size_t next_[kAlphaSize];

    Node()
        : is_terminal_(false),
          str_num_(kUndefinedFlag),
          suffix_link_(kUndefinedFlag),
          to_terminal_link_(kUndefinedFlag) {
      std::fill(next_, next_ + kAlphaSize, kUndefinedFlag);
    }
  };

  struct OccurrenceInfo {
    std::size_t str_end_pos_;
    std::size_t str_num_;
  };

  std::size_t next_str_num_;
  std::vector<Node> nodes_;
};

}  // namespace ads

#endif  // CUSTOMADS_SRC_DATA_STRUCTURES_AHO_CORASICK_AUTOMATA_AHO_CORASICK_AUTOMATA_HPP_
