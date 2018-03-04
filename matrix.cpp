#include <bits/stdc++.h>
using namespace std;

#define INF_LL (int64)1e18
#define INF (int32)1e9
#define REP(i, n) for(int i = 0;i < (n);i++)
#define FOR(i, a, b) for(int i = (a);i < (b);i++)
#define all(x) x.begin(),x.end()
#define fs first
#define sc second
using int32 = int_fast32_t;
using uint32 = uint_fast32_t;
using int64 = int_fast64_t;
using uint64 = uint_fast64_t;
using PII = pair<int32, int32>;
using PLL = pair<int64, int64>;

const double eps = 1e-10;

template<typename A, typename B>inline void chmin(A &a, B b){if(a > b) a = b;}
template<typename A, typename B>inline void chmax(A &a, B b){if(a < b) a = b;}

template<typename T>
class Matrix{
private:
	using mat = vector<vector<T>>;
	const int64 mod = 1e9+7;
	mat m;
public:
	uint32 col=-1, row=-1;
	Matrix(){}
	Matrix(mat&& m):m(m){ row = m.size(); col = m[0].size(); }
	Matrix(Matrix<T>&& rhs){ m = rhs.m; row = rhs.row; col = rhs.col;}
	Matrix(int32 row, int32 col, bool isIdentity=false):row(row),col(col){
		m = mat(row, vector<T>(col, 0));
		if(isIdentity && row == col){
			for(int32 i = 0;i < row;i++)	m[i][i] = 1;
		}
	}

	Matrix<T> pow(int64 x){
		if(x == 0) return Matrix(row, col, 1);
		if(x&1) return (*this)*pow(x-1);
		Matrix<T> ret = pow(x/2);
		return ret*ret;
	}

	Matrix<T> operator+(const Matrix<T>& rhs){
		Matrix<T> ret = *this;
		if(row != rhs.row || col != rhs.col){
			cerr << "Error happened in operator+:the number of lhs's col & row is not the number of rhs's" << endl;
			exit(-1);
		}
		for(int32 i = 0;i < row;i++){
			for(int32 j = 0;j < col;j++){
				ret[i][j] = m[i][j]+rhs[i][j];
				//if(is_integral<T>::value) ret[i][j] %= mod;
			}
		}
		return ret;
	}

	Matrix<T> operator*(T x){
		Matrix<T> ret = m;
		for(int32 i = 0;i < row;i++){
			for(int32 j = 0;j < col;j++){
				ret[i][j] = m[i][j]*x;
				//if(is_integral<T>::value) ret[i][j] %= mod;
			}
		}
		return ret;
	}

	Matrix<T> operator*(const Matrix<T>& rhs){
		Matrix<T> ret = mat(row, vector<T>(rhs.col, 0));
		if(col != rhs.row){
			cerr << "Error happened in operator*:the number of lhs's col is not the number of rhs's" << endl;
			exit(-1);
		}

		for(int32 i = 0;i < row;i++){
			for(int32 j = 0;j < rhs.col;j++){
				for(int32 k = 0;k < col;k++){
					ret[i][j] = ret[i][j]+m[i][k]*rhs[k][j];
				}
				//if(is_integral<T>::value) ret[i][j] %= mod;
			}
		}
		return ret;
	}

	Matrix<T>& operator=(mat&& rhs){ m = rhs; row = rhs.size(); col = rhs[0].size(); return (*this);}
	Matrix<T>& operator=(Matrix&& rhs){ m = rhs.m; row = rhs.row; col = rhs.col; return (*this);}
	const vector<T>& operator[](int32 x) const{ return m[x];}
	vector<T>& operator[](int32 x){return m[x];}

};

template<typename T>
istream& operator>>(istream& is, Matrix<T>& m){
	if(m.row == -1 || m.col == -1){
		cerr << "The matrix is not available." << endl;
		return is;
	}
	for(int32 i = 0;i < m.row;i++) for(int32 j = 0;j < m.col;j++) is >> m[i][j];
	return is;
}

template<typename T>
ostream& operator<<(ostream&os, const Matrix<T>& m){
	if(m.row == -1 || m.col == -1){
		cerr << "The matrix is not available." << endl;
		return os;
	}
	for(int32 i = 0;i < m.row;i++){
		for(int32 j = 0;j < m.col;j++) os << m[i][j] << " ";
		os << endl;
	}
	return os;
}

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}