#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>


#include <vector>
#include <queue>
#include <iostream>
#include <memory.h>


using namespace std;

class Post {
public:
    int like;
    int timestamp;
    int uID;
    int pID;

    Post() {}
    Post(int _like, int _timestamp, int _uID, int _pID) {
        like = _like;
        timestamp = _timestamp;
        uID = _uID;
        pID = _pID;
    }
    void set(int _like, int _timestamp, int _uID, int _pID) {
        like = _like;
        timestamp = _timestamp;
        uID = _uID;
        pID = _pID;
    }
};

struct compare {
    bool operator()(Post& a, Post& b) {
        if (a.like != b.like)
            return a.like < b.like;
        return a.timestamp < b.timestamp;
    }
};

struct compare2_ {
	bool operator()(Post& a, Post& b) {
		return a.timestamp < b.timestamp;
	}
};

bool compare2(Post& a, Post& b) {

	return a.timestamp < b.timestamp;

}
typedef priority_queue<Post, vector<Post>, compare> pq;
typedef vector<Post> pq2;
Post posts[100002]; // 글의 좋아요 개수 확인
vector<Post> mans_post[1001];
pq before_1000[1001];
pq2 after_1000[1001];
vector<int> followee[1001];

void init(int N)
{
    //0번 순서 제거

    for (int i = 0; i <= N; i++) {
        mans_post[i].clear();
        followee[i].clear();
        before_1000[i] = pq();
        after_1000[i] = pq2();
    }
    cout << "init" << endl;
}

void follow(int uID1, int uID2, int timestamp)
{   // 1이 2 팔로우
    cout <<  "follow " << uID1 << " " << uID2 << " " << timestamp << endl;
    followee[uID1].push_back(uID2);
}

void makePost(int uID, int pID, int timestamp)
{
    // A 생성
    cout <<  "make post " << uID << " " << pID << " " << timestamp << endl; 
	Post A(0, timestamp, uID, pID);
	posts[pID] = A;
    
    
    // 사람이 작성한 글에 넣고 1000초 이내에 푸쉬
    mans_post[uID].push_back(A);
    before_1000[uID].push(A);
    // 팔로이한테 글 전달
}

void like(int pID, int timestamp)
{
    cout <<  "like " << pID << " " << timestamp << endl;
    posts[pID].like += 1; // 좋아요 확인용
	Post A(posts[pID].like, posts[pID].timestamp, posts[pID].uID, posts[pID].pID);
	mans_post[posts[pID].uID].push_back(A);
	before_1000[posts[pID].uID].push(A);
    //Post A(posts[pID].like, posts[pID].timestamp, posts[pID].uID, posts[pID].pID, posts[pID].mID);
}
void add_after(Post A) {
	
	int idx = after_1000[A.uID].size();
	for (int i = 0; i < after_1000[A.uID].size(); i++) {
		if (after_1000[A.uID][i].timestamp < A.timestamp) {
			idx = i;
			break;
		}
	}
	after_1000[A.uID].insert(after_1000[A.uID].begin() + idx, A);

	if (after_1000[A.uID].size() > 10) {
		after_1000[A.uID].pop_back();
	}

}

void getFeed(int uID, int timestamp, int pIDList[])
{
    cout <<  "getfeed " << uID << " " << timestamp << endl;;
    memset(pIDList, 0, 10 * sizeof(int));
    int cnt = 0;

	priority_queue<Post, vector<Post>, compare> gpq;
	priority_queue<Post, vector<Post>, compare2_> gpq2;
    vector<Post> answer2;
	vector<Post> answer;
	int like = 0;
	int idx = -1;

	while (!before_1000[uID].empty()) {
		Post B = before_1000[uID].top();
		before_1000[uID].pop();
		if (posts[B.pID].like != B.like)
			continue;
		if (timestamp - 1000 > B.timestamp) {
			add_after(B);
			continue;
		}
		gpq.push(B);
		break;
	}

	for (int i = 0; i < followee[uID].size(); i++) {
		while (!before_1000[followee[uID][i]].empty()) {
			Post A = before_1000[followee[uID][i]].top();
			before_1000[followee[uID][i]].pop();
			if (posts[A.pID].like != A.like)
				continue;
			if (timestamp - 1000 > A.timestamp) {
				add_after(A);
				continue;
			}
			gpq.push(A);
			continue;
		}
	}
	
	while (!gpq.empty()) {
		Post B = gpq.top();
		gpq.pop();
		answer.push_back(B);
		cnt++;
		if (!before_1000[B.uID].empty()) {
			Post C = before_1000[B.uID].top();
			before_1000[B.uID].pop();
			if (posts[C.pID].like != C.like)
				continue;
			if (timestamp - 1000 > C.timestamp) {
				add_after(C);
				continue;
			}
			gpq.push(C);
		}
		if (cnt == 10) break;
	}

	if (!after_1000[uID].empty()) {
		Post A = after_1000[uID][0];
		after_1000[uID].erase(after_1000[uID].begin());
		gpq2.push(A);
	}
	for (int i = 0; i < followee[uID].size(); i++) {
		if (!after_1000[followee[uID][i]].empty()) {
			Post A = after_1000[followee[uID][i]][0];
			after_1000[followee[uID][i]].erase(after_1000[followee[uID][i]].begin());
			gpq2.push(A);
		}
	}

	while (cnt < 10 && !gpq2.empty()) {
		Post B = gpq2.top();
		gpq2.pop();
		answer2.push_back(B);
		cout << B.pID << " ";
		cnt++;
		if (!after_1000[B.uID].empty()) {
			Post A = after_1000[B.uID][0];
			after_1000[B.uID].erase(after_1000[B.uID].begin());
			gpq2.push(A);
			//cout << "추가 : " << A.pID << " ";
		}
		if (cnt == 10) break;
	}
	cout << endl;

	for (int i = 0; i < answer.size(); i++) {
		pIDList[i] = answer[i].pID;
		before_1000[answer[i].uID].push(answer[i]);
	}
	for (int i = 0; i < answer2.size(); i++) {
		pIDList[answer.size() + i] = answer2[i].pID;
		add_after(answer2[i]);
	}

	while (!gpq.empty()) {
		Post B = gpq.top();
		gpq.pop();
		before_1000[B.uID].push(B);
	}
	while (!gpq2.empty()) {
		Post B = gpq2.top();
		gpq2.pop();
		add_after(B);
	}
	for (int i = 0; i < 10; i++) {
		cout << pIDList[i] << " ";
	}
	cout << endl;

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