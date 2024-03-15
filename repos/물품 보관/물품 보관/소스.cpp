#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT			(100)
#define CMD_STOCK			(200)
#define CMD_SHIP			(300)
#define CMD_GET_HEIGHT		(400)

#include <iostream>
#include <memory.h>
#include <deque>
#include <algorithm>

using namespace std;


struct P {
	int height;
	int pos;
	P(){}
	P(int _height, int _pos) {
		height = _height; pos = _pos;
	}
};

deque<P> dq;
P find_pos;


class Node {
public:
	int start;
	int end;
	int	height;
	P hp;
	Node* parent;
	Node* lft; Node* rgt;
	Node(){}
	
	void ship(int _start, int _end, int _height) {
		if (start == _start && end == _end) {
			height += _height;
			hp = { height, _start };
			return;
		}

		int mid = (start + end) / 2;

		if (_end <= mid) {
			lft->ship(_start, _end, _height);
		}
		else {
			rgt->ship(_start, _end, _height);
		}
		
		int lft_height = lft->hp.height;
		int rgt_height = rgt->hp.height;

		if (rgt_height > lft_height) {
			hp = rgt->hp;
		}
		else {
			hp = lft->hp;
		}
	}

	void find_height(int _start, int _end) {
		if (start == _start && end == _end) {
			if (find_pos.height < hp.height) {
				find_pos = hp;
			}
			return;
		}
		int mid = (start + end) / 2;
		if (_end <= mid) {
			lft->find_height(_start, _end);
		}
		else if (mid < _start) {
			rgt->find_height(_start, _end);
		}
		else {
			lft->find_height(_start, mid);
			rgt->find_height(mid + 1, _end);
		}
	}
};

Node nodes[200001];
int  boxes[100000];

int node_ptr;
int N;
Node* root;

Node* node_set(int _start, int _end) {

	Node* N = &nodes[node_ptr++];
	N->start = _start; N->end = _end;
	if (_start == _end)		return N;

	int mid = (_start + _end) / 2;
	N->lft = node_set(_start, mid);
	N->rgt = node_set(mid + 1, _end);
	N->lft->parent = N; N->rgt->parent = N;
	return N;
}
void init(int _N)
{
	memset(nodes, 0, sizeof nodes);
	memset(boxes, 0, sizeof boxes);
	node_ptr = 1;
	N = _N;

	root = node_set(0, N - 1);
}

void find_lft(int idx) {

	if (idx < 0)	return;
	
	find_pos = { 0,0 };
	root->find_height(0, idx);
	if (find_pos.height == 0)	return;
	dq.push_front(find_pos);
	idx = find_pos.pos;

	find_lft(idx - 1);
}

void find_rgt(int idx) {
	if (idx >= N)	return;
	find_pos = { 0,0 };
	root->find_height(idx, N-1);
	if (find_pos.height == 0)	return;
	dq.push_back(find_pos);
	idx = find_pos.pos;

	find_rgt(idx + 1);
}

int make_dq() {

	find_pos = { 0,0 };
	int answer = 0;

	root->find_height(0, N - 1);
	dq.push_back(find_pos);
	int idx = find_pos.pos;
	answer += find_pos.height;

	find_lft(idx-1);
	find_rgt(idx+1);

	int x = dq[0].pos;
	int h = dq[0].height;

	for (int i = 1; i < dq.size(); i++) {
		int nx = dq[i].pos; int nh = dq[i].height;
		if (nh >= h) {
			answer += (nx - x) * h;
		}
		else {
			answer += (nx - x) * nh;
		}
		x = nx; h = nh;
	}
	return answer;
}

int stock(int mLoc, int mBox)
{
	mLoc--;
	boxes[mLoc] += mBox;
	root->ship(mLoc, mLoc, mBox);
	
	dq.clear();
	int answer = make_dq();
	return answer;
}

int ship(int mLoc, int mBox)
{
	mLoc--;
	boxes[mLoc] -= mBox;
	root->ship(mLoc, mLoc, -mBox);

	dq.clear();
	int answer = make_dq();
	return answer;
}

int getHeight(int mLoc)
{
	mLoc--;
	return boxes[mLoc];
}


static bool run()
{
	int Q;
	int N, mLoc, mBox;

	int ret = -1, ans;

	scanf("%d", &Q);

	bool okay = false;

	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &N);
			init(N);
			okay = true;
			break;
		case CMD_STOCK:
			scanf("%d %d", &mLoc, &mBox);
			ret = stock(mLoc, mBox);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_SHIP:
			scanf("%d %d", &mLoc, &mBox);
			ret = ship(mLoc, mBox);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_GET_HEIGHT:
			scanf("%d", &mLoc);
			ret = getHeight(mLoc);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
	}

	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int TC, MARK;

	scanf("%d %d", &TC, &MARK);
	for (int tc = 1; tc <= TC; ++tc)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}