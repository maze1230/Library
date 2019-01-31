#include <bits/stdc++.h>

template<typename T>
class Matrix{
private:
	using size_type = ::std::size_t;
	using Row = ::std::vector<T>;
	using Mat = ::std::vector<Row>;

	size_type R, C; // row, column
	Mat A;

	void add_row_to_another(size_type r1, size_type r2, const T k){ // Row(r1) += Row(r2)*k
		for(size_type i = 0;i < C;i++)
			A[r1][i] += A[r2][i]*k;
	}

	void scalar_multiply(size_type r, const T k){
		for(size_type i = 0;i < C;i++)
			A[r][i] *= k;
	}

	void scalar_division(size_type r, const T k){
		for(size_type i = 0;i < C;i++)
			A[r][i] /= k;
	}

public:
	Matrix(){}
	Matrix(size_type r, size_type c) : R(r), C(c), A(r, Row(c)) {}
	Matrix(const Mat &m) : R(m.size()), C(m[0].size()), A(m) {}
	Matrix(const Mat &&m) : R(m.size()), C(m[0].size()), A(m) {}
	Matrix(const Matrix<T> &m) : R(m.R), C(m.C), A(m.A) {}
	Matrix(const Matrix<T> &&m) : R(m.R), C(m.C), A(m.A) {}
	Matrix<T> &operator=(const Matrix<T> &m){
 		R = m.R; C = m.C; A = m.A;
		return *this;
	}
	Matrix<T> &operator=(const Matrix<T> &&m){
 		R = m.R; C = m.C; A = m.A;
		return *this;
	}
	static Matrix I(const size_type N){
		Matrix m(N, N);
		for(size_type i = 0;i < N;i++) m[i][i] = 1;
		return m;
	}

	const Row& operator[](size_type k) const& { return A.at(k); }
	Row& operator[](size_type k) & { return A.at(k); }
	Row operator[](size_type k) const&& { return ::std::move(A.at(k)); }

	size_type row() const { return R; } // the number of rows
	size_type column() const { return C; }

	T determinant(){
		assert(R == C);
		Mat tmp = A;
		T res = 1;
		for(size_type i = 0;i < R;i++){
			for(size_type j = i;j < R;j++){ // satisfy A[i][i] > 0
				if (A[j][i] != 0) {
					if (i != j) res *= -1;
					swap(A[j], A[i]);
					break;
				}
			}
			if (A[i][i] == 0) return 0;
			res *= A[i][i];
			scalar_division(i, A[i][i]);
			for(size_type j = i+1;j < R;j++){
				add_row_to_another(j, i, -A[j][i]);
			}
		}
		swap(tmp, A);
		return res;
	}

	Matrix inverse(){
		assert(R == C);
		assert(determinant() != 0);
		Matrix inv(Matrix::I(R)), tmp(*this);
		for(size_type i = 0;i < R;i++){
			for(size_type j = i;j < R;j++){
				if (A[j][i] != 0) {
					swap(A[j], A[i]);
					swap(inv[j], inv[i]);
					break;
				}
			}
			inv.scalar_division(i, A[i][i]);
			scalar_division(i, A[i][i]);
			for(size_type j = 0;j < R;j++){
				if(i == j) continue;
				inv.add_row_to_another(j, i, -A[j][i]);
				add_row_to_another(j, i, -A[j][i]);
			}
		}
		(*this) = tmp;
		return inv;
	}

	Matrix& operator+=(const Matrix &B){
		assert(column() == B.column() && row() == B.row());
		for(size_type i = 0;i < R;i++)
			for(size_type j = 0;j < C;j++)
				(*this)[i][j] += B[i][j];
		return *this;
	}

	Matrix& operator-=(const Matrix &B){
		assert(column() == B.column() && row() == B.row());
		for(size_type i = 0;i < R;i++)
			for(size_type j = 0;j < C;j++)
				(*this)[i][j] -= B[i][j];
		return *this;
	}

	Matrix& operator*=(const Matrix &B){
		assert(R == B.row());
		Mat M(R, Row(C, 0));
		for(size_type i = 0;i < R;i++)
			for(size_type j = 0;j < B.column();j++)
				for(size_type k = 0;k < C;k++)
					M[i][j] += (*this)[i][k] * B[k][j];
		A.swap(M);
		return *this;
	}

	Matrix& operator/=(const Matrix &B){
		assert(C == B.row());
		Matrix M(B);
		(*this) *= M.inverse();
		return *this;
	}

	Matrix operator+(const Matrix &B) const { return (Matrix(*this) += B); }
	Matrix operator-(const Matrix &B) const { return (Matrix(*this) -= B); }
	Matrix operator*(const Matrix &B) const { return (Matrix(*this) *= B); }
	Matrix operator/(const Matrix &B) const { return (Matrix(*this) /= B); }

	Matrix pow(size_type k){
		assert(R == C);
		Matrix M(Matrix::I(R));
		while(k){
			if (k & 1) M *= (*this);
			k >>= 1;
			(*this) *= (*this);
		}
		A.swap(M.A);
		return *this;
	}

	friend ::std::ostream &operator<<(::std::ostream &os, Matrix &p){
		for(size_type i = 0;i < p.row();i++){
			for(size_type j = 0;j < p.column();j++){
				os << p[i][j] << " ";
			}
			os << ::std::endl;
		}
		return os;
	}
};

int main(void){
}
