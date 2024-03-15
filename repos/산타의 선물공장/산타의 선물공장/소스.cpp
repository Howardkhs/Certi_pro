#include <iostream>
#include <list>
#include <memory.h>
#include <algorithm>

using namespace std;

int q, n, m, order;


list<int> belts[100001];

int prv[100001];
int nxt[100001];

void init() {
	int a;
	cin >> n >> m;

	for (int i = 1; i <= m; i++) {
		cin >> a;
		belts[a].push_back(i);
	}

	memset(prv, 0, sizeof prv);
	memset(nxt, 0, sizeof nxt);

	for (int i = 1; i <= n; i++) {
		list<int>* ptr = &belts[i];
		auto it = ptr->begin();
		if (ptr->empty()) continue;

		while (1) {
			if (it == ptr->end())	break;

			int idx = *it;

			if (it == ptr->begin() && it == --ptr->end()) {
				prv[idx] = -1;
				nxt[idx] = -1;
			}
			else if (it == ptr->begin()) {
				prv[idx] = -1;
				auto p = next(it, 1);
				nxt[idx] = *p;
			}
			else if (it == --ptr->end()) {
				nxt[idx] = -1;
				auto p = prev(it, 1);
				prv[idx] = *p;
			}
			else {
				auto p = next(it, 1);
				nxt[idx] = *p;
				p = prev(it, 1);
				prv[idx] = *p;
			}
			it++;
		}
	}

}

void mv_box() {
	int src; int dst;
	cin >> src >> dst;

	if (belts[src].empty()) {
		cout << belts[dst].size() << endl;
		return;
	}
	if (belts[dst].empty()) {
		belts[dst].insert(belts[dst].begin(), belts[src].begin(), belts[src].end());
		belts[src].clear();
		cout << belts[dst].size() << endl;
		return;
	}

	int prv_id = *belts[src].rbegin(); // 확인 피료
	int nxt_id = *belts[dst].begin();
	// auto prv_ptr = prev(it, 1); //  src 벨트 맨뒤에거


	belts[dst].insert(belts[dst].begin(), belts[src].begin(), belts[src].end());
	belts[src].clear();

	nxt[prv_id] = nxt_id; // prv_id의 다음은 
	prv[nxt_id] = prv_id; // nxt_id의 이전은
	cout << belts[dst].size() << endl;
	return;
}

void change_box() { // 앞에 박스 교환
	int src; int dst;
	cin >> src >> dst;

	if (belts[src].empty() && belts[dst].empty()) { 
		cout << belts[dst].size() << endl;
		return; 
	}

	if (belts[src].empty()) {
		auto now = belts[dst].begin();
		int now_id = *now;
		belts[dst].pop_front();
		nxt[now_id] = -1;
		
		if (!belts[dst].empty()) {
			auto nxt_ptr = belts[dst].begin();
			int nxt_id = *nxt_ptr;
			prv[nxt_id] = -1;
		}
		belts[src].push_back(now_id);
	}
	else if (belts[dst].empty()) {
		auto now = belts[src].begin();
		int now_id = *now;
		belts[src].pop_front();
		nxt[now_id] = -1;

		if (!belts[src].empty()) {
			auto nxt_ptr = belts[src].begin();
			int nxt_id = *nxt_ptr;
			prv[nxt_id] = -1;
		}
		belts[dst].push_back(now_id);
	}
	else {
		auto dnow = belts[dst].begin();
		int dnow_id = *dnow;
		belts[dst].pop_front();
		nxt[dnow_id] = -1;

		auto snow = belts[src].begin();
		int snow_id = *snow;
		belts[src].pop_front();
		nxt[snow_id] = -1;

		if (!belts[src].empty()) {
			auto nxt_ptr = belts[src].begin();
			int nxt_id = *nxt_ptr;
			prv[nxt_id] = dnow_id;
			nxt[dnow_id] = nxt_id;
		}

		if (!belts[dst].empty()) {
			auto nxt_ptr = belts[dst].begin();
			int nxt_id = *nxt_ptr;
			prv[nxt_id] = snow_id;
			nxt[snow_id] = nxt_id;
		}
		belts[src].push_front(dnow_id);
		belts[dst].push_front(snow_id);
	}
	cout << belts[dst].size() << endl;
	return;
	
}

void divide_box() {
	int src; int dst;
	cin >> src >> dst;

	if (belts[src].empty() || belts[src].size() == 1) { 
		cout << belts[dst].size() << endl;
		return; 
	}

	int src_size = belts[src].size();

	int div_size = src_size / 2;

	auto dit = next(belts[src].begin(), div_size); // 옮기는 박스 바로 뒤

	int mid = *dit;  // 4
	int back_box_id = prv[mid];
	nxt[prv[mid]] = -1; // 옮기는 박스 뒤 끊어주기
	prv[mid] = -1;

	if (belts[dst].empty()) {
		belts[dst].insert(belts[dst].begin(), belts[src].begin(), dit);
		belts[src].erase(belts[src].begin(), dit);
		cout << belts[dst].size() << endl;
		return;
	}

	int head = (*belts[dst].begin());
	nxt[back_box_id] = head;
	prv[head] = back_box_id;
	belts[dst].insert(belts[dst].begin(), belts[src].begin(), dit);
	belts[src].erase(belts[src].begin(), dit);
	cout << belts[dst].size() << endl;
	return;
}

void info_box() {
	int id;
	cin >> id;

	int a, b;

	a = prv[id];
	if (a == 0) a = -1;
	b = nxt[id];
	if (b == 0) b = -1;

	cout << a + 2 * b << "\n";
}

void info_belt() {
	int id;
	cin >> id;

	int a, b, c;
	if (belts[id].empty()) {
		a = -1;
		b = -1;
	}
	else {
		a = *belts[id].begin();
		b = *belts[id].rbegin();
	}
	c = belts[id].size();

	cout << a + 2 * b + 3 * c << "\n";
}

void print() {
	for (int i = 1; i <= n; i++) {
		cout << "belt " << i << " : ";
		for (auto p = belts[i].begin(); p != belts[i].end(); p++) {
			cout << *p << " ";
		}
		cout << endl;
		
	}
	for (int i = 1; i <= m; i++) {
		cout << "prv nxt : " << i << " ";
		cout << prv[i] << " " << nxt[i] << endl;
	}

}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);

	cin >> q;
	while (q--) {
		cin >> order;

		if (order == 100)		init();
		else if (order == 200)	mv_box();
		else if (order == 300)	change_box();
		else if (order == 400)	divide_box();
		else if (order == 500)	info_box();
		else if (order == 600)	info_belt();
		//print();
	}
}