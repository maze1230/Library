#include <bits/stdc++.h>

// return x is prime
// O(sqrt x)
bool isPrime(::std::int64_t x){
	for (::std::int64_t i = 2; i*i <= x; ++i) {
		if (x % i == 0) return false;
	}
	return true;
}



// return vector of primes from 2 to upper
// O(upper sqrt upper)
::std::vector<::std::int64_t> primes(::std::int64_t upper){
	::std::vector<::std::int64_t> res;
	for (::std::int64_t i = 2; i <= upper; ++i) {
		bool isPrime = true;
		for (auto x : res) {
			if (x * x > upper) break;
			if (i % x == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime)
			res.push_back(i);
	}
	return res;
}



// return vector of prime factors of x
// O(sqrt x)
::std::vector<::std::pair<::std::int64_t, ::std::int32_t>> primeFactors(::std::int64_t x) {
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

// return map of prime factors of x
// O(sqrt x + N log N?) (N is the number of kinds of prime factors?)
::std::map<::std::int64_t, ::std::int32_t> primeFactors(::std::int64_t x) {
	::std::map<::std::int64_t, ::std::int32_t> res;
	for (::std::int64_t i = 2; i*i <= x; ++i) {
		if (x % i == 0) {
			while (x % i == 0) x /= i, res[i]++;
		}
	}
	if (x != 1) res[x]++;
	return res;
}



// return vector of divisors of x
// O(sqrt x + N log N) (N is the number of factors)
::std::vector<::std::int64_t> divisors(::std::int64_t x) {
	::std::vector<::std::int64_t> res;
	for (::std::int64_t i = 1; i*i <= x; ++i) {
		if (x % i == 0) {
			res.push_back(i);
			if (i * i != x) res.push_back(x / i);
		}
	}
	::std::sort(res.begin(), res.end());
	return res;
}



// return whether x is the primitive root modulo n or not
// O(sqrt x)
//
// Require -> divisors, modpow
bool isPrimitiveRoot(::std::int64_t x, ::std::int64_t n) {
	auto divs = divisors(n - 1);
	for (auto d : divs) {
		if (d == p - 1) break;
		if (modpow(x, d, n) == 1) return false;
	}
	return true;
}



// return gcd
// O(log max(a, b))
::std::int64_t gcd(::std::int64_t a, ::std::int64_t b) {
	while (b != 0) {
		::std::swap(a, b);
		a = a % b;
	}
	return a;
}



// return lcm
// O(log max(a, b))
//
// Require -> gcd
::std::int64_t lcm(::std::int64_t a, ::std::int64_t b) {
	return a / gcd(a, b) * b;
}



// return pair of x, y that ax + by = gcd(a, b)
// O(log max(a, b))
::std::pair<::std::int64_t, ::std::int64_t> extgcd(::std::int64_t a, ::std::int64_t b) {
	::std::pair<::std::int64_t, ::std::int64_t> pa(1, 0), pb(0, 1);
	while (b != 0) {
		::std::swap(a, b); ::std::swap(pa, pb);
		pb = ::std::make_pair(pb.first - pa.first * (b / a), pb.second - pa.second * (b / a));
		b = b % a;
	}
	return pa;
}



// return a^-1 mod (mod)
// O(log mod)
// 
// Require -> extgcd
::std::int64_t modinv(::std::int64_t a, ::std::int64_t mod) {
	::std::int64_t x = extgcd(a, mod).first;
	if (x < 0) x += mod;
	return x;
}



// return a^b mod (mod)
// O(log mod)
::std::int64_t modpow(::std::int64_t a, ::std::int64_t b, ::std::int64_t mod) {
	::std::int64_t res = 1;
	while (b > 0) {
		if (b & 1) res = res * a % mod;
		a = a * a % mod;
		b >>= 1;
	}
	return res;
}
