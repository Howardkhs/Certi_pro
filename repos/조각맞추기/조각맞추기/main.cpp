#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define BAR_LEN 5

#define CMD_INIT 100
#define CMD_MAKEWALL 200
#define CMD_MATCHPIECE 300

#include<iostream>
#include<set>
#include<vector>
#include<bitset>
#include<algorithm>
#include <ctime>
using namespace std;

class Node {
public:
	int h;
	int idx;
	int nxt;
	int prv;

	Node(){
	}
	Node(int _h, int _nxt) {
		h = _h;
		nxt = _nxt;
	}
	void set(int _h, int _idx,int _nxt, int _prv) {
		h = _h;
		idx = _idx;
		nxt = _nxt;
		prv = _prv;
	}
};

int dels;

int tree_arr[150001];

class Tree {
public:
	int start;
	int end = 0;
	int idx;
	
	Tree* lft;
	Tree* rgt;

	Tree(){}
	Tree(int _start, int _end, int _idx) {
		start = _start;
		end = _end;
		idx = _idx;
		//num = _num;
	}
	void set(int _start, int _end, int _idx) {
		start = _start;
		end = _end;
		idx = _idx;
		//num = _num;
	}

	void add(int _start, int _end, int _num) {
		if (start == _start && end == _end) {
			tree_arr[idx] += _num;
			return;
		}
		if (_end <= (start + end) / 2) {
			lft->add(_start, _end, _num);
		}
		else {
			rgt->add(_start, _end, _num);
		}
		tree_arr[idx] = tree_arr[lft->idx] + tree_arr[rgt->idx];
	}
	
	void find(int _start, int _end) {

		if (_start > _end)	return;

		if (start == _start && end == _end) {
			dels += tree_arr[idx];
			return;
		}
		if (_end <= (start + end) / 2) {
			lft->find(_start, _end);
		}
		else if ((start + end) / 2 < _start) {
			rgt->find(_start, _end);
		}
		else {
			lft->find(_start, (start + end) / 2);
			rgt->find((start + end) / 2 + 1, _end);
		}
	}
};

int node_ptr;
int end_ptr;
int tree_ptr;
int start_ptr;
int prv_dirty;
int tree_set = 0;

Node nodes[100001];
set<int> map[1050000];
Tree trees[150001];

vector<int> names_base;
vector<int> idxs;
Tree* root;

void treeSet(Tree* N, int _start, int _end, int idx) {

	N->set(_start, _end, idx);
	//node_ptr++;
	//cout << _start << " " << _end << " " << endl;
	if (_start  == _end) {
		return;
	}

	N->lft = &trees[node_ptr];
	treeSet(N->lft, _start, (_start + _end) / 2, node_ptr++);
	N->rgt = &trees[node_ptr];
	treeSet(N->rgt, (_start + _end) / 2 + 1, _end, node_ptr++);
	
	
}

void init()
{
	start_ptr = 0;
	node_ptr = 0;
	end_ptr = 0;
	tree_ptr = 0;
	for (int i = 0; i < 1050000; i++) {
		map[i].clear();
	}
	if (!tree_set) {
		root = &trees[node_ptr];
		treeSet(root, 0,74999, node_ptr++);
		tree_set = 1;
		
	}/*
	else {
		for (int i = 0; i < 150001; i++) {
			trees[i].num = 0;
		}
	}
	*/
	memset(tree_arr, 0, sizeof tree_arr);
}

int get_start(int pivot) {

	int cnt = 0;

	while (1) {
		if (nodes[pivot].prv == -1)	return pivot;
		pivot = nodes[pivot].prv;
		cnt++;
		if (cnt == 4) return pivot;
	}
}

int get_end(int pivot) {  // end는 끝나는 지점 + 1

	int cnt = 0;

	while (1) {
		pivot = nodes[pivot].nxt;
		if (pivot == end_ptr)	return end_ptr;
		cnt++;
		if (cnt == 9) return pivot;
	}
}

void get_name_base(int start, int end) {

	names_base.clear();
	idxs.clear();
	int pivot = start;
	while (1) {
		if (pivot == end)	return;
		names_base.push_back(nodes[pivot].h);
		idxs.push_back(nodes[pivot].idx);
		pivot = nodes[pivot].nxt;
	}
}

void make_name() {
	//cout << "make " << endl;
	for (int i = 0; i < names_base.size() - 4; i++) {
		int name = 0;
		int max_h = *max_element(names_base.begin() + i, names_base.begin() + i + 5);
		for (int j = i; j < i + 5; j++) {
			name = name << 4;
			name += max_h - names_base[j];
		}
		//cout << bitset<32>(name)<< endl;
		map[name].insert(idxs[i]);
	}
	//cout << endl;
}

void erase_name() {
	//cout << "erase  " << endl;
	for (int i = 0; i < names_base.size() - 4; i++) {
		int name = 0;
		int max_h = *max_element(names_base.begin() + i, names_base.begin() + i + 5);
		for (int j = i; j < i + 5; j++) {
			name = name << 4;
			name += max_h - names_base[j];
		}
		//cout << bitset<32>(name) << endl;
		map[name].erase(idxs[i]);
	}
	//cout << endl;
}

void erase_idx(int idx) {

	int end = idx;
	int cnt = 0;
	int start = nodes[idx].prv;

	while (1) {
		end = nodes[end].nxt;
		if (end == end_ptr)	break;
		cnt++;
		if (cnt == 5) break;
	}

	if (start == -1) {
		//nodes[nodes[end].prv].nxt = -1;
		nodes[end].prv = start;
		start_ptr = end;
	}
	else {
		nodes[start].nxt = end;
		nodes[end].prv = start;
		
		if (end == end_ptr) prv_dirty = 1;
	}
}

void makeWall(int mHeights[5])
{
	for (int i = 0; i < 5; i++) {
		if (!prv_dirty) {
			nodes[i + end_ptr].set(mHeights[i], i + end_ptr, i + end_ptr + 1, i + end_ptr - 1);
		}
		else {
			int temp = nodes[i + end_ptr].prv;
			nodes[i + end_ptr].set(mHeights[i], i + end_ptr, i + end_ptr + 1, i + end_ptr - 1);
			nodes[i + end_ptr].prv = temp;
			prv_dirty = 0;
		}
		if (i + end_ptr == start_ptr) {
			nodes[i + end_ptr].prv = -1;
		}
	}

	int start = get_start(end_ptr);

	end_ptr += 5;
	get_name_base(start, end_ptr);
	make_name();
}

int matchPiece(int mHeights[5])
{
	dels = 0;
	int answer;
	int min_h = *min_element(mHeights, mHeights + 5);

	for (int i = 0; i < 5; i++) {
		mHeights[i] -= min_h;
	}

	int name = 0;
	for (int i = 4; i >= 0; i--) {
		name = name << 4;
		name += mHeights[i];
	}
	//cout << bitset<32>(name) << endl;
	if (map[name].empty()) return -1;

	int idx = *map[name].rbegin();

	int start = get_start(idx);
	int end = get_end(idx);

	get_name_base(start, end);
	erase_name();
	erase_idx(idx);
	root->add(idx, idx, 5);
	root->find(0, idx - 1);
	answer = idx - dels + 1;

	//cout << answer << endl;

	if (nodes[idx].prv == -1)	return answer;
	get_name_base(start, end);
	make_name();

	return answer;
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
				//cout << ans << " " << ret << endl;
			}
			break;
		}
		//print();
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