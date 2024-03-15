#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT			(100)
#define CMD_STOCK			(200)
#define CMD_SHIP			(300)
#define CMD_GET_HEIGHT		(400)

#include <set>
#include <iostream>
#include <memory.h>
#include <deque>
#include <vector>

using namespace std;
int heights[100001];
struct P {
	int idx;
	int h;
	P(){}
	P(int _idx, int _h){
		idx = _idx;
		h = _h;
	}
};

P T;

P compare(P A, P B) {

	if (A.h > B.h)
		return A;
	else
		return B;
}

class Node {
public:
	int start;
	int end;
	P id_h;

	Node* lft;	Node* rgt;

	Node(){}
	void set(int _start, int _end) {
		start = _start;
		end = _end;
		id_h = { 0,0 }; //?
	}

	void stock(int _start, int _end, int h) {
		if (start == _start && end == _end) {
			int _h = id_h.h;
			id_h = { start, _h + h };
			heights[start] = _h + h;
			return;
		}

		int mid = (start + end) / 2;
		if (_start <= mid) {
			lft->stock(_start, _end, h);
		}
		else {
			rgt->stock(_start, _end, h);
		}
		id_h = compare(lft->id_h, rgt->id_h);
	}

	void ship(int _start, int _end, int h) {
	
		if (start == _start && end == _end) {
			int _h = id_h.h;
			id_h = { start, _h - h };
			heights[start] = _h - h;
			return;
		}

		int mid = (start + end) / 2;
		if (_start <= mid) {
			lft->ship(_start, _end, h);
		}
		else {
			rgt->ship(_start, _end, h);
		}
		id_h = compare(lft->id_h, rgt->id_h);
	}

	P find(int _start, int _end) {
		//
		if (start == _start && end == _end) {
			return id_h;
		}

		int mid = (start + end) / 2;
		if (_end <= mid) {
			T = compare(T, lft->find(_start, _end));
			return T;
		}
		else if(mid < _start){
			T = compare(T, rgt->find(_start, _end));
			return T;
		}
		else {
			T = compare(T, lft->find(_start, mid));
			T = compare(T, rgt->find(mid + 1, _end));
			return T;
		}
	}
};

Node nodes[200020];

int node_ptr;
Node* root = &nodes[0];
vector<P> dq;
vector<P> temp_dq;
int N, answer;

Node* set_nodes(int start, int end) {
	Node* now = &nodes[node_ptr++];
	now->set(start, end);
	//cout << start << " " << end << endl;
	if (start == end)	return now;

	now->lft = set_nodes(start, (start + end) / 2);
	now->rgt = set_nodes((start + end) / 2 + 1, end);
	return now;
}

void init(int _N)
{	
	N = _N;
	node_ptr = 0;
	memset(heights, 0, sizeof heights);
	set_nodes(0, N+1);
	return;
}

void print_dq() {
	for (int i = 0; i < dq.size(); i++) {
		cout << dq[i].h << " ";
	}
	cout << endl;
}

void get_lft(int idx) {
	
	T.idx = 0; T.h = 0;
	root->find(0, idx);

	if (T.h != 0) {
		temp_dq.push_back({ T.idx, T.h });
		get_lft(T.idx - 1);
	}
}

void get_rgt(int idx) {

	T.idx = 0; T.h = 0;
	root->find(idx, N+1);
	if (T.h != 0) {
		dq.push_back({ T.idx, T.h });
		get_rgt(T.idx + 1);
	}
}

void sum_dq() {
	if (dq.empty())	return;
	if (dq.size() == 1) {
		answer += dq[0].h;
		return;
	}
	int mid = root->id_h.idx;
	int idx = dq[0].idx; int h = dq[0].h;
	int nidx, nh;
	bool forward = true;

	if (idx == mid) {
		forward = false;
		answer += h;
	}

	for (int i = 1; i < dq.size(); i++) {
		nidx = dq[i].idx; nh = dq[i].h;
		//cout << nidx << " " << nh << endl;
		if (forward)
			answer += (nidx - idx) * h;
		//cout << answer << " " << endl;
		if (nidx == mid) {
			forward = false;
			answer += nh;
			idx = nidx; h = nh;
			continue;
		}
		if (!forward)
			answer += (nidx - idx) * nh;
		idx = nidx; h = nh;
	}

}

void cal(int mid) {
	get_lft(mid - 1);
	for (auto p = temp_dq.rbegin(); p != temp_dq.rend(); p++) {
		dq.push_back({ p->idx, p->h });
	}
	dq.push_back({ root->id_h.idx ,  root->id_h.h });
	get_rgt(mid + 1);
	//print_dq();
	sum_dq();
}

int stock(int mLoc, int mBox)
{	
	temp_dq.clear(); dq.clear(); answer = 0;

	root->stock(mLoc, mLoc, mBox);

	int mid = root->id_h.idx;
	
	cal(mid);
	//cout << "answer : " << answer << endl;
	return answer;
}

int ship(int mLoc, int mBox)
{
	temp_dq.clear(); dq.clear(); answer = 0;

	root->ship(mLoc, mLoc, mBox);

	int mid = root->id_h.idx;
	if (root->id_h.h == 0) {
		//cout << "answer : " << answer << endl;
		return answer;
	}
	cal(mid);
	//cout << "answer : " << answer << endl;
	return answer	;
}

int getHeight(int mLoc)
{
	//cout << heights[mLoc] << endl;
	return heights[mLoc];
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
		if (okay == false) {
			cout << "cmd : " << cmd << endl;
			okay = true;
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