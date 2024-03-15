#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Node {
public:
	int id;
	int num;
	int tot_num;
	bool rm;
	Node* parent;
	vector<Node*> childs;
	Node(){}
	void reset() {
		num = 0; tot_num = 0; parent = nullptr; childs.clear(); rm = false;
	}
};

Node nodes[19001];
vector<int> root_vec;
unordered_map<int, int> id_map;
int node_ptr;

void init(int N, int mId[], int mNum[]) {

	node_ptr = 1;
	for (int i = 0; i < 17001; i++) {
		nodes[i].reset();
		nodes[i].id = i;
	}
	root_vec.clear();
	id_map.clear();

	for (int i = 0; i < N; i++) {
		int id = node_ptr;
		id_map[mId[i]] = node_ptr++;
		Node* N = &nodes[id];
		N->num = mNum[i];
		N->tot_num = mNum[i];
		root_vec.push_back(id);
	}

	return;
}

int add(int mId, int mNum, int mParent) {

	int Pid = id_map[mParent];
	Node* P = &nodes[Pid];
	if (P->childs.size() == 3)	return -1;

	int id = node_ptr;
	id_map[mId] = node_ptr++;
	Node* N = &nodes[id];
	N->num = mNum;
	N->tot_num = mNum;

	P->childs.push_back(N);
	N->parent = P;
	
	P->tot_num += mNum;
	while (P->parent) {
		P = P->parent;
		P->tot_num += mNum;
	}

	return N->parent->tot_num;
}

void rm(Node* N) {
	N->rm = true;

	for (int i = 0; i < N->childs.size(); i++) {
		rm(N->childs[i]);
	}
}

int remove(int mId) {

	int id = id_map[mId];
	if (id == 0)	return -1;
	Node* N = &nodes[id];
	if (N->rm)	return -1;
	
	rm(N);

	int cnt = N->tot_num;
	Node* P = N->parent;

	P->childs.erase(remove(P->childs.begin(), P->childs.end(), N), P->childs.end());
	
	P->tot_num -= cnt;
	while (P->parent) {
		P = P->parent;
		P->tot_num -= cnt;
	}
	N->parent = nullptr;

	return cnt;
}

vector<int> temp;

int set_K(int K, int tot) {

	int start = 0; int end = *max_element(temp.begin(), temp.end());

	if (tot <= K) {
		return end;
	}

	int answer;

	while (start <= end) {
		int mid = (start + end) / 2;
		int cnt = 0;
		for (int i = 0; i < temp.size(); i++) {
			int num = temp[i];
			if (num < mid) {
				cnt += num;
				continue;
			}
			cnt += mid;
		}
		if (cnt <= K) {
			answer = mid;
			start = mid + 1;
		}
		else {
			end = mid - 1;
		}
	}
	return answer;
}

int distribute(int K) {

	temp.clear();

	int tot = 0;
	for (int i = 0; i < root_vec.size(); i++) {
		temp.push_back(nodes[root_vec[i]].tot_num);
		tot += temp[i];
	}

	int answer = set_K(K, tot);
	
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
		}
	
		
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