#include <bits/stdc++.h>

template<::std::uint_fast64_t mod>
class ModInt{
private:
	using value_type = ::std::uint_fast64_t;
	value_type n;
public:
	ModInt() : n(0) {}
	ModInt(value_type n_) : n(n_ % mod) {}
	ModInt(const ModInt& m) : n(m.n) {}

	explicit operator value_type() const { return n; }
	template<::std::uint_fast64_t mod_>
	friend ::std::ostream& operator<<(::std::ostream&, const ModInt<mod_>&);
	template<::std::uint_fast64_t mod_>
	friend ::std::istream& operator>>(::std::istream&, const ModInt<mod_>&);

	bool operator==(const ModInt& m) const { return n == m.n; }
	bool operator!=(const ModInt& m) const { return n != m.n; }
	ModInt& operator*=(const ModInt& m){ n = n * m.n % mod; return *this; }
	
	ModInt operator++(int){
		ModInt old(n);
		n = (n + 1) % mod;
		return old;
	}
	ModInt operator--(int){
		ModInt old(n);
		n = (n - 1 + mod) % mod;
		return old;
	}

	ModInt pow(value_type b) const{
		ModInt ans = 1, m = ModInt(*this);
		while(b){
			if(b & 1) ans *= m;
			m *= m;
			b >>= 1;
		}
		return ans;
	}

	ModInt inv() const { return (*this).pow(mod-2); }
	ModInt& operator++(){ n = (n + 1) % mod; return *this; }
	ModInt& operator--(){ n = (n - 1 + mod) % mod; return *this; }
	ModInt& operator+=(const ModInt& m){ n += m.n; n = (n < mod ? n : n - mod); return *this; }
	ModInt& operator-=(const ModInt& m){ n += mod - m.n; n = (n < mod ? n : n - mod); return *this; }
	ModInt& operator/=(const ModInt& m){ n = n * m.inv() % mod; return *this; }
	ModInt operator+(const ModInt& m) const { return ModInt(*this) += m; }
	ModInt operator-(const ModInt& m) const { return ModInt(*this) -= m; }
	ModInt operator*(const ModInt& m) const { return ModInt(*this) *= m; }
	ModInt operator/(const ModInt& m) const { return ModInt(*this) /= m; }
};

template<::std::uint_fast64_t mod>
::std::ostream& operator<<(::std::ostream& stream, const ModInt<mod>& m){
	stream << m.n;
	return stream;
}

template<::std::uint_fast64_t mod>
::std::istream& operator>>(::std::istream& stream, const ModInt<mod>& m){
	stream >> m.n;
	return stream;
}

template<::std::size_t size, ::std::uint_fast64_t mod=1000000007>
class Factorial{
private:
	using value_type = ModInt<mod>;
	::std::vector<value_type> fact, inv;
public:
	Factorial() : fact(size+1, 1), inv(size+1, 1){
		for(::std::size_t i = 1; i <= size; ++i){
			fact[i] = fact[i-1] * value_type(i);
			inv[i] = fact[i].inv();
		}
	}

	value_type comb(::std::int64_t a, ::std::int64_t b){
		assert(a >= b);
		assert(b >= 0);
		return fact[a]*inv[b]*inv[a-b];
	}

	value_type& operator[](::std::size_t k){ return fact[k]; }
};