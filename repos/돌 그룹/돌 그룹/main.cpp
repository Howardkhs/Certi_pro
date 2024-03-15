#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

int finds = 0;
class T {
public:
	int a; int b; int c;
	T(){}
	T(int _a, int _b, int _c) {
		a = _a;	b = _b;	c = _c;
	}
};


vector<T> S;

void run(int a, int b, int c) {
	if (finds)	return;
	int _a = a;	int _b = b; int _c = c;

	
	T A(a, b, c);

	for (int i = 0; i < S.size(); i++) {
		if (S[i].a == a && S[i].b == b && S[i].c == c)	return;
	}
	cout << a << " " <<  b << " " << c << endl;
	S.push_back(A);
	

	if (a == b && b == c) {
		finds = 1;
		return;
	}

	if (a > b) {
			a -= b;	b *= 2;
			run(a, b, c);
		
	}

	a = _a; b = _b; c = _c;
	if (b > a) {
		
			b -= a; a *= 2;
			run(a, b, c);
		
	}
	
	a = _a; b = _b; c = _c;
	if (c > a) {
			c -= a; a *= 2;
			run(a, b, c);
	
	}

	a = _a; b = _b; c = _c;
	if (a > c) {
		
			a -= c;	c *= 2;
			run(a, b, c);
		
	}
	a = _a; b = _b; c = _c;
	if (b > c) {
		
			b -= c;	c *= 2;
			run(a, b, c);
		
	}
	a = _a; b = _b; c = _c;
	if (c > b) {
		
			c -= b; b *= 2;
			run(a, b, c);
		
	}
}


int main() {
	int a, b, c;
	cin >> a >> b >> c;
	if ((a + b + c) % 3 != 0) { 
		cout << 0 << endl; 
		return 0;
	}
	run(a, b, c);
	cout << finds << endl;
	
}