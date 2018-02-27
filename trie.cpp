#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <cmath>
#include <map>
#include <set>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#define INF_LL (ll)1e18
#define INF (int)1e9
#define REP(i, n) for(int i = 0;i < (n);i++)
#define FOR(i, a, b) for(int i = (a);i < (b);i++)
#define all(x) x.begin(),x.end()
using ll = long long;
using PII = pair<int, int>;

const double eps = 1e-10;

template<typename A, typename B>inline void chmin(A &a, B b){if(a > b) a = b;}
template<typename A, typename B>inline void chmax(A &a, B b){if(a < b) a = b;}

struct Node{
	int ch[26];
	int cost;

	Node(){
		memset(ch, -1, sizeof ch);
		cost = 0;
	}
};

vector<Node> trie;

void add_str(int v, const string &s, int i, int cost){
	if(s.size() == i){
		trie[v].cost = cost;
		return;
	}

	if(trie[v].ch[s[i]-'a'] == -1){
		trie.push_back(Node());
		trie[v].ch[s[i]-'a'] = trie.size()-1;
	}

	add_str(trie[v].ch[s[i]-'a'], s, i+1, cost);
}

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
