#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define BAR_LEN 5

#define CMD_INIT 100
#define CMD_MAKEWALL 200
#define CMD_MATCHPIECE 300

#include <iostream>
#include <memory.h>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

using namespace std;

class Node {
public:
	int id;
	int height;
	int rm = 0;
	Node* nxt;
	Node* prv;
	
	Node(){}
	Node(int _id, int _height) {
		id = _id; height = _height;
	}
};

int erase_cnt;

class Tree {
public:
	int start; 
	int end;
	int sum;

	Tree* lft;
	Tree* rgt;
	Tree(){}
	void reset(int _start, int _end) {
		start = _start; end = _end;
	}
	void add(int _start, int _end) {
		if (start == _start && end == _end) {
			sum += 5;
			return;
		}
		int mid = (start + end) / 2;

		if (_end <= mid) {
			lft->add(_start, _end);
		}
		else {
			rgt->add(_start, _end);
		}
		sum = lft->sum + rgt->sum;
	}
	void finds(int _start, int _end) {
		if (start == _start && _end == end) {
			erase_cnt += sum;
			return;
		}
		int mid = (start + end) / 2;

		if (_end <= mid) {
			lft->finds(_start, _end);
		}
		else if (mid < _start) {
			rgt->finds(_start, _end);
		}
		else {
			lft->finds(_start, mid);
			rgt->finds(mid + 1, _end);
		}
	}
};

Node nodes[100001];
Tree trees[200001];
Tree* root;
int node_ptr;
int tree_ptr;
int start_point;

Tree* set_tree(int _start, int _end) {
	Tree* N = &trees[tree_ptr++];
	N->reset(_start, _end);
	if (_start == _end)	return N;

	int mid = (_start + _end) / 2;
	N->lft = set_tree(_start, mid);
	N->rgt = set_tree(mid + 1, _end);
	return N;
}

set<int, greater<int>> pq[10][10][10][10][10];

void init()
{
	memset(nodes, 0, sizeof nodes);
	memset(trees, 0, sizeof trees);
	node_ptr = 0;
	start_point = 0;
	tree_ptr = 0;
	root = set_tree(0, 99999);

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 10; k++)
				for (int l = 0; l < 10; l++)
					for (int m = 0; m < 10; m++)
						pq[i][j][k][l][m].clear();
}

void set_blocks(int end) {

	int a, b, c, d, e;
	Node* E = &nodes[end + 1];
	Node* S = &nodes[end];
	for (int i = 0; i < 9; i++) {
		if(S->prv)
			S = S->prv; // 다없으면 null로 해줘야겟네
		else	break;
	}
	int arr[5];
	int stop = 0;


	while (1) {
		Node* N = S;

		for (int i = 0; i < 5; i++) {
			if (N == E) {
				stop = 1;
				break;
			}
			arr[i] = N->height;
			N = N->nxt;
		}

		if (stop)	 break;

		int min_height = *min_element(arr, arr + 5);

		for (int i = 0; i < 5; i++) {
			arr[i] -= min_height;
		}
		
		a = arr[0]; b = arr[1]; c = arr[2]; d = arr[3]; e = arr[4];
		pq[a][b][c][d][e].insert(S->id);
		S = S->nxt;
	}
}

void makeWall(int mHeights[5])
{
	for (int i = 0; i < 5; i++) {
		int id = node_ptr++;
		Node* N = &nodes[id];
		N->id = id;
		N->height = mHeights[i];
		N->nxt = &nodes[id + 1];
		if (id && N->prv == nullptr && id != start_point) { // 첫번째 블록은 미리 연결해두어야함 삭제될시
			N->prv = &nodes[id - 1];
		}
	}
	nodes[node_ptr].id = node_ptr;

	set_blocks(node_ptr- 1);
}

void set_piece(int mHeights[5]) {

	int min_height = *min_element(mHeights, mHeights + 5);

	for (int i = 0; i < 5; i++) {
		mHeights[i] -= min_height;
	}

	swap(mHeights[0], mHeights[4]);	swap(mHeights[1], mHeights[3]);
	int max_height = *max_element(mHeights, mHeights + 5);

	for (int i = 0; i < 5; i++) {
		mHeights[i] = max_height - mHeights[i];
	}
}

void erase_block(int id) {

	int a, b, c, d, e;
	Node* T = &nodes[id];
	Node* S = T;
	for (int i = 0; i < 4; i++) {
		if (S->prv)
			S = S->prv; // 다없으면 null로 해줘야겟네
		else	break;
	}

	Node* E = T;
	for (int i = 0; i < 9; i++) {
		if (E->id == node_ptr)	break;
		E = E->nxt; // 다없으면 null로 해줘야겟네
		if (E->id == node_ptr)	break;
	}
	int arr[5];
	int stop = 0;

	while (1) {
		Node* N = S;
		for (int i = 0; i < 5; i++) {
			if (N == E) {
				stop = 1;
				break;
			}
			
			arr[i] = N->height;
			N = N->nxt;
		}
		if (stop)	 break;
		int min_height = *min_element(arr, arr + 5);

		for (int i = 0; i < 5; i++) {
			arr[i] -= min_height;
		}
		a = arr[0]; b = arr[1]; c = arr[2]; d = arr[3]; e = arr[4];

		pq[a][b][c][d][e].erase(S->id);
		
		S = S->nxt;
	}
}

void after_erase(Node* P, Node* N) {
	int a, b, c, d, e;
	Node* S = P;
	for (int i = 0; i < 3; i++) {
		if (S->prv)
			S = S->prv; // 다없으면 null로 해줘야겟네
		else	break;
	}
	Node* E = N;
	for (int i = 0; i < 4; i++) {
		if (E->id == node_ptr)	break;
		E = E->nxt; // 다없으면 null로 해줘야겟네
		if (E->id == node_ptr)	break;
	}
	int arr[5];
	int stop = 0;
	while (1) {
		Node* N = S;
		for (int i = 0; i < 5; i++) {
			if (N == E) {
				stop = 1;
				break;
			}
			arr[i] = N->height;
			N = N->nxt;
		}
		if (stop)	 break;
		int min_height = *min_element(arr, arr + 5);

		for (int i = 0; i < 5; i++) {
			arr[i] -= min_height;
		}
		a = arr[0]; b = arr[1]; c = arr[2]; d = arr[3]; e = arr[4];
		pq[a][b][c][d][e].insert(S->id);
		S = S->nxt;
	}
}

int matchPiece(int mHeights[5])
{
	erase_cnt = 0;
	set_piece(mHeights);

	int a, b, c, d, e;
	a = mHeights[0]; b = mHeights[1]; c = mHeights[2]; d = mHeights[3]; e = mHeights[4];
	if (pq[a][b][c][d][e].empty()) {  return -1; }

	int id = *pq[a][b][c][d][e].begin();
	Node* N = &nodes[id];
	root->add(id, id);
	erase_block(N->id);
	
	Node* P = N->prv;

	for (int i = 0; i < 5; i++) {
		N->rm = 1;
		N = N->nxt;
	}
	
	N->prv = P;
	if (P)	P->nxt = N;
	else {
		start_point = N->id;  // 블록이 다사라짐 이거 다시보기
	}
	if (N->id != start_point)
		after_erase(P, N);

	if (id > 0)
		root->finds(0, id-1);
	return id - erase_cnt + 1;
}

static bool run()
{
	int N;
	int cmd;
	int heights[BAR_LEN];

	int ret = 0;
	int ans = 0;
	scanf("%d", &N);

	scanf("%d", &cmd);
	bool okay = false;
	if (cmd == CMD_INIT)
	{
		init();
		okay = true;
	}

	for (int turn = 0; turn < N - 1; turn++)
	{
		scanf("%d", &cmd);
		for (int i = 0; i < BAR_LEN; i++)
		{
			scanf("%d", &heights[i]);
		}

		switch (cmd)
		{
		case CMD_MAKEWALL:
			makeWall(heights);
			break;
		case CMD_MATCHPIECE:
			ret = matchPiece(heights);
			scanf("%d", &ans);
			if (ans != ret) {
				okay = false;
			}
			break;
		}
	}
	return okay;
}

int main()
{
	clock_t start, finish;
	double duration;
	start = clock();
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);
	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << duration << "초" << endl;

	return 0;
}