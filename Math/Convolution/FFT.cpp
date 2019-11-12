#include <bits/stdc++.h>

namespace FFT {
  using Real = double;
  using Complex = std::complex<double>;
  using size_type = std::size_t;

  static Complex mul(const Complex& a, const Complex& b) {
    return Complex(real(a)*real(b)-imag(a)*imag(b), real(a)*imag(b)+real(b)*imag(a));
  }

  static constexpr std::int64_t N = 1 << 18;
  static std::array<Complex, N> zeta;

  static void initialize_zeta() {
    for (std::int64_t i = 0; i < N/4; i++) {
      Real x = cosl(2 * M_PI * i / N), y = sinl(2 * M_PI * i / N);
      zeta[i] = Complex(x, y);
      zeta[i + N/4] = Complex(-y, x);
      zeta[i + N/2] = Complex(x, -y);
      zeta[i + N/4 + N/2] = Complex(-y, -x);
    }
  }

  static void fft(std::vector<Complex>& a, bool inv = 0) {
    const std::int64_t n = a.size();
    std::vector<Complex> tmp(n);
    for (std::int64_t s = 1, m = n >> 1; m > 0; s <<= 1, m >>= 1) {
      for (std::int64_t p = 0; p < m; p++) {
        for (std::int64_t q = 0; q < s; q++) {
          tmp[q + s * 2*p] = a[q + s * p] + a[q + s * (p + m)];
          tmp[q + s * (2*p + 1)] = (a[q + s * p] - a[q + s * (p + m)]) * zeta[p * N / m / 2 + (inv ? N/2 : 0)];
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

  static std::vector<std::int64_t> conv(const std::vector<std::int64_t>& ar, const std::vector<std::int64_t>& br) {
    if (zeta[0] == Complex(0, 0)) initialize_zeta();
    size_type deg = ar.size() + br.size() - 1;
    size_type n = 1;

    while (n < deg) n <<= 1;
    std::vector<Complex> a(n, 0);
    for (std::int64_t i = 0; i < ar.size(); i++) a[i].real(ar[i]);
    for (std::int64_t i = 0; i < br.size(); i++) a[i].imag(br[i]);

    std::vector<Complex> c(n);

    fft(a);
    for (std::int64_t i = 0; i < n; i++) {
      size_type j = i == 0 ? 0 : n - i;
      c[i] = mul(mul(a[i] + conj(a[j]), a[i] - conj(a[j])), Complex(0, -0.25));
    }
    fft(c, 1);
    std::vector<std::int64_t> cr(n);
    for (std::int64_t i = 0; i < deg; i++) {
      cr[i] = static_cast<std::int64_t>(c[i].real() + 0.5);
    }
    return cr;
  }
};



int main(void) {
  using namespace std;
  int N;
  cin >> N;
  vector<int64_t> a(N+1, 0), b(N+1, 0);
  for (int i = 0; i < N; i++) {
    int x, y;
    scanf("%d %d", &x, &y);
    a[i+1] = x; b[i+1] = y;
  }
  auto c = FFT::conv(a, b);
  for (int i = 1; i < 2*N+1; i++) {
    cout << c[i] << "\n";
  }
}
