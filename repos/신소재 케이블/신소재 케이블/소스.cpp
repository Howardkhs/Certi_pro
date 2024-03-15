#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT			(100)
#define CMD_CONNECT			(200)
#define CMD_MEASURE			(300)
#define CMD_TEST			(400)

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory.h>
#include <algorithm>

using namespace std;

class Node {
public:
	int id;
	int max_dis = 0;
	int lat = 0;
	Node* parent;
	vector<Node*> childs;
	Node(){}
	void reset() {
		max_dis = 0; lat = 0; parent = nullptr; childs.clear();
	}
};

struct P {
	int nxt;
	int lat;
	P(){}
	P(int _nxt, int _lat) {
		nxt = _nxt; lat = _lat;
	}
};

Node nodes[10001];
vector<P> roads[10001];
vector<P> childs[10001];
unordered_map<int, int> id_map;
int node_ptr;
Node* root;

int visited[10001];

void init(int mDevice)
{
	for (int i = 0; i < 10001; i++) {
		nodes[i].reset();
		nodes[i].id = i;
		roads[i].clear();
		childs[i].clear();
	}
	id_map.clear();
	node_ptr = 1;

	int id = node_ptr;
	id_map[mDevice] = node_ptr++;
	root = &nodes[id];
	return;
}

void connect(int mOldDevice, int mNewDevice, int mLatency)
{
	int oId = id_map[mOldDevice];
	int nId = node_ptr;
	id_map[mNewDevice] = node_ptr++;

	roads[oId].push_back({ nId, mLatency });
	roads[nId].push_back({ oId, mLatency });
	childs[oId].push_back({ nId, mLatency });
	
	Node* O = &nodes[oId];	Node* N = &nodes[nId];

	N->lat = mLatency;
	N->parent = O;

	int lat = mLatency;

	Node* P = O;

	while (N->parent) {
		N = N->parent;
		if (N->max_dis < lat) {
			N->max_dis = lat;
			lat += N->lat;
			continue;
		}
		break;
	}

	return;
}

int finds = 0;
int answer;

void dfs(int s, int e, int cost) {

	if (finds)	return;
	visited[s] = 1;

	if (s == e) {
		finds = 1;
		answer = cost;
		return;
	}

	for (int i = 0; i < roads[s].size(); i++) {
		if (visited[roads[s][i].nxt]) continue;
		dfs(roads[s][i].nxt, e, cost + roads[s][i].lat);
	}
}

int measure(int mDevice1, int mDevice2)
{
	finds = 0;
	memset(visited, 0, sizeof visited);

	int s = id_map[mDevice1];
	int e = id_map[mDevice2];

	dfs(s, e, 0);
//	cout << answer << endl;

	return answer;
}
vector<int> temp;

void find(Node* N) {

	int lat = N->max_dis;

	while (N->parent) {
		temp.clear();
		Node* P = N->parent;
		int Id = P->id;
		lat = lat + N->lat;

		//cout << Id << " " << lat << " ";

		for (int i = 0; i < childs[Id].size(); i++) {
			int nId = childs[Id][i].nxt;
			if (nId == N->id)	continue;
			temp.push_back(nodes[nId].max_dis + nodes[nId].lat);
		}

		sort(temp.begin(), temp.end(), greater<int>());

		if (temp.empty()) {
			answer = max(answer, lat);
			N = N->parent;
			P = P->parent;
			continue;
		}
		answer = max(answer, lat + temp[0]);
		//cout << answer << endl;
		N = N->parent;
		P = P->parent;
	}
}


int test(int mDevice)
{
	int Id = id_map[mDevice];
	answer = 0;
	temp.clear();


	if (Id == 1) {
		for (int i = 0; i < childs[1].size(); i++) {
			int nId = roads[1][i].nxt;
			temp.push_back(nodes[nId].max_dis + nodes[nId].lat);
		}

		sort(temp.begin(), temp.end(), greater<int>());

		if (temp.size() >= 2) {
			answer = temp[0] + temp[1];
		}
		else {
			answer = temp[0];
		}
		//cout << answer << endl;
		return answer;
	}

	Node* N = &nodes[Id];
	
	for (int i = 0; i < childs[Id].size(); i++) {
		int nId = childs[Id][i].nxt;
		temp.push_back(nodes[nId].max_dis + nodes[nId].lat);
	}
	sort(temp.begin(), temp.end(), greater<int>());

	if (temp.size() >= 2) {
		answer = temp[0] + temp[1];
	}
	else if (temp.size() == 1){
		answer = temp[0];
	}

	find(N);

	
	return answer;
}

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
		}
		if (okay == false) {
			cout << cmd << endl;
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