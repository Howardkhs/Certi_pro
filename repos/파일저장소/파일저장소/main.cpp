#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_map>
#include <memory.h>

using namespace std;

struct P {
	int start;
	int end;
	int id;
	P() {}
	P(int _start, int _end) {
		start = _start;
		end = _end;
	}
	P(int _start, int _end, int _id) {
		start = _start;
		end = _end;
		id = _id;
	}
};

struct compare {
	bool operator()(const P& A, const P& B) const {
		return A.start < B.start;
	}
};

P used_space(0, 0);
set<P, compare> dq;
set<P, compare> datas;
unordered_map<int, int> id_map;
vector<P> using_space[12001]; // 앞에서 부터 순서대로 넣어라
int ptr;
int N;
int empty_size;
int ids[12001];

void init(int _N) {
	used_space = P(1, 1);
	dq.clear();
	datas.clear();
	id_map.clear();
	id_map.reserve(12000);

	for (int i = 0; i < 12001; i++) {
		using_space[i].clear();
	}

	N = _N;
	ptr = 1;
	empty_size = N;
	return;
}

int add(int mId, int mSize) { // 분할추가, 꽉찼을 경우 추가 합치는 것도 고려해보자

	if (mSize > empty_size) {
		return -1;
	}
	int id = id_map[mId];
	int psize;
	int answer;

	empty_size -= mSize; // 빈공간

	if (id == 0) {
		id_map[mId] = ptr;
		id = ptr++;
	}

	//cout << "MID : " << mId << " ID : " << id << " " <<"Size : " << mSize << endl;

	if (dq.empty()) {
		int start = used_space.start;	int end = used_space.end;
		using_space[id].push_back({ end, end + mSize, id });
		used_space.end += mSize;
		answer = end;
		datas.insert({ end, end + mSize, id });
	}
	else {

		answer = (*dq.begin()).start;
		while (!dq.empty()) {
			P part = *dq.begin();
			dq.erase(*dq.begin());

			int start = part.start;	int end = part.end;
			psize = end - start;
			if (psize > mSize) { // 주
				if (!using_space[id].empty() && (*using_space[id].rbegin()).end == start) {
					datas.erase({ using_space[id].rbegin()->start, using_space[id].rbegin()->end, using_space[id].rbegin()->id });
					(*using_space[id].rbegin()).end = start + mSize;
					datas.insert({ using_space[id].rbegin()->start, start + mSize, using_space[id].rbegin()->id });
				}
				else {
					using_space[id].push_back({ start, start + mSize, id });
					datas.insert({ start, start + mSize, id });
				}
				dq.insert({ start + mSize, end });
				mSize = 0;

			}
			else {
				mSize -= psize;
				if (!using_space[id].empty() && (*using_space[id].rbegin()).end == start) {
					datas.erase({ using_space[id].rbegin()->start, using_space[id].rbegin()->end, using_space[id].rbegin()->id });
					(*using_space[id].rbegin()).end = end;
					datas.insert({ using_space[id].rbegin()->start, end, using_space[id].rbegin()->id });
				}
				else {
					using_space[id].push_back({ start, end, id });
					datas.insert({ start, end, id });
				}
			}
	
			if (mSize == 0) {
				break;
			}
		}
		if (dq.empty() && mSize > 0) {
			int start = used_space.start;	int end = used_space.end;
			if (!using_space[id].empty() && (*using_space[id].rbegin()).end == end) {
				datas.erase({ using_space[id].rbegin()->start, using_space[id].rbegin()->end, using_space[id].rbegin()->id });
				(*using_space[id].rbegin()).end = end + mSize;
				datas.insert({ using_space[id].rbegin()->start, end + mSize, using_space[id].rbegin()->id });
			}
			else {
				using_space[id].push_back({ end, end + mSize, id });
				datas.insert({ end, end + mSize, id });
			}
			used_space.end += mSize;

		}
	}

	return answer;
}

int remove(int mId) {

	int id = id_map[mId];

	vector<P>* ptr = &using_space[id];
	int size = ptr->size();

	for (int i = size - 1; i >= 0; i--) {
		P* A = &(*ptr)[i];
		empty_size += A->end - A->start; // 빈공간
		dq.insert(*A);
		datas.erase(*A);
	}
	ptr->clear();
	return size;
}

int count(int mStart, int mEnd) {

	//cout << "4번 : " << endl;
	//cout << mStart << " " << mEnd << endl;

	memset(ids, 0, sizeof ids);
	int cnt = 0;

	auto p = datas.upper_bound({ mStart, mEnd });
	int t = 0;
	while (p != datas.begin()) {
		p--;
		t++; 
		if (t == 2)	break;
	}

	for (; p != datas.end(); p++) {
		//cout << p->id << " " << p->start << " " << p->end << " \n";

		if (mStart <= p->start && p->start <= mEnd) {
			if (ids[p->id] == 1)	continue;
			else
			{
				ids[p->id] = 1;
				cnt++;
				continue;
			}
		}
		else if (mStart <= p->end - 1 && p->end - 1 <= mEnd) {
			if (ids[p->id] == 1)	continue;
			else
			{
				ids[p->id] = 1;
				cnt++;
				continue;
			}
		}
		else if (p->start <= mStart && mStart <= p->end - 1) {
			if (ids[p->id] == 1)	continue;
			else
			{
				ids[p->id] = 1;
				cnt++;
				continue;
			}
		}
		if (p->end - 2 > mEnd)	break;

	}


	return cnt;
}

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_COUNT 4

static bool run() {
	int q;
	scanf("%d", &q);

	int mid, msize, mstart, mend, n;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			scanf("%d", &n);
			init(n);
			okay = true;
			break;
		case CMD_ADD:
			scanf("%d %d %d", &mid, &msize, &ans);
			ret = add(mid, msize);
			if (ans != ret)
				okay = false;
			break;
		case CMD_REMOVE:
			scanf("%d %d", &mid, &ans);
			ret = remove(mid);
			if (ans != ret)
				okay = false;
			break;
		case CMD_COUNT:
			scanf("%d %d %d", &mstart, &mend, &ans);
			ret = count(mstart, mend);
			if (ans != ret)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
		if (okay == false) {
			
			cout << "cmd : " << cmd << endl;
			okay = true;
		}
		/*
		cout << "빈공간들 : " << endl;
		for (auto p = dq.begin(); p != dq.end(); p++) {
			cout << p->start << " " << p->end << endl;
		}*/
	}
	return okay;
}

int main() {
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}