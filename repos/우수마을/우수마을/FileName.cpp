#include <vector>
#include <iostream>

using namespace std;

int dp[10001][2] = { 0, };
int visited[10001] = { 0, };

int mans[10001] = { 0, };
vector<int> roads[10001];

void dfs(int now) {
	visited[now] = 1;

	dp[now][1] = mans[now];

	for (int i = 0; i < roads[now].size(); i++) {
		int to = roads[now][i];
		cout << to << " ";
		if (visited[to])	continue;
		dfs(to);

		dp[now][0] += max(dp[to][1], dp[to][0]);
		dp[now][1] += dp[to][0];
	}
}

int main() {

	int N;
	cin >> N;
		
	for (int i = 1; i <= N; i++) {
		cin >> mans[i];
	}

	int a, b;

	for (int i = 0; i < N - 1; i++) {
		cin >> a >> b;
		roads[a].push_back(b);
		roads[b].push_back(a);
	}

	dfs(1);
	cout << max(dp[1][0], dp[1][1]) << endl;
}