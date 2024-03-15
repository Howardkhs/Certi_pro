#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Node {
public:
	double x;
	double y;

	Node* parent = nullptr;

	Node(){}
	void set(double _x, double _y) {
		x = _x; y = _y;
	}

	Node* find_parent() {
		if (parent)
			return parent = parent->find_parent();
		return this;
	}
};

Node nodes[1001];

struct P {
	double dis;
	int to;
	int from;
	P(){}
	P(double _dis, int _to, int _from) {
		dis = _dis; to = _to; from = _from;
	}
	void set(double _dis, int _to, int _from) {
		dis = _dis; to = _to; from = _from;
	}
};

bool compare(P& A, P& B) {
	return A.dis < B.dis;
}

int main() {
	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

	int N, M;
	int A, B;
	double a, b;
	cin >> N >> M;

	vector<P> P_vec;

	for (int i = 1; i < N + 1; i++) {
		cin >> a >> b;
		nodes[i].set(a, b);
	}

	for (int i = 1; i < N + 1; i++) {
		for (int j = i+1; j < N + 1; j++) {
			if (i == j)	continue;
			int x_0 = nodes[i].x; int y_0 = nodes[i].y;
			int x_1 = nodes[j].x; int y_1 = nodes[j].y;
			double dis = sqrt(pow((x_0 - x_1), 2) + pow((y_0 - y_1), 2));
			P_vec.push_back({ dis , j, i });
		}
	}
	

	while (M--) {
		cin >> A >> B;
		Node* P_B = nodes[B].find_parent();
		Node* P_A = nodes[A].find_parent();
		if (P_B == P_A)	continue;

		if (A < B) {
			P_B->parent = P_A;
		}
		else {
			P_A->parent = P_B;
		}
	}

	sort(P_vec.begin(), P_vec.end(), compare);
	double answer = 0;
	for (int i = 0; i < P_vec.size(); i++) {
		double dis = P_vec[i].dis; int A = P_vec[i].to; int B = P_vec[i].from;
		Node* P_B = nodes[B].find_parent();
		Node* P_A = nodes[A].find_parent();
		if (P_B == P_A)	continue;
		
		P_B->parent = P_A;
		answer += dis;
	}
	cout << fixed;
	cout.precision(2);
	cout << answer << endl;

}