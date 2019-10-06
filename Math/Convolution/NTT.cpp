#include <bits/stdc++.h>

namespace ntt {
    std::pair<std::int64_t, std::int64_t> extgcd(std::int64_t a, std::int64_t b) {
        std::pair<std::int64_t, std::int64_t> pa(1, 0), pb(0, 1);
        while (b != 0) {
            ::std::swap(a, b); ::std::swap(pa, pb);
            pb = ::std::make_pair(pb.first - pa.first * (b / a), pb.second - pa.second * (b / a));
            b = b % a;
        }
        return pa;
    }

    std::uint64_t modinv(const std::uint64_t& a, const std::int64_t& m) {
        std::int64_t res = extgcd(static_cast<std::int64_t>(a), static_cast<std::int64_t>(m)).first % m;
        if (res < 0) res += m;
        return res;
    }

    std::uint64_t modpow(std::uint64_t a, std::uint64_t b, std::uint64_t m) {
        std::uint64_t res = 1;
        while (b) {
            res = res * (b & 1 ? a : 1) % m;
            a = a * a % m;
            b >>= 1;
        }
        return res;
    }

    template<std::uint64_t MOD, std::uint64_t PRIM_ROOT>
    class NTT {
    public:
        using value_type = std::uint64_t;
        using size_type = std::size_t;

    private:

        static void ntt(std::vector<value_type> &a, bool inv = false) {
            std::int64_t n = a.size();
            std::vector<value_type> tmp(n);
            std::int64_t mask = n - 1;
            std::int64_t h_bit = n >> 1; // highest-bit
            for (std::int64_t k = 1, l = mask; l > 0; k++, l >>= 1) {
                value_type e = modpow(PRIM_ROOT, (MOD - 1) / (1 << k), MOD);
                if (inv) e = modinv(e, MOD);
                value_type zeta = 1;

                for (std::int64_t i = 0; i <= mask; i++) {
                    std::int64_t idx = ((i >> k) << (k - 1)) | (i & ((1 << (k - 1)) - 1));
                    tmp[i] = (a[idx] + zeta * a[h_bit | idx] % MOD) % MOD;
                    zeta = zeta * e % MOD;
                }
                swap(a, tmp);
            }
            if (inv) {
                std::uint64_t inv_n = modinv(n, MOD);
                for (std::int64_t i = 0; i < n; i++) {
                    a[i] = a[i] * inv_n % MOD;
                }
            }
        }

    public:
        template<typename T>
        static std::vector<value_type> prepare(const std::vector<T>& v) {
            std::vector<value_type> r(v.size());
            for (std::int64_t i = 0; i < v.size(); i++) {
                r[i] = v[i] % MOD;
                if (r[i] < 0) r[i] += MOD;
            }
            return r;
        }

        template<typename T>
        static std::vector<value_type> conv(const std::vector<T>& a, const std::vector<T>& b) {
            size_type deg = a.size() + b.size() - 1;
            size_type n = 1;

            while (n < deg) n <<= 1;
            std::vector<value_type> _a = prepare(a), _b = prepare(b);
            _a.resize(n, 0);
            _b.resize(n, 0);
            std::vector<value_type> c(n);

            ntt(_a);
            ntt(_b);
            for (std::int64_t i = 0; i < n; i++) {
                c[i] = _a[i] * _b[i] % MOD;
            }
            ntt(c, 1);
            return c;
        }
    };

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

    template<typename T>
    std::vector<std::int64_t> garner_ntt(const std::vector<T>& a, const std::vector<T>& b, const std::int64_t m, std::int64_t use = 3) {
        std::int64_t deg = a.size() + b.size() - 1;
        if (use <= 0) use = 1;
        if (use > 4) use = 4;
        static constexpr std::int64_t NTT_PRIMES[][2] = {
                {1224736769, 3}, // 2^24 * 73 + 1,
                {998244353, 3},  // 2^23 * 7 * 17 + 1
                {469762049, 3},  // 2^26 * 7 + 1
                {167772161, 3},  // 2^25 * 5 + 1
        }; // Thanks to luma

        using ntt1 = NTT<NTT_PRIMES[0][0], NTT_PRIMES[0][1]>;
        using ntt2 = NTT<NTT_PRIMES[1][0], NTT_PRIMES[1][1]>;
        using ntt3 = NTT<NTT_PRIMES[2][0], NTT_PRIMES[2][1]>;
        using ntt4 = NTT<NTT_PRIMES[3][0], NTT_PRIMES[3][1]>;
        std::vector<std::vector<std::int64_t>> _ga(deg, std::vector<std::int64_t>(use));
        std::vector<std::int64_t> _gb(use);
        for (std::int64_t i = 0; i < use; i++) {
            std::vector<std::uint64_t> v;
            switch (i) {
                case 0: v = ntt1::conv(a, b); break;
                case 1: v = ntt2::conv(a, b); break;
                case 2: v = ntt3::conv(a, b); break;
                case 3: v = ntt4::conv(a, b); break;
            }
            for (std::int64_t j = 0; j < deg; j++) _ga[j][i] = v[j];
            _gb[i] = NTT_PRIMES[i][0];
        }
        std::vector<std::int64_t> res(deg);
        for (std::int64_t i = 0; i < deg; i++) {
            res[i] = garner(_ga[i], _gb, m);
        }
        return res;
    }
}
