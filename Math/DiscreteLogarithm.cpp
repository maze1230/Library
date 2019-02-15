#include <bits/stdc++.h>

::std::int64_t gcd(::std::int64_t a, ::std::int64_t b) {
	while (b != 0) {
		::std::swap(a, b);
		b = b % a;
	}
	return a;
}

::std::pair<::std::int64_t, ::std::int64_t> extgcd(::std::int64_t a, ::std::int64_t b) {
	::std::pair<::std::int64_t, ::std::int64_t> pa(1, 0), pb(0, 1);
	while (b != 0) {
		::std::swap(a, b); ::std::swap(pa, pb);
		pb = ::std::make_pair(pb.first - pa.first * (b / a), pb.second - pa.second * (b / a));
		b = b % a;
	}
	return pa;
}

::std::int64_t modinv(::std::int64_t a, ::std::int64_t mod) {
	::std::int64_t x = extgcd(a, mod).first;
	if (x < 0) x += mod;
	return x;
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

::std::int64_t sqrt_int(::std::int64_t x) {
	::std::int64_t l = 0, r = x+1, m;
	while (r - l > 1) {
		m = (l + r) >> 1;
		if (m * m > x)
			r = m;
		else
			l = m;
	}
	return l;
}

// return x that a^x ≡ b (mod p)
// 		if there isn't, return -1
// O(sqrt(p) log p)
//
// Require -> gcd, modinv(extgcd), modpow, sqrt_int
::std::int64_t BabyStepGiantStep(::std::int64_t a, ::std::int64_t b, ::std::int64_t p) {
	a %= p; if (a < 0) a += p;
	b %= p; if (b < 0) b += p;

	if (a == 1 && b != 1) return -1;
	if (b == 1) return 0;

	::std::int64_t g, bias = 0;
	while((g = gcd(a, p)) != 1) {
		if (b % g != 0) return -1;
		b /= g;
		p /= g;
		b = b * modinv(a / g, p) % p;
		bias++;
		if (b == 1) return bias;
	}

	::std::int64_t r = sqrt_int(p);
	if (r * r < p) r++;

	::std::map<::std::int64_t, ::std::int64_t> table;

	::std::int64_t a_r = modpow(a, r, p), ainv = modinv(a, p);

	for (::std::int64_t baby = b, i = 0; i < r; i++, baby = baby * ainv % p) {
		table[baby] = ::std::max(table[baby], r - i);
	}

	for (::std::int64_t giant = 1, i = 0; i < r; i++, giant = giant * a_r % p) {
		if (table.count(giant)) {
			return i * r + r - table[giant]; 
		}
	}

	return -1;
}

/*

verify: https://atcoder.jp/contests/arc042/submissions/4271048 

*/
