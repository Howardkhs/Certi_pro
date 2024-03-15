#include <iostream>

using namespace std;

class Node {
public:
	bool empty = true;
	int id;
	Node* parent = nullptr;
	Node(){}
	Node(int _id) {
		id = _id;
		parent = nullptr;
	}

	Node* get_parent() {
		if (parent) {
			return parent = parent->get_parent(); // 부모를 셋하면서 감 이거 확인하자
		}
		return this;
	}

	
};

Node nodes[100001];
int stop = 0;
int cnt = 0;

void united(Node* N, Node* P) {

	N->parent = P;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(NULL); 

	int G, P, num;

	for (int i = 0; i < 100001; i++) {
		nodes[i].id = i;
	}

	cin >> G >> P;
	for (int i = 0; i < P; i++) {
		
		cin >> num;
		Node* N = &nodes[num];

		Node *P_N = N->get_parent();
		if (P_N->id == 0) {
			cout << cnt;
			return 0;
		}
		Node* P = nodes[P_N->id -1].get_parent();
		united(P_N, P);
		cnt++;
	}
	cout << cnt;
	return 0;

}