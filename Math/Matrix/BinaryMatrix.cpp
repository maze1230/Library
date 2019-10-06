#include <bits/std++.h>

template<::std::size_t Column>
class BinaryMatrix {
public:
	using size_type = ::std::size_t;
	using value_type = ::std::uint64_t;
	using Row = ::std::bitset<Column>;
	using Mat = ::std::vector<Row>;

private:
	size_type R, C;
	Mat A;

	void add_row_to_another(size_type r1, size_type r2){ // Row(r1) += Row(r2)
		A[r1] = A[r1] ^ A[r2];
	}


public:
	BinaryMatrix() {}
	BinaryMatrix(size_type R_, size_type C_ = Column) : R(R_), C(C_), A(Mat(R_)) {}
	BinaryMatrix(const Mat& A_) : R(A_.size()), C(A_[0].size()), A(A_) {}
	BinaryMatrix(const Mat&& A_) : R(A_.size()), C(A_[0].size()), A(A_) {}
	BinaryMatrix(const BinaryMatrix& m) : R(m.R), C(m.C), A(m.A) {}
	BinaryMatrix(const BinaryMatrix&& m) : R(m.R), C(m.C), A(m.A) {}

	BinaryMatrix &operator=(const BinaryMatrix &m){
 		R = m.R; C = m.C; A = m.A;
		return *this;
	}
	BinaryMatrix &operator=(const BinaryMatrix &&m){
 		R = m.R; C = m.C; A = m.A;
		return *this;
	}
	static BinaryMatrix I(const size_type N){
		BinaryMatrix m(N, N);
		for(size_type i = 0;i < N;i++) m[i][i] = 1;
		return m;
	}

	const Row& operator[](size_type k) const& { return A.at(k); }
	Row& operator[](size_type k) & { return A.at(k); }
	Row operator[](size_type k) const&& { return ::std::move(A.at(k)); }

	size_type row() const { return R; } // the number of rows
	size_type column() const { return C; }

	BinaryMatrix& operator+=(const BinaryMatrix &B){
		assert(column() == B.column() && row() == B.row());
		for(size_type i = 0;i < R;i++)
			(*this)[i] ^= B[i];
		return *this;
	}

	BinaryMatrix& operator-=(const BinaryMatrix &B){
		assert(column() == B.column() && row() == B.row());
		for(size_type i = 0;i < R;i++)
			(*this)[i] ^= B[i];
		return *this;
	}

	BinaryMatrix& operator*=(const BinaryMatrix &B){
		assert(column() == B.row());
		BinaryMatrix M(R, B.column());
		for(size_type i = 0;i < R;i++) {
			for(size_type j = 0;j < B.column();j++) {
				M[i][j] = 0;
				for(size_type k = 0;k < C;k++) {
					M[i][j] ^= ((*this)[i][k] & B[k][j]);
				}
			}
		}
		swap(M, *this);
		return *this;
	}

	void gaussian_elimination() {
		size_type last_row = 0;
		for (int i = C-1; i >= 0 && last_row < R; i--) {
			for (size_type j = last_row; j < R; j++) {
				if (A[j][i]) {
					swap(A[j], A[last_row]);
					break;
				}
			}

			for (size_type j = 0; j < R; j++) {
				if (last_row == j) continue;
				if (A[last_row][i] & A[j][i]) {
					add_row_to_another(j, last_row);
				}
			}
			if (A[last_row][i]) last_row++;
		}
	}

	size_type rank() {
		Mat tmp = A;
		gaussian_elimination();
		swap(tmp, A);
		for (size_type i = 0; i < R; i++) {
			size_type cnt = 0;
			for (size_type j = 0; j < C; j++) {
				if (tmp[i][j]) cnt++;
			}
			if (cnt == 0) return i;
		}
		return R;
	}

	// if you use this method, you must call gaussian_elimination before.
	bool can_construct_x(const Row& x) {
		Row now;
		for (size_type i = 0; i < R; i++) {
			for (size_type j = 0; j < C; j++) {
				if (x[j] == now[j] && A[i][j]) break;
				if (x[j] != now[j] && A[i][j]) {
					now ^= A[i];
					break;
				}
			}
		}
		return now == x;
	}
};
