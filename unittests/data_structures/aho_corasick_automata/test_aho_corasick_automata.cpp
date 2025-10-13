#include <unordered_map>
#include <unordered_set>

#include <gtest/gtest.h>

#include "data_structures/aho_corasick_automata/aho_corasick_automata.hpp"

typedef ads::AhoCorasickAutomata<'a', 'z'> LetterAhoCorasickAutomata;

void expectSetEquality(
    const LetterAhoCorasickAutomata::occurrences& occurrences,
    const std::unordered_map<std::size_t, std::unordered_set<std::size_t>>&
        expected_occurrences) {
  std::size_t expected_occurrences_total_size = 0;
  for (const auto& [_, set] : expected_occurrences) {
    expected_occurrences_total_size += set.size();
  }
  EXPECT_EQ(occurrences.size(), expected_occurrences_total_size);
  for (const auto& occurrence : occurrences) {
    auto f_iter = expected_occurrences.find(occurrence.str_start_pos_);
    EXPECT_TRUE(f_iter != expected_occurrences.end());
    EXPECT_TRUE(f_iter->second.contains(occurrence.str_num_));
  }
}

TEST(AhoCorasickAutomata, SimpleTest) {
  LetterAhoCorasickAutomata automata;
  automata.addString("he");
  automata.addString("she");
  automata.addString("hers");
  const std::string text = "ahishers";
  std::unordered_map<std::size_t, std::unordered_set<std::size_t>>
      expected_occurrences;
  expected_occurrences[4].insert(0);
  expected_occurrences[3].insert(1);
  expected_occurrences[4].insert(2);
  expectSetEquality(automata.findAllOccurrences(text), expected_occurrences);
  automata.addString("his");
  expected_occurrences[1].insert(3);
  expectSetEquality(automata.findAllOccurrences(text), expected_occurrences);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
