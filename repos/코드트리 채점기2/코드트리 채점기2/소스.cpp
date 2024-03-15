#include <iostream>
#include <set>
#include <queue>
#include <memory.h>
#include <unordered_map>
#include <string>

using namespace std;

class P {
public:
	int DId;  // 도메인 id
	int MId;   // 문제 id
	int time;
	int p;	  // 우선순위
	string url;
	P() {}
	P(int _DId, int _MId, int _time, int _p, string _url) {
		DId = _DId; MId = _MId; time = _time; p = _p; url = _url;
	}
};

struct compare {
	bool operator()(const P& A, const P& B) const {
		if (A.p != B.p)
			return  A.p > B.p;
		return A.time > B.time;
	}
};

unordered_map<string, int> domain_id;
unordered_map<string, bool> is_in;  // 똑같은거 있냐?
priority_queue<int, vector<int>, greater<int>> usable_machine; // 사용 가능한 채점기
priority_queue<P, vector<P>, compare> pq[501];  // 도메인 별로 대기큐 ㅅ애성

bool domain_running[50001]; // 채점 진행중인 도메인인지 체크
bool machine_running[50001];
int domain_end_time[50001]; // 도메인이 언제까치 채점 못하는지
P machine[50001];			// 머신안에 있는 문제들

int domain_ptr;

int q, n, order; // n 채점기
string url;
int cnt = 0;

int slicing_url(string url) {

	for (int i = 0; i < url.length(); i++) {
		if (url[i] == '/')
			return i;
	}
}

int get_id(string domain) {
	int id = domain_id[domain];
	if (id == 0) {
		id = domain_ptr++;
		domain_id[domain] = id;
	}
	return id;
}

void init() {

	domain_id.clear();
	is_in.clear();
	domain_ptr = 1;

	cin >> n;
	cin >> url;

	usable_machine = priority_queue<int, vector<int>, greater<int>>();
	for (int i = 1; i <= n; i++) {
		usable_machine.push(i);
	}

	for (int i = 1; i < 501; i++)
		pq[i] = priority_queue<P, vector<P>, compare>();
	memset(domain_running, 0, sizeof domain_running);
	memset(domain_end_time, 0, sizeof domain_end_time);
	memset(machine_running, 0, sizeof machine_running);

	int idx = slicing_url(url);
	string domain = url.substr(0, idx);
	int MId = stoi(url.substr(idx + 1, domain.length()));
	int DId = get_id(domain);
	int time = 0;
	int p = 1;

	pq[DId].push({ DId, MId, time, p, url });
	is_in[url] = 1;
	cnt++;
}

void asking_judging() {
	int time, p;
	cin >> time >> p >> url;

	if (is_in[url])	return;
	is_in[url] = 1;

	int idx = slicing_url(url);
	string domain = url.substr(0, idx);
	int MId = stoi(url.substr(idx + 1, domain.length()));
	int DId = get_id(domain);

	pq[DId].push({ DId, MId, time, p, url });
	cnt++;
}

void trying_judging() {
	int time, DId, ptime;
	cin >> time;

	if (usable_machine.empty())	return;

	priority_queue<P, vector<P>, compare> temp;

	for (DId = 1; DId <= domain_ptr; DId++) {
		if (domain_running[DId])	continue;
		if (domain_end_time[DId] > time)	continue;
		if (pq[DId].empty())	continue;
		P A = pq[DId].top(); pq[DId].pop();
		temp.push(A); 
	}

	if (temp.empty()) return;

	P N = temp.top(); temp.pop();

	url = N.url;

	int judging_id = usable_machine.top(); usable_machine.pop();

	is_in[url] = 0;
	DId = N.DId;
	machine[judging_id] = N;
	machine[judging_id].time = time;// 들어간 시간으로 변경
	domain_running[DId] = true;
	machine_running[judging_id] = true;
	cnt--;

	while (!temp.empty()) {
		P A = temp.top(); temp.pop();
		DId = A.DId;
		pq[DId].push(A);
	}
}

void finish_judging() {
	int time, judging_id;
	cin >> time >> judging_id;

	if (!machine_running[judging_id])	return;

	P N = machine[judging_id];
	int DId = N.DId;
	int ptime = N.time;
	int end_time = ptime + (time - ptime) * 3;
	domain_end_time[DId] = end_time;
	domain_running[DId] = false;
	machine_running[judging_id] = false;
	usable_machine.push(judging_id);
}

void q_size() {
	int time; cin >> time;
	cout << cnt << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);

	cin >> q;

	while (q--) {
		cin >> order;
		if (order == 100)		init();
		else if (order == 200)	asking_judging();
		else if (order == 300)	trying_judging();
		else if (order == 400)	finish_judging();
		else if (order == 500)	q_size();
	}

}