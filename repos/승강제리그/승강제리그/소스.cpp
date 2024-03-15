#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class P {
public:
	int id;
	int ab;
	P(){}
};

struct compare {
	bool operator()(const  P* A, const P* B) const { // 높은게 먼저
		if (A->ab != B->ab)
			return A->ab > B->ab;
		return A->id < B->id;
	}
};

struct compare2 {
	bool operator()(const  P* A, const P* B) const { // 낮은게 먼저
		if (A->ab != B->ab)
			return A->ab < B->ab;
		return A->id > B->id;
	}
};

bool Compare(const  P* A, const P* B) { // 낮은게 먼저
	if (A->ab != B->ab)
		return A->ab < B->ab;
	return A->id > B->id;
}

P players[40000];
int n, L, mem;

set<P*, compare2> low_pq[10]; 
set<P*, compare>  high_pq[10];

void init(int _N, int _L, int mAbility[])
{
	n = _N;  L = _L; mem = n / L;
	vector<P*> temp;
	int idx = 0;


	for (int i = 0; i < 10; i++) {
		low_pq[i].clear(); high_pq[i].clear();
	}

	for (int i = 0; i < n; i++) {
		players[i].id = i;
		players[i].ab = mAbility[i];
		temp.push_back(&players[i]);

		if (i % mem == mem - 1) {
			sort(temp.begin(), temp.end(), Compare);
			for (int j = 1; j <= mem; j++) {
				if (j <= (mem + 1) / 2) {
					high_pq[idx].insert(temp[j - 1]);
				}
				else {
					low_pq[idx].insert(temp[j - 1]);
				}
			}
			idx++;
			temp.clear();
		}
	}
}

P* get_best(int idx) {
	auto p = prev(low_pq[idx].end(), 1);
	P* A = *p;
	low_pq[idx].erase(p);
	return A;
}

P* get_worst(int idx) {
	auto p = prev(high_pq[idx].end(), 1);
	P* A = *p;
	high_pq[idx].erase(p);
	return A;
}

void insert_pq(P* N, int idx) {

	set<P*, compare>* hptr = &high_pq[idx];
	set<P*, compare2>* lptr = &low_pq[idx];

	if (hptr->size() > lptr->size()) {
		P* T = (*hptr->begin()); hptr->erase(T);
		lptr->insert(T);
	}

	lptr->insert(N);
	P* M = (*lptr->begin());
	lptr->erase(M);
	hptr->insert(M);
}

int move()
{	
	int answer = 0;

	vector<P*> worsts;
	vector<P*>	bests;

	for (int i = 1; i < L; i++) {
		P* A = get_worst(i - 1); P* B = get_best(i);
		worsts.push_back(A); bests.push_back(B);
		answer += A->id + B->id;
		
	}
	for (int i = 0; i < worsts.size(); i++) {
		P* A = worsts[i]; P* B = bests[i];
		insert_pq(B, i); insert_pq(A, i + 1);
	}
	return answer;
}

P* get_mid(int idx) {
	auto p = high_pq[idx].begin();
	P* A = *p;
	high_pq[idx].erase(p);
	return A;
}

int trade()
{
	vector<P*>  mids;
	vector<P*>	bests;
	int answer = 0;
	for (int i = 1; i < L; i++) {
		P* A = get_mid(i - 1); P* B = get_best(i);
		answer += A->id + B->id;
		mids.push_back(A); bests.push_back(B);
	}
	for (int i = 0; i < mids.size(); i++) {
		P* A = mids[i]; P* B = bests[i];
		insert_pq(B, i); insert_pq(A, i+1);
	}
	exit(0);
	return answer;
}
#define MAX_N 39990

#define CMD_INIT 100
#define CMD_MOVE 200
#define CMD_TRADE 300

static bool run()
{
	int query_num;
	scanf("%d", &query_num);

	int ans;
	bool ok = false;

	for (int q = 0; q < query_num; q++)
	{
		int query;
		scanf("%d", &query);
		if (query == CMD_INIT)
		{
			int N;
			int L;
			int mAbility[MAX_N];
			scanf("%d %d", &N, &L);
			for (int i = 0; i < N; i++) {
				scanf("%d", &mAbility[i]);
			}
			init(N, L, mAbility);
			ok = true;
		}
		else if (query == CMD_MOVE)
		{
			int ret = move();
			scanf("%d", &ans);
			if (ans != ret)
			{
				ok = false;
			}
		}
		else if (query == CMD_TRADE)
		{
			int ret = trade();
			scanf("%d", &ans);
			if (ans != ret)
			{
				ok = false;
			}
		}
	}
	return ok;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);
	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	return 0;
}