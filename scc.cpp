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

class SCC{
private:
	vector<vector<int> > gg, rg;
	vector<int> order, comp;
	vector<bool> used;
	vector<vector<int> > ng, vs;

	int n, nn;
public:
	SCC(){}
	SCC(int v) : gg(v), rg(v), comp(v, -1), used(v, 0), n(v){}

	void add_edge(int x, int y){
		gg[x].push_back(y);
		rg[y].push_back(x);
	}

	int operator[](int k){
		return comp[k];
	}

	void dfs(int v){
		used[v] = true;
		REP(i, gg[v].size()){
			if(!used[gg[v][i]]) dfs(gg[v][i]);
		}
		order.push_back(v);
	}

	void rdfs(int v, int k){
		used[v] = true;
		comp[v] = k;
		REP(i, rg[v].size()){
			if(!used[rg[v][i]]) rdfs(rg[v][i], k);
		}
	}

	int build(){
		REP(i, n){
			if(!used[i]) dfs(i);
		}
		fill(all(used), 0);
		int k = 0;
		for(int i = order.size()-1;i >= 0;i--){
			if(!used[order[i]]) rdfs(order[i], k++);
		}
		nn = k;

		//---------それぞれの強連結成分に含まれる頂点の番号----------
		vs.resize(k, vector<int>());

		REP(i, n)
			vs[comp[i]].push_back(i);
		//-----------------------------------------------------------

		//---------強連結成分をまとめた後のNew Graph!----------------
		ng.resize(k, vector<int>());
		REP(i, n){
			REP(j, gg[i].size()){
				if(comp[i] != comp[gg[i][j]])
					ng[comp[i]].push_back(comp[gg[i][j]]);
			}
		}
		REP(i, nn){
			sort(all(ng[i]));
			ng[i].erase(unique(all(ng[i])), ng[i].end());
		}
		//------------------------------------------------------------
		return k;
	}
	
	int size(){
		return nn;
	}

	vector<vector<int> > graph(){
		return ng;
	}

	vector<int> vertices(int v){
		return vs[v];
	}
};

class TwoSAT{
private:
	SCC scc;
	int n;
	vector<bool> truth;
public:
	TwoSAT(){}
	TwoSAT(int _n) : n(_n), scc(2*_n){}

	void add_state(int a, bool truth1, int b, bool truth2){
		int na = truth1 ? a+n : a, nb = truth2 ? b+n : b;
		a = truth1 ? a : a+n; b = truth2 ? b : b+n;
		scc.add_edge(na, b);
		scc.add_edge(nb, a);
	}

	bool build(){
		scc.build();
		REP(i, n){
			if(scc[i] == scc[i+n]) return false;
			if(scc[i] > scc[i+n]) truth.push_back(true);
			else truth.push_back(false);
		}
		return true;
	}

	vector<bool> result(){
		return truth;
	}

	bool operator[](int k){
		return truth[k];
	}
};

int main(void){
	cin.tie(0);
	ios::sync_with_stdio(false);
}
