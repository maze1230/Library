#include <bits/stdc++.h>

template<class RandomAccessIterator>
void LSD_RadixSort(RandomAccessIterator first, RandomAccessIterator last,
		int maximum, int max_digit,
		::std::function<int(typename RandomAccessIterator::value_type, int)> get_digit){
	using value_type = typename RandomAccessIterator::value_type;

	::std::size_t size = ::std::distance(first, last);
	::std::vector<::std::vector<int>> bucket(maximum+1);
	::std::vector<value_type> v(first, last), tmp;

	int digit = 0;

	for(int digit = 0;digit < max_digit;digit++){
		for(int i = 0;i < size;i++){
			int x = get_digit(v[i], digit);
			bucket[x].push_back(i);
		}

		for(int i = 0;i <= maximum;i++){
			for(int j = 0;j < bucket[i].size();j++){
				tmp.push_back(v[bucket[i][j]]);
			}
			bucket[i].clear();
		}

		::std::swap(v, tmp);
		tmp.clear();
	}
	::std::move(v.begin(), v.end(), first);
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
	return x >> dig & 1;
}

int main(void){
	int n;
	::std::vector<int> v;
	std::cin >> n;
	v.resize(n);
	for(int i = 0;i < n;i++) std::cin >> v[i];

	LSD_RadixSort(v.begin(), v.end(), 2, 32, get);
	std::reverse(v.begin(), v.end());
	int res = 0;
	for(int i = 0;i < n;i++) res = res + v[i] * (i%2 ? -1 : 1);
	std::cout << res << std::endl;
	return 0;
}

*/

