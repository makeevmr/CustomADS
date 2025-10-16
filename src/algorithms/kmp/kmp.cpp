#include "kmp.hpp"

namespace ads {

namespace {

[[nodiscard]] std::vector<std::size_t> prefixFunction(const std::string& s) {
  const std::size_t s_size = s.size();
  std::vector<std::size_t> pref_func(s_size);
  for (std::size_t i = 1; i < s_size; ++i) {
    std::size_t j = pref_func[i - 1];
    while (j > 0 && s[i] != s[j]) {
      j = pref_func[j - 1];
    }
    if (s[i] == s[j]) {
      ++j;
    }
    pref_func[i] = j;
  }
  return pref_func;
}

}  // namespace

[[nodiscard]] std::vector<std::size_t> kmpSubstrSearch(
    const std::string& text, const std::string& substr) {
  const std::size_t substr_size = substr.size();
  const std::size_t text_size = text.size();
  const std::vector<std::size_t> pref_func = prefixFunction(substr);
  std::vector<std::size_t> occurrences;
  std::size_t match_len = 0;
  for (std::size_t i = 0; i < text_size; ++i) {
    while (match_len > 0 && text[i] != substr[match_len]) {
      match_len = pref_func[match_len - 1];
    }
    if (text[i] == substr[match_len]) {
      ++match_len;
    }
    if (match_len == substr_size) {
      occurrences.push_back(i + 1 - substr_size);
      match_len = pref_func[match_len - 1];
    }
  }
  return occurrences;
}

}  // namespace ads
