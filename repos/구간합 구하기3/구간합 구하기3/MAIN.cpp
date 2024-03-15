#include <iostream>

using namespace std;

long long temp_sum;

class Node {
public:
	int start;
	int end;
	int num = 0;

	Node* lft; Node* rgt;
	Node(){}
	void set(int _start, int _end) {
		start = _start; end = _end;
	}
	void sum(int _start, int _end) {
	
		if (start == _start && end == _end) {
			temp_sum += num;
			return;
		}
		int mid = (start + end) / 2;
		
		if (_end <= mid) {
			lft->sum(_start, _end);
		}
		else if (mid < _start) {
			rgt->sum(_start, _end);
		}
		else {
			lft->sum(_start, mid);
			rgt->sum(mid + 1, _end);
		}
	}
	void change(int _start, int _end, int _num) {
		if (start == _start && end == _end) {
			num = _num;
			return;
		}
		int mid = (start + end) / 2;

		if (_end <= mid) {
			lft->change(_start, _end, _num);
		}
		else if (mid < _start) {
			rgt->change(_start, _end, _num);
		}
		num = lft->num + rgt->num;
	}
	
};

Node nodes[1024][1024 * 2 + 1];

int node_ptr;
int N, M, a;
int lines[1024][1024];

Node* set_node(int i, int start, int end) {
	Node* N = &nodes[i][node_ptr++];
	N->set(start, end);
	if (start == end) {
		N->num = lines[i][start];
		return N;
	}
	int mid = (start + end) / 2;
	N->lft = set_node(i, start, mid);
	N->rgt = set_node(i, mid + 1, end);
	N->num = N->lft->num + N->rgt->num;
	return N;
}
int order, x_1, y_1, x_2 ,y_2;
long long sum, num;
int main() {
	ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

	cin >> N >> M;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> lines[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		node_ptr = 0;
		set_node(i, 0, N - 1);
	}

	while (M--) {
		cin >> order;
		if (order == 1) {
			temp_sum = 0;
			cin >> x_1 >> y_1 >> x_2 >> y_2;

			for (int i = x_1; i <= x_2; i++) {
				nodes[i - 1][0].sum(y_1 - 1, y_2 - 1);
			}

			temp_sum;
			cout << temp_sum << "\n";
		}
		else {
			cin >> x_1 >> y_2 >> num;
			nodes[x_1 - 1][0].change(y_2 -1, y_2-1, num);
		}
	}
}