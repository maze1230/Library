#include <bits/stdc++.h>

class FFT {
public:
    using Real = double;
    using Complex = std::complex<double>;
    using size_type = std::size_t;

private:
    static Complex mul(const Complex& a, const Complex& b) {
        return Complex(real(a)*real(b)-imag(a)*imag(b), real(a)*imag(b)+real(b)*imag(a));
    }

    static void fft(std::vector<Complex> &a, bool inv = false) {
        std::int64_t n = a.size();
        std::vector<Complex> tmp(n);
        std::int64_t mask = n - 1;
        std::int64_t h_bit = n >> 1; // highest-bit
        for (std::int64_t k = 1, l = mask; l > 0; k++, l >>= 1) {
            double deg = 2 * M_PI / (1 << k) * (inv ? -1 : 1);
            Complex e = Complex(cosl(deg), sinl(deg));
            Complex zeta =1;
            for (std::int64_t i = 0; i <= mask; i++) {
                std::int64_t idx = ((i >> k) << (k - 1)) | (i & ((1 << (k - 1)) - 1));
                tmp[i] = a[idx] + mul(zeta, a[h_bit | idx]);
                zeta = mul(zeta, e);
            }
            std::swap(a, tmp);
        }

        if (inv) {
            for (std::int64_t i = 0; i < n; i++) {
                a[i] /= n;
            }
        }
    }

public:
    static std::vector<std::int64_t> conv(const std::vector<std::int64_t>& ar, const std::vector<std::int64_t>& br) {
        size_type deg = ar.size() + br.size() - 1;
        size_type n = 1;

        while (n < deg) n <<= 1;
        std::vector<Complex> a(n, 0);
        for (std::int64_t i = 0; i < ar.size(); i++) a[i].real(ar[i]);
        for (std::int64_t i = 0; i < br.size(); i++) a[i].imag(br[i]);

        std::vector<Complex> c(n);

        fft(a);
        for (std::int64_t i = 0; i < n; i++) {
            c[i] = mul(mul(a[i] + conj(a[(n - i)%n]), a[i] - conj(a[(n - i)%n])), Complex(0, -0.25));
        }
        fft(c, 1);
        std::vector<std::int64_t> cr(n);
        for (std::int64_t i = 0; i < deg; i++) {
            cr[i] = std::round(c[i].real());
        }
        return cr;
    }
};



int main(void) {
  std::ios::sync_with_stdio(false), std::cin.tie(0);
  using namespace std;
  int N;
  cin >> N;
  vector<int64_t> a(N+1), b(N+1);
  for (int i = 0; i < N; i++) {
    cin >> a[i+1] >> b[i+1];
  }
  auto c = FFT::conv(a, b);
  for (int i = 1; i < 2*N+1; i++) {
    cout << c[i] << "\n";
  }
}
