#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <memory.h>
#include <unordered_map>

using namespace std;

class Node {
public:
	int id;
	int man;
	int tot_man;
	bool live;

	Node* parent;
	Node* lft; Node* rgt;
	Node(){}
	void make_node(int _id, int _man) {
		id = _id; man = _man; tot_man = _man; live = true;
	}
};

Node nodes[9001];
Node* root;

unordered_map<int, int> id_map;

int node_ptr;

int M;
int cnt;

int get_id(int CId) {
	int id = id_map[CId];
	if (id == 0) {
		id = node_ptr++;
		id_map[CId] = id;
	}
	return id;
}

void init(int mId, int mNum) {
	memset(nodes, 0, sizeof nodes);
	node_ptr = 1;
	id_map.clear();

	int id = get_id(mId);
	root = &nodes[id];
	root->make_node(id, mNum);
}

int add_node(Node* P, Node* C) {

	if (!P->lft)	P->lft = C;
	else			P->rgt = C;
	C->parent = P;

	int new_man = C->man;
	int answer;
	
	P->tot_man += new_man;
	answer = P->tot_man;

	while (P->parent) {
		P = P->parent;
		P->tot_man += new_man;
	}
	return answer;
}

int add(int CId, int mNum, int PId) {

	int pid = get_id(PId);
	Node* P = &nodes[pid];

	if (P->lft && P->rgt)	return -1;

	int cid = get_id(CId);
	Node* C = &nodes[cid];
	
	C->make_node(cid, mNum);
	
	int answer = add_node(P, C);
	//cout << answer << endl;
	return answer;
}

void set_rm(Node* N) {
	N->live = false;
	if (N->lft)	set_rm(N->lft);
	if (N->rgt)	set_rm(N->rgt);
}

int remove(int mId) {

	int id = get_id(mId);
	
	Node* N = &nodes[id];
	if (!N->live)	return -1;

	set_rm(N);

	int del_man = N->tot_man;

	Node* P = N->parent;
	
	N->parent = nullptr;
	if (P->lft == N)	P->lft = nullptr;
	else				P->rgt = nullptr;
	
	P->tot_man -= del_man;
	while (P->parent) {
		P = P->parent;
		P->tot_man -= del_man;
	}
	//cout << del_man << endl;
	return del_man;
}

int fail;

int partition_node(Node* N, int K) {

	if (fail)	return 0;

	int lft_done = 0;
	int rgt_done = 0;

	if (N->lft)	lft_done = partition_node(N->lft, K);
	if (N->rgt) rgt_done = partition_node(N->rgt, K);

	if (N->man > K) {
		fail = 1;
		return 0;
	}
	int tot_man;
	int lft_man = 0;
	int rgt_man = 0;

	tot_man = N->tot_man - lft_done - rgt_done;
	if (N->lft)	lft_man = N->lft->tot_man - lft_done;
	if (N->rgt) rgt_man = N->rgt->tot_man - rgt_done;

	if (tot_man <= K) { // 3개 다합쳐도 작아
		//cout << N->man << endl;
		return lft_done + rgt_done;
	}/*
	if (tot_man == K) { // 3개 다합치면 딱맞아
		cnt++;
		return lft_done + rgt_done + tot_man;
	}*/
	if (lft_man >= rgt_man && tot_man - lft_man <= K) {
		cnt++;
		return lft_done + rgt_done + lft_man;
	}
	/*
	if (lft_man > rgt_man && tot_man - lft_man == K) {
		cnt++;	cnt++;
		return lft_done + rgt_done + tot_man;
	}*/
	if (rgt_man > lft_man && tot_man - rgt_man <= K) {
		cnt++;
		return lft_done + rgt_done + rgt_man;
	}
	/*
	if (rgt_man > lft_man && tot_man - rgt_man == K) {
		cnt++;	cnt++;
		return lft_done + rgt_done + tot_man;
	}*/
	if (tot_man - rgt_man > K && tot_man - lft_man > K && tot_man - lft_man - rgt_man <= K) {
		cnt++; cnt++;
		return lft_done + rgt_done + lft_man + rgt_man;
	}
	/*
	if (tot_man - rgt_man > K && tot_man - lft_man > K && tot_man - lft_man - rgt_man == K) {
		cnt++; cnt++; cnt++;
		return lft_done + rgt_done + tot_man;
	}*/
}

int reorganize(int _M, int K) {
	fail = 0;
	M = _M;
	M--;
	cnt = 0;
	int del = partition_node(root, K); // k가 목표치 인원
	if (root->man == K) cnt--;
	//cout << del << endl;
	//cout << "cnt : " << cnt << endl;
	if (fail) { 
		//cout << 0 << endl;
		return 0;
	}

	if (cnt > M) {
		//cout << 0 << endl;
		return 0;
	}

	
	//cout << 1 << endl;
	return 1;
}

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_REORGANIZE 4

static bool run() {
	int q;
	scanf("%d", &q);

	int mid, mnum, mparent, m, k;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			scanf("%d %d", &mid, &mnum);
			init(mid, mnum);
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
		case CMD_REORGANIZE:
			scanf("%d %d %d", &m, &k, &ans);
			ret = reorganize(m, k);
			if (ans != ret)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
		if (okay == false) {
			cout << "cmd :" << cmd << endl;
			okay = true;
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