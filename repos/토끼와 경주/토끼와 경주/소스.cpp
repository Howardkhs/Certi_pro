#include <iostream>
#include <queue>
#include <memory.h>
#include <unordered_map>
using namespace std;

int n, m, order, p;

class R {
public:
	int id;
	int pid;
	int x;
	int y;
	int cnt;
	int jump;
	long long score;
	R(){}
	void Rset(int _id, int _pid, int _jump) {
		id = _id; pid = _pid; jump = _jump;
	}
};

struct compare {
	bool operator()(R* A, R* B) {
		if (A->cnt != B->cnt)
			return A->cnt > B->cnt;
		if (A->x + A->y != B->x + B->y)
			return A->x + A->y > B->x + B->y;
		if (A->x != B->x)
			return A->x > B->x;
		if (A->y != B->y)
			return A->y > B->y;
		return A->pid > B->pid;
	}
};

struct compare2 {
	bool operator()(R& A, R& B) {
		if (A.x + A.y != B.x + B.y)
			return A.x + A.y < B.x + B.y;
		if (A.x != B.x)
			return A.x < B.x;
		if (A.y != B.y)
			return A.y < B.y;
		return A.pid > B.pid;
	}
};

struct P {
	int x;  int y;
	P(int _x, int _y) {
		x = _x; y = _y;
	}
};

struct compareP {
	bool operator()(P& A, P& B) {
		if (A.x + A.y != B.x + B.y)
			return A.x + A.y < B.x + B.y;
		if (A.x != B.x)
			return A.x < B.x;
		return A.y < B.y;
	}
};

R rabbits[2001];
unordered_map<int, int> r_id;

priority_queue<R*, vector<R*>, compare> pq;

int r_ptr;
long long tot_score = 0;

int get_id(int pid) {
	int id = r_id[pid];
	if (id == 0) {
		id = r_ptr++;
		r_id[pid] = id;
	}
	return id;
}

void init() {
	cin >> n >> m >> p;
	memset(rabbits, 0, sizeof rabbits);
	r_id.clear();
	r_ptr = 1;

	int pid, jump;
	
	for (int i = 1; i <= p; i++) {
		cin >> pid >> jump;
		int id = get_id(pid);
		R* N = &rabbits[id];
		N->Rset(id, pid, jump);
		pq.push(N);
	}
}

int jump_down(int x, int njump) {

	int nx = x + njump;

	if (nx >= n) {
		nx = (n - 1) - (nx - (n - 1));
	}
	if (nx < 0) {
		nx = -nx;
	}
	return nx;
}

int jump_up(int x, int njump) {

	int nx = x - njump;

	if (nx < 0) {
		nx = -nx;
	}
	if (nx >= n) {
		nx = (n - 1) - (nx - (n - 1));
	}
	
	return nx;
}

int jump_right(int y, int njump) {

	int ny = y + njump;

	if (ny >= m) {
		ny = (m - 1) - (ny - (m - 1));
	}
	if (ny < 0) {
		ny = -ny;
	}

	return ny;
}

int jump_left(int y, int njump) {

	int ny = y - njump;

	if (ny < 0) {
		ny = -ny;
	}
	if (ny >= m) {
		ny = (m - 1) - (ny - (m - 1));
	}

	return ny;
}

void race() {
	int K, S;
	cin >> K >> S;

	priority_queue<R, vector<R>, compare2> afterkq;
	
	while (K--) {
		R* N = pq.top(); pq.pop();
		int nx, ny;
		int x = N->x; int y = N->y; int jump = N->jump; int cnt = N->cnt; int score = N->score;
		priority_queue<P, vector<P>, compareP> jumpq;

		int njump = jump % (2 * (n - 1));
		nx = jump_down(x, njump);
		jumpq.push({ nx, y });
		nx = jump_up(x, njump);
		jumpq.push({ nx, y });

		njump = jump % (2 * (m - 1));
		ny = jump_right(y, njump);
		jumpq.push({ x, ny });
		ny = jump_left(y, njump);
		jumpq.push({ x, ny });

		P A = jumpq.top();
		nx = A.x; ny = A.y;
		
		tot_score += nx + ny + 2;
		score -= nx + ny + 2;

		N->cnt += 1;
		N->x = nx; N->y = ny; N->score = score;

		afterkq.push(*N);
		pq.push(N);
	}

	R B = afterkq.top();
	int id = B.id;
	rabbits[id].score += S;
}

void jump_change() {
	int pid, L;

	cin >> pid >> L;
	int id = get_id(pid);
	rabbits[id].jump *= L;
}

void game_end() {

	long long answer = 0;
	for (int i = 1; i <= p; i++) {
		answer = max(answer, rabbits[i].score + tot_score);
	}
	cout << answer << "\n";
}

int main() {

	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	int q;
	cin >> q;
	while (q--) {
		cin >> order;
		if (order == 100)		init();
		else if (order == 200)	race();
		else if (order == 300)	jump_change();
		else if (order == 400)	game_end();
	}

}