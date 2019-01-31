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

	friend ::std::ostream& operator<<(::std::ostream &os, const ModInt<mod> &a) {
		return os << a.n;
	}

	friend ::std::istream& operator>>(::std::istream &is, ModInt<mod> &a) {
		value_type x;
		is >> x;
		a = ModInt<mod>(x);
		return is;
	}

	bool operator==(const ModInt& m) const { return n == m.n; }
	bool operator!=(const ModInt& m) const { return n != m.n; }
	ModInt& operator*=(const ModInt& m){ n = n * m.n % mod; return *this; }

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
	ModInt& operator+=(const ModInt& m){ n += m.n; n = (n < mod ? n : n - mod); return *this; }
	ModInt& operator-=(const ModInt& m){ n += mod - m.n; n = (n < mod ? n : n - mod); return *this; }
	ModInt& operator/=(const ModInt& m){ n = n * m.inv() % mod; return *this; }
	ModInt operator+(const ModInt& m) const { return ModInt(*this) += m; }
	ModInt operator-(const ModInt& m) const { return ModInt(*this) -= m; }
	ModInt operator*(const ModInt& m) const { return ModInt(*this) *= m; }
	ModInt operator/(const ModInt& m) const { return ModInt(*this) /= m; }
	ModInt& operator++(){ n += 1; return *this; }
	ModInt& operator--(){ n -= 1; return *this; }
	ModInt operator++(int){
		ModInt old(n);
		n += 1;
		return old;
	}
	ModInt operator--(int){
		ModInt old(n);
		n -= 1;
		return old;
	}
};
