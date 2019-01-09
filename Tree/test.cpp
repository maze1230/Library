#include <iostream>
using namespace std;

class C{
	int a, b;
public:
	C(int x):a(x), b(a*2){
		cout << a << " " << b << endl;
	}
};

int main(void){
	C c(2);
}
