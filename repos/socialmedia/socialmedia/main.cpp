#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class P {
public:
	int pId;
	int owner;
	int like;
	int timestamp;

	P(){}
	void reset(int _pId, int _owner, int _like, int _timestamp) {
		pId = _pId; owner = _owner; like = _like; timestamp = _timestamp;
	}
};

struct compare1 {
	bool operator()(P& A, P& B) {
		if (A.like != B.like) 
			return A.like < B.like;
		return A.timestamp < B.timestamp;
	}
};

struct compare2 {
	bool operator()(P& A, P& B) {
		return A.timestamp < B.timestamp;
	}
};

vector<int> follows[1001]; // idx -> 팔로우하는 게시물들
P posts[100001];
priority_queue<P, vector<P>, compare1> pq1[1001];
priority_queue<P, vector<P>, compare2> pq2[1001];
vector<P> return_vec;
priority_queue<P, vector<P>, compare1> tpq1;
priority_queue<P, vector<P>, compare2> tpq2;
int cnt;

void init(int N)
{
	for (int i = 1; i < 1001; i++) {
		follows[i].clear();
		follows[i].push_back(i);
		pq1[i] = priority_queue<P, vector<P>, compare1>();
		pq2[i] = priority_queue<P, vector<P>, compare2>();
	}

}

void follow(int uID1, int uID2, int timestamp)
{
	follows[uID1].push_back(uID2);
}

void makePost(int uID, int pID, int timestamp)
{
	posts[pID].reset(pID, uID, 0, timestamp);
	pq1[uID].push(posts[pID]);
}

void like(int pID, int timestamp)
{
	posts[pID].like += 1;
	pq1[posts[pID].owner].push(posts[pID]);
}

int check_post(P* N, int timestamp) {
	if (N->like != posts[N->pId].like)	return 0;
	if (N->timestamp < timestamp - 1000)	return -1;
	return 1;
}

void set_tpq1(int timestamp) {
	
	priority_queue<P, vector<P>, compare1>* ptr = &tpq1;
	while (!ptr->empty()) {
		P N = ptr->top(); ptr->pop();
		return_vec.push_back(N); // 넣어두고
		cnt++; 
		if (cnt == 10)	return;
		int man = N.owner;
		while (!pq1[man].empty()) {
			P T = pq1[man].top(); pq1[man].pop();
			
			int check = check_post(&T, timestamp);
			if (check == 0)		continue;
			if (check == -1) {
				pq2[N.owner].push(T);
				continue;
			}
			ptr->push(T);
			break;
		}
	}
}


void set_pIDList(int pIDList[]) {
	for (int i = 0; i < return_vec.size(); i++) {
		pIDList[i] = return_vec[i].pId;
	}
}

void set_tpq2(int timestamp) {

	priority_queue<P, vector<P>, compare2>* ptr = &tpq2;
	while (!ptr->empty()) {
		P N = ptr->top(); ptr->pop();
		return_vec.push_back(N); // 넣어두고
		cnt++;
		if (cnt == 10)	return;
		int man = N.owner;
		while (!pq2[man].empty()) {
			P T = pq2[man].top(); pq2[man].pop();
			int check = check_post(&T, timestamp);
			if (check == 0)		continue;
			ptr->push(T);
			break;
		}
	}
}

void return_P(int timestamp) {
	for (int i = 0; i < return_vec.size(); i++) {
		int man = return_vec[i].owner;
		int ptime = return_vec[i].timestamp;
		if (ptime < timestamp - 1000) {
			pq2[man].push(return_vec[i]);
			continue;
		}
		pq1[man].push(return_vec[i]);
	}

	while (!tpq1.empty()) {
		P N = tpq1.top(); tpq1.pop();
		int man = N.owner;
		pq1[man].push(N);
	}

	while (!tpq2.empty()) {
		P N = tpq2.top(); tpq2.pop();
		int man = N.owner;
		pq2[man].push(N);
	}
}

void getFeed(int uID, int timestamp, int pIDList[])
{

	return_vec.clear();	tpq1 = priority_queue<P, vector<P>, compare1>(); tpq2 = priority_queue<P, vector<P>, compare2>(); cnt = 0;

	for (int i = 0; i < follows[uID].size(); i++) {
		int man = follows[uID][i];
	
		while (!pq1[man].empty()) {
			P N = pq1[man].top(); pq1[man].pop();
			int check = check_post(&N, timestamp);
			if (check == 0)		continue;
			if (check == -1) {
				pq2[N.owner].push(N);
				continue;
			}
			tpq1.push(N);
			break;
		}
	}

	set_tpq1(timestamp);
	if (cnt == 10) { 
		set_pIDList(pIDList);
		return_P(timestamp);
		return; 
	}

	for (int i = 0; i < follows[uID].size(); i++) {
		int man = follows[uID][i];

		while (!pq2[man].empty()) {
			P N = pq2[man].top(); pq2[man].pop();
			int check = check_post(&N, timestamp);
			if (check == 0)		continue;
			tpq2.push(N);
			break;
		}
	}

	set_tpq2(timestamp);
	set_pIDList(pIDList);
	return_P(timestamp);
	return;
}

static int mSeed;
static int pseudo_rand(void)
{
	mSeed = mSeed * 431345 + 2531999;
	return mSeed & 0x7FFFFFFF;
}

static int follow_status[1005][1005];
static int answer_score;
static int n; // n >= 2 && n <= 1000
static int end_timestamp;
static int follow_ratio; // follow_ratio >= 1 && follow_ratio <= 10000
static int make_ratio; // make_ratio >= 1 && make_ratio <= 10000
static int like_ratio; // like_ratio >= 1 && like_ratio <= 10000
static int get_feed_ratio; // get_feed_ratio >= 1 && get_feed_ratio <= 10000
static int post_arr[200000];
static int total_post_cnt;
static int min_post_cnt;

static bool run()
{
	int uId1, uId2, pId, pIdList[10], ans_pIdList[10], rand_val;
	bool ret = true;

	scanf("%d%d%d%d%d%d%d", &mSeed, &n, &end_timestamp, &follow_ratio, &make_ratio, &like_ratio, &get_feed_ratio);
	init(n);

	for (int uId1 = 1; uId1 <= n; uId1++)
	{
		follow_status[uId1][uId1] = 1;
		int m = n / 10 + 1;
		if (m > 10)
			m = 10;
		for (int i = 0; i < m; i++)
		{
			uId2 = uId1;
			while (follow_status[uId1][uId2] == 1)
			{
				uId2 = pseudo_rand() % n + 1;
			}
			follow(uId1, uId2, 1);
			follow_status[uId1][uId2] = 1;
		}
	}
	min_post_cnt = total_post_cnt = 1;

	for (int timestamp = 1; timestamp <= end_timestamp; timestamp++)
	{
		rand_val = pseudo_rand() % 10000;
		if (rand_val < follow_ratio)
		{
			uId1 = pseudo_rand() % n + 1;
			uId2 = pseudo_rand() % n + 1;
			int lim = 0;
			while (follow_status[uId1][uId2] == 1 || uId1 == uId2)
			{
				uId2 = pseudo_rand() % n + 1;
				lim++;
				if (lim >= 5)
					break;
			}
			if (follow_status[uId1][uId2] == 0)
			{
				follow(uId1, uId2, timestamp);
				follow_status[uId1][uId2] = 1;
			}
		}
		rand_val = pseudo_rand() % 10000;

		if (rand_val < make_ratio)
		{
			uId1 = pseudo_rand() % n + 1;
			post_arr[total_post_cnt] = timestamp;

			makePost(uId1, total_post_cnt, timestamp);
			total_post_cnt += 1;
		}

		rand_val = pseudo_rand() % 10000;

		if (rand_val < like_ratio && total_post_cnt - min_post_cnt > 0)
		{
			while (post_arr[min_post_cnt] < timestamp - 1000 && min_post_cnt < total_post_cnt)
				min_post_cnt++;

			if (total_post_cnt != min_post_cnt)
			{
				pId = pseudo_rand() % (total_post_cnt - min_post_cnt) + min_post_cnt;
				like(pId, timestamp);
			}
		}

		rand_val = pseudo_rand() % 10000;
		if (rand_val < get_feed_ratio && total_post_cnt > 0)
		{
			uId1 = pseudo_rand() % n + 1;
			getFeed(uId1, timestamp, pIdList);

			for (int i = 0; i < 10; i++)
			{
				scanf("%d", ans_pIdList + i);
			}

			for (int i = 0; i < 10; i++)
			{
				if (ans_pIdList[i] == 0)
					break;

				if (ans_pIdList[i] != pIdList[i])
				{
					ret = false;
				}
			}
		}
	}

	return ret;
}

int main()
{
	freopen("sample_input.txt", "r", stdin);
	setbuf(stdout, NULL);
	int tc;
	scanf("%d%d", &tc, &answer_score);

	for (int t = 1; t <= tc; t++)
	{
		int score;
		for (int i = 0; i < 1005; i++)
			for (int j = 0; j < 1005; j++)
				follow_status[i][j] = 0;

		if (run())
			score = answer_score;
		else
			score = 0;

		printf("#%d %d\n", t, score);
	}
	return 0;
}