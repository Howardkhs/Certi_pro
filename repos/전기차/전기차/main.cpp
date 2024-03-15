#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <queue>
#include <memory.h>
#include <vector>
#include <iostream>

using namespace std;

#define MAX_N 350

int (*graph)[MAX_N];  // 0은 길 1은 장애물 2는 충전소
int gas_station[MAX_N][MAX_N];
int visited[MAX_N][MAX_N] = {};
int visited_gas[200];
int N;
int R;
int ptr;

typedef pair<int, int> P;

vector<P> dis[200]; // first -> 목적지 second 거리

class Node {
public:
	int id;
	P now;
	//P end;
	int fuel;
	int L; // 지나온 거리
	//Node* prv;
	Node* nxt;

	Node() {}
	Node(int _id, P _now, int _fuel, int _L) {
		id = _id;
		//end = _end;
		now = _now;
		fuel = _fuel;
		L = _L;
		nxt = nullptr;
	}
	void set(int _id, P _now, int _fuel, int _L) {
		id = _id;
		//end = _end;
		now = _now;
		fuel = _fuel;
		L = _L;
		nxt = nullptr;
	}

};

class Gas {
public:
	int now;
	int L;

	Gas() {}
	Gas(int _now, int _L) {
		now = _now;
		L = _L;
	}
	void set(int _now, int _L) {
		now = _now;
		L = _L;
	}
};

struct compare {
	bool operator()(Gas* A, Gas* B) {
		return A->L > B->L;
	}
};

Node nodes[122501];
int node_ptr;
Gas gases[2000];
int gas_ptr;

void init(int _N, int mRange, int mMap[MAX_N][MAX_N])
{
	N = _N;
	R = mRange;
	ptr = 0;
	node_ptr = 0;
	
	graph = mMap;

	memset(gas_station, 0, sizeof gas_station);
	for (int i = 0; i < 200; i++) {
		dis[i].clear();
	}

	return;
}

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0,-1, 1 };

void bfs(Node* A) {

	Node* front = A;
	Node* end = A;

	while (front != nullptr) {
		Node* B = front;

		int id = B->id;
		int x = B->now.first;
		int y = B->now.second;
		int r = B->fuel;
		int L = B->L;
		int nR = r - 1;
		int nL = L + 1;

		//q.pop_front();

		if (front == end) {
			front = nullptr;
			end = nullptr;

		}
		else {
			front = front->nxt;
		}
		B->nxt = nullptr;

		if (r == 0) continue;

		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];

			if (nx < 0 || ny < 0 || nx >= N || ny >= N || graph[nx][ny] == 1 || visited[nx][ny] == ptr)	continue;
			visited[nx][ny] = ptr;

			if (graph[nx][ny] == 2) {
				int gas_id = gas_station[nx][ny];
				dis[id].push_back({ gas_id, nL });
				dis[gas_id].push_back({ id, nL });
				continue;
			}
			//q.push_back({ id, {nx, ny}, nR, nL});
			Node* C = &nodes[node_ptr++];
			C->set(id, { nx, ny }, nR, nL);
			//Node* C = new Node(id, { nx, ny }, nR, nL);

			if (front == nullptr && end == nullptr) {
				front = C;
				end = C;
			}
			else {
				end->nxt = C;
				end = C;
			}
		}
	}
}

void add(int ID, int Row, int Col)
{
	//memset(visited, 0, sizeof visited);
	ptr++;
	node_ptr = 0;
	gas_station[Row][Col] = ID;
	graph[Row][Col] = 2;

	visited[Row][Col] = ptr;
	//Node* A = new Node(ID, { Row , Col }, R, 0);
	Node* A = &nodes[node_ptr++];
	A->set(ID, { Row , Col }, R, 0);
	bfs(A);
	return;
}

int distance(int mFrom, int mTo)
{
	memset(visited_gas, 0, sizeof visited_gas);
	visited_gas[mFrom] = 1;
	gas_ptr = 0;
	priority_queue < Gas*, vector<Gas*>, compare> pq;

	Gas* B = &gases[gas_ptr++];
	B->set(mFrom, 0);
	pq.push(B);

	while (!pq.empty()) {
		Gas* A = pq.top();
		pq.pop();

		int now = A->now;
		int L = A->L;
		if (now == mTo) {
			//cout << L << endl;
			return L;
		}

		for (int i = 0; i < dis[now].size(); i++) {

			int nnow = dis[now][i].first;
			int nL = L + dis[now][i].second;

			if (visited_gas[nnow] != 0 && visited_gas[nnow] < nL)	continue;
			visited_gas[nnow] = nL;
			Gas* B = &gases[gas_ptr++];
			B->set(nnow, nL);
			pq.push(B);

		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////

#define INIT		0
#define ADD			1
#define DISTANCE	2

static bool run()
{
	int cmd, ans, ret;
	int N, range, id, id2, r, c;
	int Q = 0;
	bool okay = false;
	int region[MAX_N][MAX_N];

	scanf("%d", &Q);

	for (int q = 0; q < Q; ++q)
	{
		scanf("%d", &cmd);

		switch (cmd)
		{
		case INIT:
			scanf("%d %d", &N, &range);

			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					scanf("%d", &region[i][j]);

			init(N, range, region);
			okay = true;
			break;

		case ADD:
			scanf("%d %d %d", &id, &r, &c);
			add(id, r, c);
			break;

		case DISTANCE:
			scanf("%d %d", &id, &id2);
			ret = distance(id, id2);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;

		default:
			okay = false;
		}
	}

	return okay;
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