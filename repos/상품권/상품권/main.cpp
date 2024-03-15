#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
/////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <memory.h>
#include <algorithm>
#include <unordered_map>

using namespace std;

int  alive[20000];

class Node {
public:
	int id;
	int person;
	int total_person;

	Node* parent = nullptr;
	vector<Node*> child;

	int child_cnt = 0;

	Node(){}
	Node(int _id, int _person, int _total_person) {
		id = _id;
		person = _person;
		total_person = _total_person;
	}

	int set_node(Node* P) {
		if (P->child_cnt == 3)	return -1;

		this->parent = P;
		P->child.push_back(this);
		P->child_cnt++;
		P->total_person += this->person;

		while (P->parent) {
			P->parent->total_person += this->person;
			P = P->parent;
		}

		return this->parent->total_person;
	}

	void sub_node() {
	
		Node* P = parent;
		int idx = 0;
		for (int i = 0; i < P->child_cnt; i++) {
			if (P->child[i] == this) {
				idx = i;
				break;
			}
		}
		P->child.erase(P->child.begin() + idx);
		P->child_cnt--;
		P->total_person -= this->total_person;
		while (P->parent) {
			P->parent->total_person -= this->total_person;
			P = P->parent;
		}
	
	}

	void remove_node() {

		alive[id] = -1;

		for (int i = 0; i < child_cnt; i++) {
			child[i]->remove_node();
		}
		parent = nullptr;
		child.clear();
		child_cnt = 0;

	}
	
};

Node nodes[20000];
unordered_map<int, int> ids;
int ptr = 0;

vector<int> root_nodes;
int root_size;


void init(int N, int mId[], int mNum[]) {
	memset(alive, 0, sizeof(alive));
	root_nodes.clear();
	root_size = N;
	ptr = 0;
	ids.clear();

	for (int i = 0; i < N; i++) {
		Node* A = &nodes[ptr];
		A->id = ptr;
		A->person = mNum[i];
		A->total_person = mNum[i];
		A->child.clear();
		A->child_cnt = 0;
		A->parent = nullptr;
		alive[ptr] = 1;
		root_nodes.push_back(ptr);
		ids[mId[i]] = ptr++;
	}
	return;
}

int add(int mId, int mNum, int mParent) {

	Node* A = &nodes[ptr];
	
	A->id = ptr;
	A->person = mNum;
	A->total_person = mNum;
	A->child.clear();
	A->child_cnt = 0;
	A->parent = nullptr;
	alive[ptr] = 1;
	ids[mId] = ptr++;
	
	mParent = ids[mParent];
	Node* P = &nodes[mParent];
	int answer = A->set_node(P);

	
	return answer;
}

int remove(int mId) {
	mId = ids[mId];
	if (alive[mId] == -1 || alive[mId] == 0)	return -1;

	Node* N = &nodes[mId];

	int answer = N->total_person;

	N->sub_node();

	N->remove_node();

	return answer;
}

int distribute(int K) {

	vector<int> members;
	int tot_mem = 0;
	int max_num = 0;

	for (int i = 0; i < root_size; i++) {
		members.push_back(nodes[root_nodes[i]].total_person);
		tot_mem += members[i];
		max_num = max(max_num, members[i]);

	}

	if (K >= tot_mem) return max_num;

	int start = 0, end = max_num;

	int answer = 0;

	while (start <= end) {
		int mid = start + (end - start) / 2;

		int ticket = 0;

		for (int i = 0; i < root_size; i++) {
			if (members[i] <= mid)	ticket += members[i];
			else                    ticket += mid;
		}
		if (ticket > K) {
			end = mid - 1;
			continue;
		}
		else {
			answer = mid;
			start = mid + 1;
			continue;			
		}

	}

	return answer;
}

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_DISTRIBUTE 4

static bool run() {
	int q;
	scanf("%d", &q);

	static int midArr[1000], mnumArr[1000];
	int mid, mnum, mparent, n, k;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			scanf("%d", &n);
			for (int j = 0; j < n; ++j) {
				scanf("%d %d", &midArr[j], &mnumArr[j]);
			}
			init(n, midArr, mnumArr);
			okay = true;
			break;
		case CMD_ADD:
			scanf("%d %d %d %d", &mid, &mnum, &mparent, &ans);
			ret = add(mid, mnum, mparent);
			if (ans != ret)
				okay = false;
			break;
		case CMD_REMOVE:
			scanf("%d %d", &mid, &ans);
			ret = remove(mid);
			if (ans != ret)
				okay = false;
			break;
		case CMD_DISTRIBUTE:
			scanf("%d %d", &k, &ans);
			ret = distribute(k);
			if (ans != ret)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
		if (okay == false) {
			cout << cmd << endl;
		}
		//print();
	}
	return okay;
}

int main() {
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}