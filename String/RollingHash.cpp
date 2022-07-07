#include <bits/stdc++.h>

class RollingHash {
private:
  static constexpr uint64_t mod = (1 << 61) - 1;
  const uint64_t base;
  std::vector<uint64_t> bpow;

  string S;
  std::vector<uint64_t> hash;

  static inline uint64_t generate_base() {
    
  }
public:
  RollingHash(const string& s, uint64_t base = generate_base()): S(s), base(base) {
    build();
  }

  void build() {

  }

  uint64_t get_base() {
    return base;
  }
};