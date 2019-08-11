#include <bits/stdc++.h>

::std::int64_t gcd(::std::int64_t a, ::std::int64_t b) {
	while (b != 0) {
		::std::swap(a, b);
		b = b % a;
	}
	return a;
}

::std::int64_t lcm(::std::int64_t a, ::std::int64_t b) {
	return a / gcd(a, b) * b;
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


// return x (mod lcm(m_i)) and lcm(m_i) that satisfies x ≡ b_i (mod m_i) (中国剰余定理)
// if there isn't the answer, return (-1, -1)
// O(N log M)
::std::pair<::std::int64_t, ::std::int64_t> CRT(const ::std::vector<::std::int64_t> &b, ::std::vector<::std::int64_t> &m) {
	::std::int64_t ret = 0, M = 1;
	for (::std::size_t i = 0; i < b.size(); ++i) {
		::std::int64_t p, q;
		::std::int64_t d = gcd(M, m[i]);
		::std::tie(p, q) = extgcd(M, m[i]);
		if ((b[i] - ret) % d != 0) return ::std::make_pair(-1, -1);
		::std::int64_t tmp = (b[i] - ret) / d * p % (m[i] / d);
		ret += M * tmp;
		M *= m[i] / d;
	}
	return ::std::make_pair((ret + M) % M, M);
}



// Preprocessing for Garner algorithm
// make the elements of m coprime
// O(N^2)
std::int64_t pre_garner(std::vector<std::int64_t>& b, std::vector<std::int64_t>& m, const std::int64_t MOD) {
	std::int64_t res = 1;
	for (int i = 0; i < b.size(); i++) {
		for (int j = i + 1; j < b.size(); j++) {
			std::int64_t g = gcd(m[i], m[j]);

			if ((b[i] - b[j]) % g != 0) return -1;

			m[i] /= g; m[j] /= g;
			std::int64_t gi = gcd(m[i], g), gj = g / gi;
			do {
				g = gcd(gi, gj);
				gi *= g; gj /= g;
			} while (g != 1);

			m[i] *= gi; m[j] *= gj;
			b[i] %= m[i]; b[j] %= m[j];
		}
		res = (res * m[i]) % MOD;
	}
	return res;
}

// return x mod MOD
// It must be guranteed that all elements of m are coprime
// O(N^2)
std::int64_t garner(const std::vector<std::int64_t>& b, const std::vector<std::int64_t>& m, const std::int64_t MOD) {
	std::vector<std::int64_t> coeffs(b.size()+1, 1); 
	std::vector<std::int64_t> constants(b.size()+1, 0); 
	for (int i = 0; i < b.size(); ++i) {
		std::int64_t p, inv;
		std::tie(inv, p) = extgcd(coeffs[i], m[i]);
		std::int64_t t = (b[i] - constants[i]) * inv % m[i];
		if (t < 0) t += m[i];
		for (int j = i+1; j < b.size(); ++j) {
			constants[j] = (constants[j] + coeffs[j] * t) % m[j];
			coeffs[j] = (coeffs[j] * m[i]) % m[j];
		}
		constants.back() = (constants.back() + coeffs.back() * t) % MOD;
		coeffs.back() = (coeffs.back() * m[i]) % MOD;
	}
	return constants.back();
}