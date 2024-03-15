#include <iostream>
#include <memory.h>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int visited[10001];

struct P {
	int to;
	int sec;
	P(){}
	P(int _to, int _sec) {
		to = _to;
		sec = _sec;
	}
};

struct S {
	int now;
	int sec;

	S(int _now, int _sec) {
		now = _now;
		sec = _sec;
	}
};

struct compare {
	bool operator()(S& A, S& B) {
		return A.sec > B.sec;
	}
};

vector<P> path[10001];
priority_queue<S, vector<S>, compare> pq;
int times[10001];

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);	cout.tie(NULL);

	int T, n, d, c; // Test case, 컴퓨터 개수, 의존성 개수, 해킹당한 컴퓨터 번호
	int a, b, s;  // a 가 b를 의존, 즉 b에서 a로 전파, 
	int cnt;
	int time = 0;
	cin >> T;
	while (T--) {

		memset(visited, 1, sizeof visited);

		for (int i = 0; i < 10001; i++) {
			path[i].clear();
		}
		pq = priority_queue<S, vector<S>, compare>();
		cnt = 0;

		cin >> n >> d >> c;

		while (d--) {
			cin >> a >> b >> s;
			path[b].push_back({ a,s });
		}

		pq.push({ c,0 });
		visited[c] = 0;
		cnt = 1;
	
		while (!pq.empty()) {
			S N = pq.top();
			pq.pop();

			int now = N.now;	int sec = N.sec;

		
			for (int i = 0; i < path[now].size(); i++) {
				P A = path[now][i];
				int to = A.to; int nsec = A.sec;
				if (visited[to] <= sec + nsec)	continue;
				pq.push({ to, sec + nsec });
				if (visited[to] == 16843009)	cnt++;
				visited[to] = sec + nsec;
				time = max(time, sec + nsec);
	
			}
		
		}

		int answer = 0;
		for (int i = 0; i < 10001; i++) {
			if (visited[i] == 16843009)	continue;
			answer = max(answer, visited[i]);
		}

		cout << cnt << " " << answer << "\n";
	}
}