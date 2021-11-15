#include <bits/stdc++.h>

namespace ArbitraryModFFT {
  using Real = double;
  using Complex = std::complex<Real>;
  using size_type = std::size_t;

  static Complex mul(const Complex& a, const Complex& b) {
    return Complex(real(a)*real(b)-imag(a)*imag(b), real(a)*imag(b)+real(b)*imag(a));
  }

  static constexpr std::int64_t N = 1 << 20;
  static std::array<Complex, N*2> zeta;

  static void initialize_zeta() {
    for (std::int64_t i = 0; i < N/4; i++) {
      Real x = cosl(2 * M_PI * i / N), y = sinl(2 * M_PI * i / N);
      zeta[i] = Complex(x, y);
      zeta[i + N/4] = Complex(-y, x);
      zeta[i + N/2] = Complex(-x, -y);
      zeta[i + N/4*3] = Complex(y, -x);
      zeta[i + N] = Complex(x, -y);
      zeta[i + N/4 + N] = Complex(-y, -x);
      zeta[i + N/2 + N] = Complex(-x, y);
      zeta[i + N/4*3 + N] = Complex(y, x);
    }
  }

  static void fft(std::vector<Complex>& a, bool inv = 0) {
    const std::int64_t n = a.size();
    std::vector<Complex> tmp(n);
    for (std::int64_t s = 1, m = n >> 1; m > 0; s <<= 1, m >>= 1) {
      for (std::int64_t p = 0; p < m; p++) {
        for (std::int64_t q = 0; q < s; q++) {
          tmp[q + s * 2*p] = a[q + s * p] + a[q + s * (p + m)];
          tmp[q + s * (2*p + 1)] = mul((a[q + s * p] - a[q + s * (p + m)]), zeta[p * N / m / 2 + (inv ? N : 0)]);
        }
      }
      swap(a, tmp);
    }

    if (inv) {
      for (std::int64_t i = 0; i < n; i++) {
        a[i] /= n;
      }
    }
  }

  static std::vector<std::int64_t> conv(const std::vector<std::int64_t>& ar, const std::vector<std::int64_t>& br, const std::int64_t mod = 998244353) {
    if (zeta[0] == Complex(0, 0)) initialize_zeta();

    size_type deg = ar.size() + br.size() - 1;
    size_type n = 1;

    while (n < deg) n <<= 1;
    std::vector<Complex> a(n, 0), b(n, 0), c(n, 0);
    std::int64_t mask = (1 << 10) - 1;
    for (std::int64_t i = 0; i < ar.size(); i++) {
      a[i] = Complex(ar[i] & mask, ar[i] >> 10 & mask);
      c[i] = Complex(ar[i] >> 20 & mask, 0);
    }
    for (std::int64_t i = 0; i < br.size(); i++) {
      b[i] = Complex(br[i] & mask, br[i] >> 10 & mask);
      c[i] = Complex(c[i].real(), br[i] >> 20 & mask);
    }

    std::vector<Complex> Fa(n), Fb(n), Fc(n);

    fft(a); fft(b); fft(c);
    for (std::int64_t i = 0; i < n; i++) {
      size_type j = i == 0 ? 0 : n - i;
      Complex aL = mul(a[i] + conj(a[j]), Complex(0.5, 0)), aM = mul(a[i] - conj(a[j]), Complex(0, -0.5)), aH = mul(c[i] + conj(c[j]), Complex(0.5, 0));
      Complex bL = mul(b[i] + conj(b[j]), Complex(0.5, 0)), bM = mul(b[i] - conj(b[j]), Complex(0, -0.5)), bH = mul(c[i] - conj(c[j]), Complex(0, -0.5));
      Fa[i] = mul(aL, bL) + mul(mul(aL, bM) + mul(aM, bL), Complex(0, 1));
      Fb[i] = mul(aM, bM) + mul(aL, bH) + mul(aH, bL) + mul(mul(aM, bH) + mul(aH, bM), Complex(0, 1));
      Fc[i] = mul(aH, bH);
    }
    fft(Fa, 1); fft(Fb, 1); fft(Fc, 1);
    std::vector<std::int64_t> cr(n);
    for (std::int64_t i = 0; i < deg; i++) {
      cr[i] = (1LL << 40) % mod * (static_cast<std::int64_t>(Fc[i].real() + 0.5) % mod) % mod;
      cr[i] = (cr[i] + (1LL << 30) % mod * static_cast<std::int64_t>(Fb[i].imag() + 0.5) % mod) % mod;
      cr[i] = (cr[i] + (1LL << 20) * static_cast<std::int64_t>(Fb[i].real() + 0.5) % mod) % mod;
      cr[i] = (cr[i] + (1LL << 10) * static_cast<std::int64_t>(Fa[i].imag() + 0.5) % mod) % mod;
      cr[i] = (cr[i] + static_cast<std::int64_t>(Fa[i].real() + 0.5)) % mod;
    }
    return cr;
  }
};

int main(void) {
  using namespace std;
  int N, M;
  scanf("%d %d", &N, &M);
  vector<int64_t> A(N), B(M);
  int x;
  for (int i = 0; i < N; i++) {
    scanf("%d", &x); A[i] = x;
  }
  for (int i = 0; i < M; i++) {
    scanf("%d", &x); B[i] = x;
  }
  auto c = ArbitraryModFFT::conv(A, B);
  for (int i = 0; i < N+M-1; i++) {
    printf("%lld\n", (long long)c[i]);
  }
}
