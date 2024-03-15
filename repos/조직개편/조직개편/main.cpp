#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <unordered_map>
using namespace std;

class Node {
public:
	int num = 0;
	int tot_num = 0;
	Node* lft = nullptr;	Node* rgt = nullptr;	Node* parent = nullptr;
	bool rm = false;
	Node() {}
	void reset() {
		num = 0;
		tot_num = 0;
		lft = nullptr; rgt = nullptr;	parent = nullptr;
		rm = false;	
	}
	void set(int _num, int _tot_num) {
		num = _num;
		tot_num = _tot_num;
		lft = nullptr; rgt = nullptr;	parent = nullptr;
		rm = false; // 나중에 바꿀수도 자식이 붙은 죽은노드일경우엔 바꿔야
	}
};

unordered_map<int, int> id_map;
Node nodes[8001];
int node_ptr;
Node* root = &nodes[1];
int cnt, failed, M;

void init(int mId, int mNum) {

	id_map.clear();
	for (int i = 0; i < 8001; i++) { 
		nodes[i].reset();
	}
	node_ptr = 1;
	id_map[mId] = node_ptr++;
	root->set(mNum, mNum);
	//cout << "Main : " << 1 << endl;
	return;
}

bool check_parent(int PId) {
	int id = id_map[PId];
	Node* now = &nodes[id];
	if (now->lft && now->rgt)	return false;
	else return true;
}

void set_child(Node* C, Node* P) {
	
	int add = C->tot_num;

	if (!P->lft) {
		P->lft = C;	C->parent = P;
	}
	else {
		P->rgt = C;	C->parent = P;
	}

	P->tot_num += add;
	while (P->parent) {
		P = P->parent;
		P->tot_num += add;
	}
}

int add(int CId, int mNum, int PId) {

	if(!check_parent(PId)) return -1;

	int id = node_ptr;
	id_map[CId] = node_ptr++;
	Node* C = &nodes[id];
	C->set(mNum, mNum);

	int Pid = id_map[PId];
	Node* P = &nodes[Pid];

	set_child(C, P);
	//cout << id << "->" << Pid << " : " << mNum << endl;
	//cout << P->tot_num << endl;
	return P->tot_num;
}

void set_rm(Node* N) {
	N->rm = 1;
	if (N->lft) {
		set_rm(N->lft);
	}
	if (N->rgt) {
		set_rm(N->rgt);
	}
}

void erase_child(Node* C) {

	int add = C->tot_num;
	Node* P = C->parent;
	P->tot_num -= add;
	if (P->lft == C)	P->lft = nullptr;
	if (P->rgt == C)	P->rgt = nullptr;

	C->parent = nullptr;
	C->rm = true;

	while (P->parent) {
		P = P->parent;
		P->tot_num -= add;
	}

	if (C->lft) {
		set_rm(C->lft);
	}
	if (C->rgt) {
		set_rm(C->rgt);
	}
}

int remove(int mId) {


	int id = id_map[mId];
	if (id == 0)	return -1;

	Node* N = &nodes[id];
	if (N->rm)	return -1;

	erase_child(N);
	//cout <<"Erased : " << id << endl;
	//cout << N->tot_num << endl;
	return N->tot_num;
}

int get_erased(Node* N, int K) {

	//cout << "접근 : " << N->num << endl;

	if (N->num > K || failed || cnt > M - 1) {
		failed = 1;
		return 1;
	}

	if (!N->lft && !N->rgt)	return 0;

	int lft_erased = 0; int rgt_erased = 0;	int lft_tot = 0; int rgt_tot = 0;

	if (N->lft) {
		lft_erased = get_erased(N->lft, K);
		lft_tot = N->lft->tot_num - lft_erased;
	}
	if (N->rgt) {
		rgt_erased = get_erased(N->rgt, K);
		rgt_tot = N->rgt->tot_num - rgt_erased;
	}

	//cout << N->num << " : " << lft_erased <<" " << rgt_erased << " " << lft_tot <<  " " << rgt_tot << endl;
	
	if (N->tot_num - lft_erased - rgt_erased <= K) { // 총 부서인원서 이걸 뺏을때
		return lft_erased + rgt_erased; // 부서 원활해서 그냥 가도 됨
	}
	if (lft_tot + N->num > K && rgt_tot + N->num > K) { // 둘다 잘라
		cnt += 2;
		return lft_erased + rgt_erased + lft_tot + rgt_tot;
	}
	if (lft_tot + N->num >= K && lft_tot >= rgt_tot) { // 왼쪽거 초과 인데 큰 경우
		cnt++;
		return lft_erased + rgt_erased + lft_tot;
	}
	if (rgt_tot + N->num >= K && rgt_tot > lft_tot) {
		cnt++;
		return lft_erased + rgt_erased + rgt_tot;
	}
	if (lft_tot + rgt_tot + N->num > K) {
		if (lft_tot > rgt_tot) {
			cnt++;
			return lft_erased + rgt_erased + lft_tot;
		}
		else {
			cnt++;
			return lft_erased + rgt_erased + rgt_tot;
		}
	}
}

int reorganize(int _M, int K) {

	cnt = 0;	failed = 0;		M = _M;
	get_erased(root, K);
	if (cnt > M - 1) {
		failed = 1;
	}
	if (failed) {
		//cout << 0 << endl;	
		return 0;
	}
	//cout << "cnt : " << cnt << endl;
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
			cout << "cmd : " << cmd << endl;
			//okay = true;
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