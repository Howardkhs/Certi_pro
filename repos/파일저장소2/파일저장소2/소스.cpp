#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_COUNT 4

#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

struct Node {
public:
	int start; 
	int end;
	int fid;
	int id;
	Node(){}
	void node_set(int _start, int _end, int _fid, int _id) {
		start = _start; end = _end; fid = _fid; id = _id;
	}
};

struct compare {
	bool operator()(const Node* A, const Node* B)const {
		return A->start < B->start;
	}
};
	
int file_start;
int file_end;

Node nodes[400001];
unordered_map<int, int> fid_map;
vector<Node*> fvec[30000];

set<Node*, compare> using_space;
set<Node*, compare> recycling_space;
int file_size;
int node_ptr;
int n;


int get_id(int fid) {
	int id = fid_map[fid];
	if (id == 0) {
		id = node_ptr++;
		fid_map[fid] = id;
	}
	return id;
}

void init(int _N) {
	
	n = _N;
	file_size = 0;
	file_start = file_end = node_ptr = 1;
	
	fid_map.clear();
	using_space.clear(); recycling_space.clear();
	for (int i = 0; i < 30000; i++) {
		fvec[i].clear();
	}
}

int get_new_space(int id, int fid, int mSize) {
	Node* N = &nodes[node_ptr++];

	int start = file_end; int end = file_end + mSize;
	N->node_set(start, end, fid, id);
	using_space.insert(N);
	fvec[N->id].push_back(N);
	file_end += mSize;

	return start;
}

int recycle(int id, int fid, int mSize) {
	
	auto p = recycling_space.begin();
	Node* N = *p;
	recycling_space.erase(p);

	int fsize = N->end - N->start;

	if (mSize >= fsize) {
		N->node_set(N->start, N->end, fid, id);
		using_space.insert(N);
		fvec[N->id].push_back(N);
		return mSize - fsize;
	}

	int start = N->start; int end = N->end;
	Node* M = &nodes[node_ptr++];
	M->node_set(start, start + mSize, fid, id);
	using_space.insert(M);
	fvec[M->id].push_back(M);
	N->node_set(start + mSize, end, N->fid, N->id);
	recycling_space.insert(N);
	return 0;
}

int add(int mId, int mSize) {
		
	if (file_size + mSize > n)	return -1;

	int id = get_id(mId);
	int answer;

	file_size += mSize;

	if (recycling_space.empty()) {
		answer = get_new_space(id, mId, mSize);
		return answer;
	}

	answer = (*recycling_space.begin())->start;

	while (!recycling_space.empty() && mSize) {
		mSize = recycle(id, mId, mSize);
	}
	if (mSize) {
		get_new_space(id, mId, mSize);
	}
	return answer;
}	

int remove(int mId) {
	int id = get_id(mId);
	
	vector<Node*> temp;
	temp.push_back(fvec[id][0]);
	using_space.erase(fvec[id][0]);

	for (int i = 1; i < fvec[id].size(); i++) {
		Node* N = fvec[id][i];
		using_space.erase(N);
		
		auto p = temp.rbegin();
		Node* E = *p;
		if (E->end == N->start) {
			E->end = N->end;
			continue;
		}
		temp.push_back(N);
	}

	for (int i = 0; i < temp.size(); i++) {
		file_size -= (temp[i]->end - temp[i]->start);
		recycling_space.insert(temp[i]);
	}
	fvec[id].clear();
	int answer = temp.size();
	return answer;
}

int count(int mStart, int mEnd) {

	Node T;
	T.node_set(mStart, mEnd, 0, 0);

	set<int> temp;

	auto p = using_space.upper_bound(&T);

	if (p != using_space.begin()) p--; if (p != using_space.begin()) p--;
	if (p != using_space.begin()) p--; if (p != using_space.begin()) p--;

	while (1) {
		if (p == using_space.end())	break;
		Node* N = *p;
		int start = N->start; int end = N->end; end -= 1;
		if (mEnd < start)	break;

		if (start <= mStart && mEnd <= end) {
			temp.insert(N->id);
		}
		else if (mStart <= start && start <= mEnd) {
			temp.insert(N->id);
		}
		else if (mStart <= end && end <= mEnd) {
			temp.insert(N->id);
		}
		p++;
	}
	return temp.size();
}

/////////////////////////////////////////////////////////////////////////


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
			cout << cmd << endl;
			okay = 1;
		}
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