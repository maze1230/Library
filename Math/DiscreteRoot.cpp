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



// return x that x^a ≡ b (mod p)
// Usage : if you have primitive root, give it as 4th argument
// O(sqrt(p) log p)
//
// Require -> BabyStepGiantStep(gcd, modinv(extgcd), modpow, sqrt_int), primitiveRoot(totient, primeFactors, modpow)
::std::int64_t discreteRoot(::std::int64_t a, ::std::int64_t b, ::std::int64_t p, ::std::int64_t proot = -1) {
	if (proot == -1) proot = primitiveRoot(p);
	if (proot == -1) return -1;

	::std::int64_t res = BabyStepGiantStep(modpow(proot, a, p), b, p); 
	if (res == -1) return -1;
	return modpow(proot, res, p);
}
