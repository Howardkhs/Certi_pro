#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory.h>
#include <list>
#include <set>
#include <queue>
#include <stdio.h>

using namespace std;

int q, L, order;

struct Sushi {
	int time;
	int x;
	int id;
	string name;
	Sushi(){}
	Sushi(int _time, int _x, string _name, int _id){
		time = _time; x = _x; name = _name, id = _id;
	}
	void mk_sushi(int _time, int _x, string _name, int _id) {
		time = _time; x = _x; name = _name, id = _id;
	}
};

struct compare {
	bool operator()(Sushi &A, Sushi &B) {
		return A.time > B.time;
	}
};

unordered_map<string, int> name_id_map;
vector<Sushi> names[15001];
priority_queue<Sushi, vector<Sushi>, compare> pq;
int eat_cnt[15001] = { 0, };
int sit_pos[15001];
int name_ptr = 1;
int lives = 0;
int sushi_cnt = 0;

int get_id(string name) {
	int id = name_id_map[name];
	if (id == 0) {
		id = name_ptr++;
		name_id_map[name] = id;
	}
	return id;
}

void input_pq(Sushi* N, int time, int x) { // x-> 사람이 앉은 자리
	int eta;
	string sname = N->name;
	int sx = N->x; int stime = N->time; int sid = N->id;
	int mv = (time - stime) % L;
	sx += mv;
	sx = sx % L;

	if (sx > x) {
		eta = x + L - sx;
	}
	else {
		eta = x - sx;
	}
	pq.push({ time + eta, sx, sname, sid });
}

void make_sushi() {
	int time, x;	string name;
	cin >> time >> x >> name;

	int id = get_id(name);
	sushi_cnt++;
	if (!eat_cnt[id]) {
		names[id].push_back({ time, x, name, id});
		return;
	}

	Sushi N;
	N.mk_sushi(time, x, name, id);
	// 앉은자리 나중에 가져오게
	int sit_x = sit_pos[id];
	input_pq(&N, time, sit_x);
}

void man_sit() {
	int time, x, n;
	string name;
	cin >> time >> x >> name >> n;
	if (n == 0)	return;

	int id = get_id(name);
	sit_pos[id] = x;
	eat_cnt[id] = n;
	lives++;

	for (int i = 0; i < names[id].size(); i++) {
		Sushi* N = &names[id][i];
		input_pq(N, time, x);
	}
}


void take_pic() {
	int time;
	cin >> time;

	while (!pq.empty()) {
		Sushi N = pq.top();
		if (N.time > time)	break;
		pq.pop(); sushi_cnt--;
		int id = N.id; 
		eat_cnt[id] -= 1;
		if (!eat_cnt[id]) lives--;
	} 

	cout << lives << " " << sushi_cnt << "\n";

}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	//freopen("input.txt", "r", stdin);
	cin >> L >> q;
	
	while (q--) {
		cin >> order;
		switch (order) {
			case 100:
				make_sushi();
				break;
			case 200:
				man_sit();
				break;
			case 300:
				take_pic();
				break;
		}
	}
}