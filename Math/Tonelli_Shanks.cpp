#include <bits/stdc++.h>

::std::int64_t modpow(::std::int64_t a, ::std::int64_t b, ::std::int64_t mod) {
  ::std::int64_t res = 1;
  while (b > 0) {
    if (b & 1) res = res * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return res;
}

std::int64_t tonelli_shanks(std::int64_t x, std::int64_t p, std::int64_t prim_root = -1) {
  if (modpow(x, (p-1)/2, p) == -1) return -1;
  if (p % 4 == 3) {
    return modpow(x, (p+1)/4);
  }


}
