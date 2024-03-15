#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include <iostream>
#include <algorithm>
#include <memory.h>

using namespace std;

struct Result {
	int top;
	int count;
};

class Node {
public:
	int start;
	int end;
	int lazy; // 1이면 start, end크기 만큼 그리고 접근시 1을 밑에 내려줌
	int num;
	int top;
	int low;

	Node* lft;
	Node* rgt;

	void reset(int _start, int _end) {
		start = _start; end = _end; lazy = 0; num = 0; top = 0; low = 0;
		lft = nullptr; rgt = nullptr;
	}

	void drop(int _start, int _end, int h) {

		if (lazy && ( start != end)) {
			lft->top += lazy; lft->low += lazy;
			rgt->top += lazy; rgt->low += lazy;
			lft->lazy += lazy;
			rgt->lazy += lazy;
			lazy = 0;
		}

		if (_start == start && _end == end) {
			top += h;
			low += h;
			lazy = h;
			return;
		}

		int mid = (start + end) / 2;

		if (_end <= mid) {
			lft->drop(_start, _end, h);
		}
		else if (mid < _start) {
			rgt->drop(_start, _end, h);
		}
		else {
			lft->drop(_start, mid, h);
			rgt->drop(mid + 1, _end, h);
		}
		
		top = max(lft->top, rgt->top);
		low = min(lft->low, rgt->low);
	}
};

Node nodes[2000001];
Node* root;
int node_ptr;
long long cnt = 0;
int C;

Node* node_set(int _start, int _end) {
	Node* N = &nodes[node_ptr++];
	N->reset(_start, _end);
	if (_start == _end)	return N;

	int mid = (_start + _end) / 2;

	N->lft = node_set(_start, mid);
	N->rgt = node_set(mid + 1, _end);
	return N;
}

void init(int _C)
{
	cnt = 0;
	node_ptr = 1;
	C = _C;
	memset(nodes, 0, sizeof nodes);
	root = node_set(0, C - 1);
}

Result dropBlocks(int mCol, int mHeight, int mLength)
{
	cnt += mHeight * mLength;

	root->drop(mCol, mCol + mLength - 1, mHeight);

	long long answer = cnt - root->low * C;
	Result R;
	R.count = answer % 1000000; R.top = root->top - root->low;
	//cout << R.top << " " << R.count << endl;
	return R;
}

#define CMD_INIT 100
#define CMD_DROP 200

static bool run()
{
	int query_num;
	scanf("%d", &query_num);

	int ans_top, ans_count;
	bool ok = false;

	for (int q = 0; q < query_num; q++)
	{
		int query;
		scanf("%d", &query);
		if (query == CMD_INIT)
		{
			int C;
			scanf("%d", &C);
			init(C);
			ok = true;
		}
		else if (query == CMD_DROP)
		{
			int mCol, mHeight, mLength;
			scanf("%d %d %d", &mCol, &mHeight, &mLength);
			Result ret = dropBlocks(mCol, mHeight, mLength);
			scanf("%d %d", &ans_top, &ans_count);
			if (ans_top != ret.top || ans_count != ret.count)
			{
				ok = false;
			}
		}
		if (ok == false) {
			cout << query << endl;
			ok = true;
		}
	
	}
	return ok;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);
	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	return 0;
}