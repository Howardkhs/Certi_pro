#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT			(100)
#define CMD_CONNECT			(200)
#define CMD_MEASURE			(300)
#define CMD_TEST			(400)

/// /////////////////////////////////////////////////////
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <unordered_set>

using namespace std;

int answer = 0;
int root = 0;
int c = 0;

struct Node {
	int start = 0;
	int now = 0;
	int dis = 0;
	Node* prv;
	Node* nxt;

	Node() {}
	Node(int _start, int _now, int _dis) {
		start = _start;
		now = _now;
		dis = _dis;
	}
};

class Equip {
	public:
		int num;
		int latency = 0;
		Equip* parent = nullptr;
		unordered_map<int, Equip*> childs;
		unordered_map<Equip*, int> dis; // 이쪽 방향으로 갔을때 최대 거리
		unordered_map<Equip*, int> dis_cnt;
		unordered_set<int> Setnum;

		Equip(){}
		Equip(int _num, int _latency) {
			num = _num;
			latency = _latency;
			parent = nullptr;
		}

		void set_parent(int mNewDevice, int _latency) {
			parent->Setnum.insert(mNewDevice);
			parent->childs[mNewDevice] = this;
			if (latency + _latency > parent->dis[this])
				parent->dis[this] = latency + _latency;
			if (latency + _latency == parent->dis[this])
				parent->dis_cnt[this] += 1;
			if (parent->parent) {
				parent->set_parent(mNewDevice, latency + _latency);
			}
		}
		
		void print() {
			cout << num << " ";
			/*
			for (auto p = childs.begin(); p != childs.end(); p++) {
				p->second->print();
			}*/
			for (auto p = Setnum.begin(); p != Setnum.end(); p++) {
				cout << *p << " ";
			}
		}

		void find(int mDevice) {
			auto it = Setnum.find(mDevice);

			if (it == Setnum.end()) { // 못찾을시
				answer += latency;
				if (parent->num == mDevice) return;
				parent->find(mDevice);
			}
			else {
				
				//cout << answer << endl;
				Equip* C = childs[mDevice];
				answer += C->latency;
				if (C->num == mDevice) {
					return;
				}

				C->find(mDevice);
			}
		}

		Equip* find_root(int mDevice) {
		
			answer += latency;
			int max_lat = 0;
			for (auto p = parent->dis.begin(); p != parent->dis.end(); p++) {
				if (p->first == this)
					continue;
				max_lat = max(max_lat, p->second);
			}
			c = max(c, answer + max_lat);
		
			if (parent->num == mDevice) {

				return this;
			}
			parent->find_root(mDevice);
		}
			
};

struct P {
	int nxt;
	int dis;

	P() {}
	P(int _nxt, int _dis) {
		nxt = _nxt;
		dis = _dis;
	}
};// 다음 좌표, 거리
unordered_map<int, vector<P>> graph;
unordered_map<int, int> visited;
unordered_map <int, Equip> equips;


void init(int mDevice)
{	
	graph.clear();
	root = mDevice;
	equips[mDevice] = Equip(mDevice, 0);
	return;
}

void print() {
	Equip* Root = &equips[root];
	Root->print();

}

void connect(int mOldDevice, int mNewDevice, int mLatency)
{
	P a(mOldDevice, mLatency);
	graph[mNewDevice].push_back(a);

	P b(mNewDevice, mLatency);
	graph[mOldDevice].push_back(b);

	equips[mNewDevice] = Equip(mNewDevice, mLatency);

	Equip* P = &equips[mOldDevice];
	equips[mNewDevice].parent = P;
	equips[mNewDevice].set_parent(mNewDevice, 0);
	return;
}

int measure(int mDevice1, int mDevice2)
{
	answer = 0;
	Equip* A = &equips[mDevice1];

	A->find(mDevice2);

	return answer;
}

int test(int mDevice)
{	
	int a = 0, b = 0, d = 0;
	c = 0;
	Equip* Root = &equips[root];
	if (mDevice == root) {
		for (auto p = Root->dis.begin(); p != Root->dis.end(); p++) {
			b = max(b, p->second);
			if (b > a) {
			
				int temp = b;
				b = a;
				a = temp;
			}
		}

		return b + a;
	}

	Equip* T = &equips[mDevice];
	
	answer = 0;
	for (auto p = T->dis.begin(); p != T->dis.end(); p++) {
		b = max(b, p->second);
		if (b > a) {
			
			int temp = b;
			b = a;
			a = temp;
		}

	}

	Equip* check = T->find_root(root);
	
	/* for (auto p = Root->dis.begin(); p != Root->dis.end(); p++) {
		if (p->first == check)
			continue;
		d = max(d, p->second);
	}*/

	return max(a + b, a + c);

}

/// /////////////////////////////////////////////////////

static bool run()
{
	int Q;
	int mDevice, mOldDevice, mNewDevice, mDevice1, mDevice2;
	int mLatency;

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
			scanf("%d", &mDevice);
			init(mDevice);
			okay = true;
			break;
		case CMD_CONNECT:
			scanf("%d %d %d", &mOldDevice, &mNewDevice, &mLatency);
			connect(mOldDevice, mNewDevice, mLatency);
			break;
		case CMD_MEASURE:
			scanf("%d %d", &mDevice1, &mDevice2);
			ret = measure(mDevice1, mDevice2);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_TEST:
			scanf("%d", &mDevice);
			ret = test(mDevice);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
		if (okay == false) {
			cout << cmd << endl;
			cout << root <<" " <<  mDevice << endl;
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