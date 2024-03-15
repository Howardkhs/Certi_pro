#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <queue>
#include <vector>
#include <memory.h>
#include <unordered_map>
#include <iostream>
using namespace std;

struct P {
	int to; 
	int dis;
	P(int _to, int _dis) {
		to = _to;	dis = _dis;
	}
};

struct N {
	int now;
	int dis;
	
	N(int _now, int _dis) {
		now = _now;	dis = _dis;
	}
};

struct compare {
	bool operator()(N& A, N& B) {
		return A.dis > B.dis;
	}
};


vector<P> roads[601];
vector<P> backs[601];
unordered_map<int, int> id_map;

int visited1[601];
int visited2[601];
int ptr = 1;

int init(int N, int sCity[], int eCity[], int mCost[]) {

	for (int i = 0; i < 601; i++) {
		roads[i].clear(); backs[i].clear();
	}
	id_map.clear();
	memset(visited1, 1, sizeof visited1);
	memset(visited2, 1, sizeof visited2);
	ptr = 1;


	for (int i = 0; i < N; i++) {
		int a, b;
		a = sCity[i]; b = eCity[i];

		int s = id_map[a];
		if (s == 0) {
			s = ptr;
			id_map[a] = ptr++;
		}

		int e = id_map[b];
		if (e == 0) {
			e = ptr;
			id_map[b] = ptr++;
		}

		roads[s].push_back({ e, mCost[i] });
		backs[e].push_back({ s, mCost[i] });
	}
	return ptr - 1;
}

void add(int sCity, int eCity, int mCost) {

	int a, b;
	a = sCity; b = eCity;

	int s = id_map[a];
	if (s == 0) {
		s = ptr;
		id_map[a] = ptr++;
	}

	int e = id_map[b];
	if (e == 0) {
		e = ptr;
		id_map[b] = ptr++;
	}
	roads[s].push_back({ e, mCost });
	backs[e].push_back({ s, mCost });

	return;
}

int cost(int mHub) {
	memset(visited1, 1, sizeof visited1);
	memset(visited2, 1, sizeof visited2);

	priority_queue<N, vector<N>, compare> pq;

	int now = id_map[mHub];
	pq.push({ now, 0 });
	visited1[now] = 0;

	while (!pq.empty()) {
		N T = pq.top(); pq.pop();
		int now = T.now; int dis = T.dis;

		for (int i = 0; i < roads[now].size(); i++) {
			int nnow = roads[now][i].to; int ndis = roads[now][i].dis + dis;
			if (visited1[nnow] <= ndis)	continue;
			visited1[nnow] = ndis;
			pq.push({ nnow, ndis });
		}
	}

	pq = priority_queue<N, vector<N>, compare>();

	now = id_map[mHub];
	pq.push({ now, 0 });
	visited2[now] = 0;

	while (!pq.empty()) {
		N T = pq.top(); pq.pop();
		int now = T.now; int dis = T.dis;

		for (int i = 0; i < backs[now].size(); i++) {
			int nnow = backs[now][i].to; int ndis = backs[now][i].dis + dis;
			if (visited2[nnow] <= ndis)	continue;
			visited2[nnow] = ndis;
			pq.push({ nnow, ndis });
		}
	}

	int answer = 0;

	for (int i = 1; i < ptr; i++) {
		answer += visited1[i] + visited2[i];
	}

	return answer;
}

/////////////////////////////////////////////////////////////////////////

#define MAX_N 1400
#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_COST 3

static bool run() {
	int q;
	scanf("%d", &q);

	int n;
	int sCityArr[MAX_N], eCityArr[MAX_N], mCostArr[MAX_N];
	int sCity, eCity, mCost, mHub;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			okay = true;
			scanf("%d", &n);
			for (int j = 0; j < n; ++j) {
				scanf("%d %d %d", &sCityArr[j], &eCityArr[j], &mCostArr[j]);
			}
			scanf("%d", &ans);
			ret = init(n, sCityArr, eCityArr, mCostArr);
			if (ans != ret)
				okay = false;
			break;
		case CMD_ADD:
			scanf("%d %d %d", &sCity, &eCity, &mCost);
			add(sCity, eCity, mCost);
			break;
		case CMD_COST:
			scanf("%d %d", &mHub, &ans);
			ret = cost(mHub);
			if (ans != ret)
				okay = false;
			break;
		default:
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