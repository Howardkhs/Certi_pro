

#include <iostream>
#include <set>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <numeric>

using namespace std;

class Node {
public:
	int id = 0;
	int depth = 0;
	int authority = 0;
	int on = 1;
	Node* parent = nullptr;
	Node* lft = nullptr; Node* rgt = nullptr;
	struct compare {
		bool operator()(const Node* A, const Node* B) const {
			return A->id < B->id;
		}
	};
	int childs[30] = { 0, };

	Node() {
		parent = nullptr;
		lft = nullptr; rgt = nullptr;
	};
	void set_node(int _id) {
		id = _id; authority = 0; on = 1;
	}
};

Node nodes[200001];
int Pids[200'001];
int authorities[200'001];

int n, Q, order;

struct compare {
	bool operator()(const Node* A, const Node* B) const {
		return A->id < B->id;
	}
};

void set_depth(Node *N, int depth) {
	N->depth = depth;
	if (N->lft)	set_depth(N->lft, depth + 1);
	if (N->rgt)	set_depth(N->rgt, depth + 1);
}

void init() {

	for (int i = 1; i < n + 1; i++) {
		cin >> Pids[i];
	}
	for (int i = 1; i < n + 1; i++) {
		cin >> authorities[i];
		if (authorities[i] > 25) {
			authorities[i] = 25;
		}
	}

	for (int i = 1; i < n + 1; i++) {
		Node* N = &nodes[i];
		Node* P = &nodes[Pids[i]];

		N->set_node(i);
		N->parent = P;
		N->authority = authorities[i];
		if (!P->lft)	P->lft = N;
		else			P->rgt = N;
	}

	Node* P = &nodes[0];
	set_depth(P, 0);

	for (int i = 1; i < n + 1; i++) {
		Node* N = &nodes[i];
		
		Node* P = N->parent;
		int autor = N->authority;
		N->childs[autor] += 1;
		while (autor) {
			autor--;
			P->childs[autor] += 1;
			P = P->parent;
			if (!P)	break;
		}
	}

}

void erase(Node* N, int times, int auth) {
	Node* P = N;
	while (auth) {
		if (!P->on)	break; // 부모가 off면 멈춰
		auth--;
		P = P->parent;
		if (!P) break;
		P->childs[auth] -= times; // 이게 N의 자식들
	}
}

void alarm_off(Node* N) {

	for (int auth = 1; auth < 25; auth++) {
		int times = N->childs[auth];
		if (times) erase(N, times, auth);
	}

	N->on = 0;
}

void add(Node* N, int times, int auth) {
	Node* P = N;
	while (auth) {
		if (!P->on)	break; // 부모가 off면 멈춰
		auth--;
		P = P->parent;
		if (!P) break;
		P->childs[auth] += times; // 이게 N의 자식들
	}
}

void alarm_on(Node* N) {
	N->on = 1;

	for (int auth = 1; auth < 25; auth++) {
		int times = N->childs[auth];
		if (times) add(N, times, auth);
	}
	
}

void alram() {
	int id;
	cin >> id;

	Node* N = &nodes[id];
	if (N->on) { 
		alarm_off(N);
	}
	else {
		alarm_on(N);
	}
}

void power_change() {

	int id, power;
	cin >> id >> power;
	if (power > 25)	power = 25;
	Node* N = &nodes[id];
	
	int left_auth = N->authority;
	N->childs[left_auth] -= 1;
	if (left_auth)
		erase(N, 1 ,left_auth); 
	
	N->authority = power;

	left_auth = N->authority;
	N->childs[left_auth] += 1;
	if (left_auth)
		add(N, 1, left_auth);
}

void node_swap() {

	int c1, c2;
	cin >> c1 >> c2;

	Node* N1 = &nodes[c1]; Node* P1 = N1->parent;
	Node* N2 = &nodes[c2]; Node* P2 = N2->parent;

	if (P1 == P2)	return;

	for (int auth = 1; auth < 25; auth++) {
		int times = N1->childs[auth];
		if (times) erase(N1, times, auth);
	}
	for (int auth = 1; auth < 25; auth++) {
		int times = N2->childs[auth];
		if (times) erase(N2, times, auth);
	}

	N1->parent = P2;

	for (int auth = 1; auth < 25; auth++) {
		int times = N1->childs[auth];
		if (times) add(N1, times, auth);
	}
	
	N2->parent = P1;

	for (int auth = 1; auth < 25; auth++) {
		int times = N2->childs[auth];
		if (times) add(N2, times, auth);
	}

}

void see() {
	int id;
	cin >> id;
	Node* N = &nodes[id];

	cout << accumulate(N->childs, N->childs + 30, 0) - 1 << "\n";
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);

	freopen("sample_input.txt", "r", stdin);
	cin >> n >> Q;

	while (Q--) {
		cin >> order;

		if (order == 100)		init();
		else if (order == 200)	alram();
		else if (order == 300)	power_change();
		else if (order == 400)	node_swap();
		else					see();
	}




}