#include "sieve_of_eratosthenes.hpp"

namespace ads {

[[nodiscard]] std::vector<bool> createEratoSieve(const std::size_t& n) {
  std::vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (std::size_t i = 2; i * i <= n; ++i) {
    if (is_prime[i]) {
      for (std::size_t j = i * i; j <= n; j += i) {
        is_prime[j] = false;
      }
    }
  }
  return is_prime;
}

}  // namespace ads
