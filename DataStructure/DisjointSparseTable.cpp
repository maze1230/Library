#include <bits/stdc++.h>

template<class Semigroup, int HEIGHT = 20> // HEIGHT = log2 size
class DisjointSparseTable {
private:
  using value_structure = Semigroup;
  using value_type = typename value_structure::value_type;
  using size_type = std::uint32_t;

  std::array<std::vector<value_type>, HEIGHT> table;
  size_type size_;
public:
  DisjointSparseTable() {}
  DisjointSparseTable(const std::vector<value_type>& v) : size_(v.size()) {
    table[0] = v;
    for (size_type i = 1, w = 2; i < HEIGHT; i++, w *= 2) {
      table[i].resize(size_);
      for (size_type j = w; j < size_; j += w*2) {
        for (size_type k = 0; k < w; k++) {
          if(k == 0) table[i][j-k-1] = v[j-k-1];
          else table[i][j-k-1] = value_structure::operation(v[j-k-1], table[i][j-k]);
        }
        for (size_type k = 0; k < w && k+j < size_; k++) {
          if(k == 0) table[i][j+k] = v[j+k];
          else table[i][j+k] = value_structure::operation(table[i][j+k-1], v[j+k]);
        }
      }
    }
  }

  static inline size_type log2(size_type x) {
    if (x == 0) return 0;
    return size_type(31) ^ __builtin_clz(x);
  }

  value_type query(size_type l, size_type r) {
    assert(0 <= l && l < size_ && l < r && r <= size_);
    r--;
    if (l == r) return table[0][l];
    size_type k = log2(r^l);
    return value_structure::operation(table[k][l], table[k][r]);
  }
};

/*
template<Semigroup>
class DisjointSparseTable

- Semigroup 
	-	要求
		- value_type
		- operation(value_type, value_type) -> value_type : 演算結果を返す

SparseTable
	- 提供
    - constuctor(vector v)
      - v を元にtableを構築する

		- query(size_type l, size_type r) -> value_type
			- 計算量 O(1)
			- [l, r)までの計算結果
*/