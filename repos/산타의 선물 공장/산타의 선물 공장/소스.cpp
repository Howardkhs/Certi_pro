#include <iostream>
#include <unordered_map>

using namespace std;

class Node;
Node* heads[100001];
Node* tails[100001];

class Node {
public:
	int bid;  // 0이면 없는 박스
	int oid;
	int id;
	int w;
	Node* prv; Node* nxt;
	Node(){}
	void box_set(int _bid, int _oid, int _id) {
		bid = _bid; oid = _oid; id = _id;
	}
	int pop(int _w) {
		heads[bid] = nxt;
		if (nxt)	nxt->prv = nullptr;
		else		tails[bid] = nullptr;
		nxt = nullptr;
		
		if (w <= _w) {
			bid = 0;
			return w;
		}
		Node* T = tails[bid];
		if (!T) {
			heads[bid] = tails[bid] = this; 
		}
		else {
			T->nxt = this;
			this->prv = T;
			tails[bid] = this;
		}

		return 0;

	}
	void rm() {
		if (prv && nxt) {
			prv->nxt = nxt;
			nxt->prv = prv;
		}
		else if (prv) {  // 이게 테일이란 뜻
			tails[bid] = prv;
			prv->nxt = nullptr;

		}
		else if (nxt) { // 이게 헤드란 뜼
			heads[bid] = nxt;
			nxt->prv = nullptr;
		}
		else {
			heads[bid] = tails[bid] = nullptr;
		}
		prv = nxt = nullptr;
		bid = 0;
	}
	void mv() {
		Node* head = heads[bid];
		Node* tail = tails[bid];
		if (head == tail || head == this)	return;
		tails[bid] = prv;
		heads[bid] = this;
		prv->nxt = nullptr;
		prv = nullptr;
		tail->nxt = head;
		head->prv = tail;
	}
};

Node boxes[100001];
bool broken[100001];

unordered_map<int, int> oid_map;
int q, n, m, order;
int node_ptr = 1;

int get_id(int oid) {
	int id = oid_map[oid];
	if (id == 0) {
		id = node_ptr++;
		oid_map[oid] = id;
	}
	return id;
}

void init() {
	cin >> n >> m;
	memset(boxes, 0, sizeof boxes);
	memset(heads, 0, sizeof heads);
	memset(tails, 0, sizeof tails);
	memset(broken, 0, sizeof broken);

	int oid, w;
	int cnt = 0;
	int bid = 1;
	int div = n / m;

	for (int i = 1; i <= n; i++) {
		cin >> oid;
		int id = get_id(oid);
		Node* N = &boxes[id];
		N->box_set(bid, oid, id);
		if (cnt == 0) {
			cnt++;
			heads[bid] = N;
			continue;
		}
		cnt++;
		Node* B = &boxes[id- 1];
		B->nxt = N;
		N->prv = B;
		if (cnt == div) {
			tails[bid] = N;
			bid++;
			cnt = 0;
		}
	}
	for (int i = 1; i <= n; i++) {
		cin >> w;
		Node* N = &boxes[i];
		N->w = w;
	}
}

void box_pop() {
	int w;
	int answer = 0;

	cin >> w;
	for (int i = 1; i <= m; i++) {
		if (heads[i]) {
			answer += heads[i]->pop(w);
		}
	}
	cout << answer << "\n";
}

void rm_box() {
	int r_id;
	cin >> r_id;
	int id = get_id(r_id);
	if (id > n) {
		cout << -1 << "\n";
		return;
	}
	Node* N = &boxes[id];
	if (!boxes[id].bid) {
		cout << -1 << "\n";
		return;
	}
	N->rm();
	cout << r_id << endl;
}

void check_box(){
	int r_id;
	cin >> r_id;
	int id = get_id(r_id);
	if (id > n) {
		cout << -1 << "\n";
		return;
	}
	Node* N = &boxes[id];
	if (!boxes[id].bid) {
		cout << -1 << "\n";
		return;
	}
	N->mv();
	cout << N->bid << "\n";
}

void merge(int bid, int did) {
	Node* BH = heads[bid]; Node* BT = tails[bid];
	Node* DH = heads[did]; Node* DT = tails[did];

	if (!BH)	return;
	if (!DH && !DT) {
		heads[did] = BH; tails[did] = BT;
	}
	else {
		DT->nxt = BH;
		BH->prv = DT;
		tails[did] = BT;
	}
	heads[bid] = tails[bid] = nullptr;

	Node* N = BH;
	N->bid = did;
	while (N->nxt) {
		N = N->nxt;
		N->bid = did;
	}
}

void broken_belt() {
	int bid;
	cin >> bid;
	if (broken[bid]) {
		cout << -1 << "\n";
		return;
	}
	int did = bid;
	broken[bid] = 1;

	while (1) {
		did += 1;
		if (did == m + 1)	did = 1;
		if (broken[did])	continue;
		merge(bid, did);	break;
	}
	cout << bid << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	cin >> q;

	while (q--) {
		cin >> order;
		if (order == 100)		init();
		else if (order == 200)	box_pop();
		else if (order == 300)	rm_box();
		else if (order == 400)	check_box();
		else if (order == 500)	broken_belt();
	}
}