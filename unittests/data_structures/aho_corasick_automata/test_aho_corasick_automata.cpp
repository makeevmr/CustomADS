#include <gtest/gtest.h>

#include "data_structures/aho_corasick_automata/aho_corasick_automata.hpp"

typedef ads::AhoCorasickAutomata<'a', 'z'> LetterAhoCorasickAutomata;

TEST(AhoCorasickAutomata, SimpleTest) {
  LetterAhoCorasickAutomata automata;
  automata.addString("he");
  automata.addString("she");
  automata.addString("hers");
  automata.addString("his");
  const std::string text = "ahishers";
  LetterAhoCorasickAutomata::occurrences occurrences =
      automata.findAllOccurrences(text);
  LetterAhoCorasickAutomata::occurrences expected_occurrences = {
      {3, 3}, {5, 1}, {5, 0}, {7, 2}};
  EXPECT_EQ(occurrences.size(), expected_occurrences.size());
  for (std::size_t i = 0; i < occurrences.size(); ++i) {
    EXPECT_EQ(occurrences[i].str_end_pos_,
              expected_occurrences[i].str_end_pos_);
    EXPECT_EQ(occurrences[i].str_num_, expected_occurrences[i].str_num_);
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
