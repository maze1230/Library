#include <bits/stdc++.h>

::std::int64_t totient(::std::int64_t x) {
	::std::int64_t res = x;
	for (::std::int64_t i = 2; i * i <= x; ++i) {
		if (x % i == 0) {
			res = res / i * (i - 1);
			while (x % i == 0) x /= i;
		}
	}
	if (x != 1) res = res / x * (x - 1);
	return res;
}

::std::vector<::std::pair<::std::int64_t, ::std::int32_t>> primefactors(::std::int64_t x) {
	::std::vector<::std::pair<::std::int64_t, ::std::int32_t>> res;
	for (::std::int64_t i = 2; i*i <= x; ++i) {
		if (x % i == 0) {
			res.emplace_back(i, 1);
			x /= i;
			while (x % i == 0) x /= i, res.back().second++;
		}
	}
	if (x != 1) res.emplace_back(x, 1);
	return res;
}

::std::int64_t modpow(::std::int64_t a, ::std::int64_t b, ::std::int64_t mod) {
	::std::int64_t res = 1;
	while (b > 0) {
		if (b & 1) res = res * a % mod;
		a = a * a % mod;
		b >>= 1;
	}
	return res;
}



// return minimum primitive root modulo p
// O(ans log^2 p + sqrt(p))
//
// Require -> totient, factor, modpow
::std::int64_t primitiveRoot(::std::int64_t p, ::std::int64_t lower = 2) {
	::std::int64_t x = totient(p);
	auto factors = primeFactors(x);
	
	for (::std::int64_t i = lower; i < p; ++i) {
		bool ok = true;
		for (auto f : factors) {
			if (modpow(i, x / f.first, p) == 1) {
				ok = false;
				break;
			}
		}
		if (ok) return i;
	}
	return -1;
}
