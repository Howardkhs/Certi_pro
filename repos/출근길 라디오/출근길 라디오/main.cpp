#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT 100
#define CMD_DESTROY 200
#define CMD_UPDATE 300
#define CMD_UPDATE_TYPE 400
#define CMD_CALC 500

#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int sum;

class Node {
public:
	int start;
	int end;
	int type;
	int time;

	Node* parent = nullptr;
	Node* lft = nullptr;
	Node* rgt = nullptr;

	Node(){}
	void reset() {
		start = 0; end = 0; type = 0; time = 0;
		lft = nullptr; rgt = nullptr;	parent = nullptr;
	}

	void update(int _start, int _end, int _time) {

		if (_start == start && _end == end) {
			time = _time;
			return;
		}

		int mid = (start + end) / 2;

		if (_end <= mid)	lft->update(_start, _end, _time);
		else				rgt->update(_start, _end, _time);

		time = lft->time + rgt->time;
	}

	void find(int _start, int _end) {
		if (_start == start && _end == end) {
			sum += time;
			return;
		}
		int mid = (start + end) / 2;

		if (_end <= mid) {
			lft->find(_start, _end);
		}
		else if (mid < _start) {
			rgt->find(_start, _end);
		}
		else {
			lft->find(_start, mid);
			rgt->find(mid + 1, _end);
		}
	}
};

int* times;
int* types;
int node_ptr;
vector<Node*> type_vec[1001];
Node nodes[201000];
Node* root;

Node* set(int _start, int _end) {

	Node* N = &nodes[node_ptr++];
	N->start = _start; N->end = _end;

	if (_start == _end) { 
		N->time = times[_start];
		N->type = types[_start];
		type_vec[N->type].push_back(N);
		return N; 
	}

	int mid = (_start + _end) / 2;

	N->lft = set(_start, mid);
	N->rgt = set(mid + 1, _end);
	N->lft->parent = N; N->rgt->parent = N;
	N->time = N->lft->time + N->rgt->time;
	return N;
}

void init(int N, int M, int mType[], int mTime[])
{	
	times = mTime;
	types = mType;
	for (int i = 0; i < 1001; i++) {
		type_vec[i].clear();
	}
	node_ptr = 1;
	root = set(0, N - 2);
	
}

void destroy()
{

}

void update(int mID, int mNewTime)
{
	root->update(mID, mID, mNewTime);
}

int updateByType(int mTypeID, int mRatio256)
{
	Node* now;
	int answer = 0;
	for (int i = 0; i < type_vec[mTypeID].size(); i++) {
		now = type_vec[mTypeID][i];
		now->time = floor(double(now->time) / double(256) * double(mRatio256));
		answer += now->time;

		while (now->parent) {
			now = now->parent;
			now->time = now->rgt->time + now->lft->time;
		}

	}
	return answer;
}

int calculate(int mA, int mB)
{
	sum = 0;

	if (mB < mA)	swap(mB, mA);
	root->find(mA, mB - 1);
	return sum;
}

#define MAX_N 100000

static int mType[MAX_N];
static int mTime[MAX_N];

static int run()
{
	int C;
	int isOK = 0;
	int cmd, ret, chk;

	int N, M;
	int mID, mTypeID, mNewTime, mRatio;
	int mA, mB;

	scanf("%d", &C);

	for (int c = 0; c < C; ++c)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d %d ", &N, &M);
			for (int i = 0; i < N - 1; i++) scanf("%d", &mType[i]);
			for (int i = 0; i < N - 1; i++) scanf("%d", &mTime[i]);
			init(N, M, mType, mTime);
			isOK = 1;
			break;

		case CMD_UPDATE:
			scanf("%d %d", &mID, &mNewTime);
			update(mID, mNewTime);
			break;

		case CMD_UPDATE_TYPE:
			scanf("%d %d", &mTypeID, &mRatio);
			ret = updateByType(mTypeID, mRatio);
			scanf("%d", &chk);
			if (ret != chk)
				isOK = 0;
			break;

		case CMD_CALC:
			scanf("%d %d", &mA, &mB);
			ret = calculate(mA, mB);
			scanf("%d", &chk);
			if (ret != chk)
				isOK = 0;
			break;

		default:
			isOK = 0;
			break;
		}
	}
	destroy();
	return isOK;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		if (run()) printf("#%d %d\n", tc, MARK);
		else printf("#%d %d\n", tc, 0);
	}
	return 0;
}