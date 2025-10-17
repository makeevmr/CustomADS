#include "euclidean.hpp"

#include <utility>

namespace ads {

std::uint64_t gcd(std::uint64_t a, std::uint64_t b) {
  while (b != 0) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

std::uint64_t lcm(std::uint64_t a, std::uint64_t b) {
  return (a * b) / gcd(a, b);
}

}  // namespace ads
