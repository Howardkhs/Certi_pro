#include<iostream>

using namespace std;

int min_a = 1000000001;
int max_a = 0;

class Node {
public:
	int start;
	int end;
	int max_num = 0;
	int min_num = 1000000001;

	Node* lft; Node* rgt;
	Node(){}
	void set(int _start, int _end) {
		start = _start;
		end = _end;
	}
	void input(int _start, int _end, int _num) {
		if (start == _start && end == _end) {
			max_num = _num;
			min_num = _num;
			return;
		}

		int mid = (start + end) / 2;
		if (_start <= mid) {
			lft->input(_start, _end, _num);
		}
		else {
			rgt->input(_start, _end, _num);
		}
		max_num = max(lft->max_num, rgt->max_num);
		min_num = min(lft->min_num, rgt->min_num);
	}
	void find(int _start, int _end) {
		if (start == _start && end == _end) {
			min_a = min(min_a, min_num);
			max_a = max(max_a, max_num);
			return;
		}

		int mid = (start + end) / 2;
		if (_end <= mid) {
			lft->find(_start, _end);
		}
		else if (_start > mid) {
			rgt->find(_start, _end);
		}
		else {
			lft->find(_start, mid);
			rgt->find(mid+1, _end);
		}
	}
};

Node nodes[200004];
int ptr = 0;
Node* root = &nodes[0];

Node* init(int start, int end) {
	Node* now = &nodes[ptr++];
	now->set(start, end);
	if (start == end)	return now;

	int mid = (start + end) / 2;
	now->lft = init(start, mid);
	now->rgt = init(mid + 1, end);
	return now;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);

	init(0, 100000);
	int N, M;
	cin >> N >> M;
	int a;
	int cnt = 0;
	while (N--) {
		cin >> a;
		cnt++;
		root->input(cnt, cnt, a);
	}

	while (M--) {
		int a, b;
		cin >> a >> b;
		root->find(a, b);
		cout << min_a << " " << max_a << "\n";
		min_a = 1000000001;
		max_a = 0;

	}

}