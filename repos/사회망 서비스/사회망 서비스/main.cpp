#include <iostream>
#include <vector>

using namespace std;

vector<int> roads[1000001];

int visited[1000001] = { 0, };
int dp[1000001][2] = { 0, };

void dfs(int now) {
	visited[now] = 1;

	dp[now][1] = 1;

	for (int i = 0; i < roads[now].size(); i++) {
		int to = roads[now][i];
		if (visited[to])	continue;

		dfs(to);

		dp[now][0] += dp[to][1]; // 주변 다킴
		dp[now][1] += min(dp[to][0], dp[to][1]); // 주변 다꺼져있어
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	int N,a,b;
	cin >> N;

	for (int i = 0; i < N - 1; i++) {
		cin >> a >> b;
		roads[a].push_back(b);
		roads[b].push_back(a);
	}

	dfs(1);

	cout << dp[1][0] << " " << dp[1][1] << endl;

	cout << min(dp[1][0], dp[1][1]) << endl;
}