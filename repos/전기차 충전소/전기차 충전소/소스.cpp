#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define MAX_N 350
#include <iostream>
#include <memory.h>
#include <vector>
#include <deque>
#include <queue>
#include <ctime>

using namespace std;

int (*graph)[MAX_N];
int fuel_map[MAX_N][MAX_N];
int visited[MAX_N][MAX_N];
int station_dis[MAX_N];
int N, R;

struct P {
	int nxt;
	int dis;
	P(int _nxt, int _dis) {
		nxt = _nxt; dis = _dis;
	}
};

struct T {
	int x;
	int y;
	int mv;
	T(int _x, int _y, int _mv) {
		x = _x; y = _y; mv = _mv;
	}
	T(){}
};

struct M {
	int now;
	int fuel;
	M(int _now, int _fuel) {
		now = _now; fuel = _fuel;
	}
};

struct compare {
	bool operator()(M& A, M& B) {
		return A.fuel > B.fuel;
	}
};


T Pairs[150000];

vector<P> gas_station[200];
pair<int, int> gas_pos[200];
priority_queue<M, vector<M>, compare> pq;

void init(int _N, int mRange, int mMap[MAX_N][MAX_N])
{
	graph = mMap;
	N = _N;
	R = mRange;

	for (int i = 0; i < 200; i++) {
		gas_station[i].clear();
	}
	
	memset(fuel_map, 1, sizeof fuel_map);
	return;
}

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = {  0, 0,-1, 1 };

void bfs(int x, int y, int mv, int ID) {

	int front = 0;
	int back = 1;

	Pairs[0] = { x, y, mv };
	visited[x][y] = 1;

	while (front < back) {
		T a = Pairs[front++];
		int x = a.x; int y = a.y; int mv = a.mv;
		
		if (fuel_map[x][y] < 200 && fuel_map[x][y] != ID) {
			gas_station[ID].push_back({ fuel_map[x][y], mv });
			gas_station[fuel_map[x][y]].push_back({ ID, mv });
		}

		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (nx < 0 || ny < 0 || nx >= N || ny >= N)	continue;
			if (visited[nx][ny] || graph[nx][ny]) continue;
			visited[nx][ny] = 1;
			if (mv == R)	continue;
			Pairs[back++] = { nx, ny, mv + 1 };
		}
	}
}

void add(int ID, int Row, int Col)
{
	fuel_map[Row][Col] = ID;
	gas_pos[ID] = { Row, Col };
	memset(visited, 0, sizeof visited);
	bfs(Row, Col, 0, ID);

	return;
}

int distance(int mFrom, int mTo)
{
	memset(station_dis, 1, sizeof station_dis);
	
	pq.push({ mFrom, 0 });
	station_dis[mFrom] = 0;
	while (!pq.empty()) {
		M N = pq.top(); pq.pop();
		int now = N.now; int fuel = N.fuel;
		for (int i = 0; i < gas_station[now].size(); i++) {
			P* A = &gas_station[now][i];
		
			int nxt = A->nxt;
			int nfuel = fuel + A->dis;
			if (station_dis[nxt] > nfuel) {
				pq.push({ nxt, nfuel });
				station_dis[nxt] = nfuel;
			}
		}
	}
	
	int answer = station_dis[mTo];

	if (answer > 100000) {
		return -1;
	}
	return answer;
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
	cout << duration << "ÃÊ" << endl;
	return 0;
}