#include <bits/stdc++.h>

template<::std::size_t maximum, ::std::size_t max_digit,
	class RandomAccessIterator, class F>
void LSD_RadixSort(RandomAccessIterator first, RandomAccessIterator last,
		const F &get_digit){
	using value_type = typename ::std::iterator_traits<RandomAccessIterator>::value_type;
	using size_type = ::std::size_t;

	const ::std::vector<value_type> cpy(first, last);
	const size_type size = cpy.size();
	::std::array<size_type, maximum+2> count;
	::std::vector<size_type> res(size), tmp(size);

	::std::iota(res.begin(), res.end(), 0);

	for(size_type digit = 0;digit < max_digit;++digit){
		::std::fill(count.begin(), count.end(), 0);
		for(const auto i : res){
			++count[get_digit(cpy[i], digit) + 1];
		}

		for(size_type i = 1;i < maximum;++i){
			count[i+1] += count[i];
		}

		for(const auto i : res){
			tmp[count[get_digit(cpy[i], digit)]++] = i;
		}
		::std::swap(res, tmp);
	}
for(size_type i = 0;i < size;++i){
		*(first + i) = cpy[res[i]];
	}
}

/*

	LSD_RadixSort(first, last, maximum, max_digit, get_digit)
	- 基数ソートを行う
	- 計算量はO(N * max_digit + maximum * max_digit)
	- よく考えたらランダムアクセスじゃなくてもいいけど一回vにコピーしてるしいっかなって

	Iterater first, last
	- [first, last)をソートする
	
	int maximum
	- get_digitが返しうる最大値

	int max_digit
	- max of len(x) for x in [first, last)

	function<int(T x, int digit)> get_digit
	- T: RandomAccessIterator::value_type
	- xのdigit桁目をint型で返すような関数
	- 
	- example
		- int -> return x >> digit & 1;
		- string -> return max_digit-digit-1 < x.size() ? x[max_digit-digit-1] : 0;
*/

/*

verify:https://beta.atcoder.jp/contests/abc088/submissions/3178594

int get(int x, int dig){
	for(int i = 0;i < dig;i++)
		x /= 10;
	return x % 10;
}

int main(void){
	int n;
	::std::vector<int> v;
	std::cin >> n;
	v.resize(n);
	for(int i = 0;i < n;i++) std::cin >> v[i];

	LSD_RadixSort<10, 2> (v.begin(), v.end(),get);
	std::reverse(v.begin(), v.end());
	int res = 0;
	for(int i = 0;i < n;i++) res = res + v[i] * (i%2 ? -1 : 1);
	std::cout << res << std::endl;
	return 0;
}

*/

