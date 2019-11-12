#include <bits/stdc++.h>

// ryuhei san
#include <unistd.h>
struct IO {
  static const int bufsize=1<<24;
  char ibuf[bufsize], obuf[bufsize];
  char *ip, *op;
  IO(): ip(ibuf), op(obuf) { for(int t = 0, k = 0; (k = read(STDIN_FILENO, ibuf+t, sizeof(ibuf)-t)) > 0; t+=k); }
  ~IO(){ for(int t = 0, k = 0; (k = write(STDOUT_FILENO, obuf+t, op-obuf-t)) > 0; t+=k); }
  long long scan_int(){
    long long x=0;
    bool neg=false;
    for(;*ip<'+';ip++) ;
    if(*ip=='-'){ neg=true; ip++;}
    else if(*ip=='+'){ ip++;}
    for(;*ip>='0';ip++){
      x = 10*x+*ip-'0';
    }
    if(neg) x = -x;
    return x;
  }
  void put_int(long long x, char c=0){
    static char tmp[20];
    if(x==0){
      *op++ = '0';
    }
    else {
      int i;
      if(x<0){
        *op++ = '-';
        x = -x;
      }
      for(i=0; x; i++){
        tmp[i] = x % 10;
        x /= 10;
      }
      for(i--; i>=0; i--)
        *op++ = tmp[i]+'0';
    }
    if(c) *op++ = c;
  }
  void put_double(double x, char c=0){
    unsigned y;
    const int mask = (1<<24) - 1;
    put_int(x);
    *op++ = '.';
    x = x - (int) x;
    if(x < 0) x = -x;
    y = x * (1<<24);

    for(int i=0;i<7;i++){
      y *= 10;
      *op++ = '0' + (y>>24);
      y &= mask;
    }
  }
  inline char scan_char(){ return *ip++; }
  inline void put_char(char c){ *op++ = c; }
  inline char *scan_string(){ char *s = ip; while(*ip++!='\n'){} *(ip-1)='\0'; return s;}
  inline void put_string(char *s, char c=0){ while(*s) *op++=*s++; if(c) *op++=c;}
} io;
// ------

namespace fps {
  template<::std::uint_fast64_t mod>
  class ModInt{
  private:
    using value_type = ::std::uint_fast64_t;
    value_type n;
  public:
    ModInt() : n(0) {}
    ModInt(value_type n_) : n(n_ % mod) {}
    ModInt(const ModInt& m) : n(m.n) {}

    template<typename T>
    explicit operator T() const { return static_cast<T>(n); }
    value_type get() const { return n; }

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
    ModInt& operator/=(const ModInt& m){ *this *= m.inv(); return *this; }
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
    ModInt operator-() const { return ModInt(mod-n); }
  };

  template<std::uint64_t MOD, std::uint64_t PRIM_ROOT>
  class NTT {
  public:
    using Mint = ModInt<MOD>;
    using value_type = Mint;
    using size_type = std::size_t;

    static void ntt(std::vector <value_type> &a, bool inv = false) {
      const std::int64_t n = a.size();
      std::vector <value_type> tmp(n);
      for (std::int64_t s = 1, m = n >> 1; m > 0; s <<= 1, m >>= 1) {
        value_type zeta = 1, e;
        if (!inv) e = value_type(PRIM_ROOT).pow((MOD - 1) / m / 2);
        else e = value_type(PRIM_ROOT).pow(MOD - (MOD - 1) / m / 2 - 1);
        for (std::int64_t p = 0; p < m; p++) {
          for (std::int64_t q = 0; q < s; q++) {
            tmp[q + s * 2 * p] = (a[q + s * p] + a[q + s * (p + m)]);
            tmp[q + s * (2 * p + 1)] = (a[q + s * p] - a[q + s * (p + m)]) * zeta;
          }
          zeta = zeta * e;
        }
        swap(a, tmp);
      }

      if (inv) {
        for (std::int64_t i = 0; i < n; i++) {
          a[i] = a[i] / value_type(n);
        }
      }
    }

    template<typename T>
    static std::vector <value_type> prepare(const std::vector <T> &v) {
      std::vector <value_type> r(v.size());
      for (std::int64_t i = 0; i < v.size(); i++) {
        r[i] = v[i];
      }
      return r;
    }

    template<typename T>
    static std::vector <value_type> conv(const std::vector <T> &a, const std::vector <T> &b) {
      size_type deg = a.size() + b.size() - 1;
      size_type n = 1;

      while (n < deg) n <<= 1;
      std::vector <value_type> _a = prepare(a), _b = prepare(b);
      _a.resize(n, 0);
      _b.resize(n, 0);
      std::vector <value_type> c(n);

      ntt(_a);
      ntt(_b);
      for (std::int64_t i = 0; i < n; i++) {
        c[i] = _a[i] * _b[i];
      }
      ntt(c, 1);
      return c;
    }
  };

  template<class value_type = ModInt<998244353>>
  struct Polynomial {
    using ntt = NTT<998244353, 3>;

    std::vector<value_type> poly;

    Polynomial () {}
    Polynomial (int n) : poly(n, 0) {}
    Polynomial (const std::vector<value_type>& v) : poly(v) {}

    const int size() const { return poly.size(); }
    value_type& operator[] (int k) {
      return poly[k];
    }

    Polynomial pre(int m) {
      Polynomial ret(m);
      for (int i = 0; i < std::min(m, size()); i++) {
        ret[i] = poly[i];
      }
      return ret;
    }

    Polynomial& operator+= (const Polynomial& f) {
      for (int i = 0; i < std::min(size(), f.size()); i++) {
        poly[i] += f.poly[i];
      }
      return *this;
    }

    Polynomial& operator-= (const Polynomial& f) {
      for (int i = 0; i < std::min(size(), f.size()); i++) {
        poly[i] -= f.poly[i];
      }
      return *this;
    }

    Polynomial& operator*= (const Polynomial& f) {
      *this = multiply(*this, f);
      return *this;
    }

    Polynomial& operator*= (const value_type& x) {
      for (int i = 0; i < size(); i++) {
        poly[i] *= x;
      }
      return *this;
    }

    static Polynomial multiply(const Polynomial& f, const Polynomial& g) {
      return Polynomial(ntt::conv(f.poly, g.poly));
    }

    Polynomial& operator/= (const value_type& x) {
      return (*this) *= (value_type(1) / x);
    }

    Polynomial operator+(const Polynomial& f) const { return Polynomial(*this)+=f; }
    Polynomial operator-(const Polynomial& f) const { return Polynomial(*this)-=f; }
    Polynomial operator*(const Polynomial& f) const { return Polynomial(*this)*=f; }
    Polynomial operator*(const value_type& x) const { return Polynomial(*this)*=x; }
    Polynomial operator/(const value_type& x) const { return Polynomial(*this)/=x; }
    Polynomial operator-() { return Polynomial() - poly; }

    Polynomial inv() {
      Polynomial ret(size());
      ret[0] = poly[0].inv();
      for (int i = 1; i < size(); i <<= 1) {
        std::vector<value_type> n_poly(i*2), n_ret(i*2);
        for (int j = 0; j < i; j++) n_ret[j] = ret[j];
        for (int j = 0; j < std::min(i*2, size()); j++) n_poly[j] = poly[j];
        ntt::ntt(n_ret);
        ntt::ntt(n_poly);
        for (int j = 0; j < i*2; j++) {
          n_poly[j] = n_ret[j] * n_poly[j];
        }
        ntt::ntt(n_poly, 1);
        for (int j = 0; j < i; j++) {
          n_poly[j] = 0;
        }
        ntt::ntt(n_poly);
        for (int j = 0; j < i*2; j++) {
          n_poly[j] = n_ret[j] * n_poly[j];
        }
        ntt::ntt(n_poly, 1);
        for (int j = i; j < std::min(i*2, size()); j++) {
          ret[j] = ret[j] - n_poly[j];
        }
      }
      return ret;
    }

    Polynomial sqrt() {
      Polynomial ret(size());
      ret[0] =
    }
  };
};

