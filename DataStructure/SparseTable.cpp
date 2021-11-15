#include <bits/stdc++.h>

template<class ValueMonoid, int HEIGHT = 20> // HEIGHT is log size
class SparseTable {
private:
	using value_structure = ValueMonoid;
	using value_type = typename value_structure::value_type;
  using size_type = std::uint32_t;

  size_type size_;
  std::array<std::vector<value_type>, HEIGHT> table;
public:
	SparseTable() : size_(0) {}
  SparseTable(const std::vector<value_type>& v) : size_(v.size()) {
    table[0] = v;
    for (size_type i = 1, w = 1; i < HEIGHT; i++, w *= 2) {
      table[i].resize(size_, value_structure::identity());
      for (size_type j = 0; j < size_; j++) {
        if (j + w < size_) table[i][j] = value_structure::operation(table[i-1][j], table[i-1][j+w]);
        else table[i][j] = table[i-1][j];
      }
    }
  }

  static inline size_type log2(size_type x) {
    if (x == 0) return 0;
    return size_type(31) ^ __builtin_clz(x);
  }

  value_type query(size_type l, size_type r) {
    if (r <= l) return value_structure::identity();
    size_type k = log2(r - l);
    return value_structure::operation(table[k][l], table[k][r-(size_type(1) << k)]);
  }
};

/*
template<ValueMonoid>
class SparseTable

ValueMonoid - Monoidって付いてるけどMonoidではない(セグ木等と合わせるため
	-	要求
		- value_type
		- identity() -> value_type : 単位元を返す
		- operation(value_type, value_type) -> value_type : 演算結果を返す

SparseTable
	- 提供
    - constuctor(vector v)
      - v を元にtableを構築する

		- query(size_type l, size_type r) -> value_type
			- 計算量 O(1)
			- [l, r)までの計算結果
*/