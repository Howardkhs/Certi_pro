#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <memory.h>
using namespace std;

class Node {
public:
	int oId; // 원래 아디
	int cId; // 채널 아이디 벡터 찾는데
	int Id;  // 
	int time; 
	int mans;
	bool rm = false;
	Node(){}
	void reset(int _oId, int _cId, int _time) {
		oId = _oId; cId = _cId; time = _time; rm = false; mans = 0;
	}
};

struct compare1 {
	bool operator()(Node* A, Node* B) {
		if (A->time != B->time)
			return A->time < B->time;
		return A->oId < B->oId;
	}
};

struct compare2 {
	bool operator()(Node* A, Node* B) {
		if (A->time != B->time)
			return A->time > B->time;
		return A->oId > B->oId;
	}
};

Node nodes[30001];
vector<int> news_follower[501];
vector<Node*> upq[501];
priority_queue<Node*, vector<Node*>, compare2> npq;
unordered_map<int, int> uId_map;
unordered_map<int, int> nId_map;
unordered_map<int, int> cId_map;
int node_ptr;
int user_ptr;
int chan_ptr;

void init(int N, int K)
{
	for (int i = 0; i < 501; i++) {
		news_follower[i].clear();
		upq[i].clear();
	}
	npq = priority_queue<Node*, vector<Node*>, compare2>();
	uId_map.clear(); nId_map.clear(); cId_map.clear();

	node_ptr = 1;
	user_ptr = 1;
	chan_ptr = 1;
}

int set_uId(int mUID) {
	int uId = uId_map[mUID];
	if (uId == 0) {
		uId = user_ptr;
		uId_map[mUID] = user_ptr++;
	}
	return uId;
}

int set_cId(int mCID) {
	int cId = cId_map[mCID];
	if (cId == 0) {
		cId = chan_ptr;
		cId_map[mCID] = chan_ptr++;
	}
	return cId;
}

int set_nId(int mNID) {
	int nId = nId_map[mNID];
	if (nId == 0) {
		nId = node_ptr;
		nId_map[mNID] = node_ptr++;
	}
	return nId;
}

void send_news(int mTime) {
	while (!npq.empty()) {
		Node* N = npq.top(); 
		if (N->time > mTime)	return;
		npq.pop();
		
		if (N->rm)	continue;

		int cId = N->cId;

		for (int i = 0; i < news_follower[cId].size(); i++) {
			int uId = news_follower[cId][i];
			upq[uId].push_back(N);
		}
		N->mans = news_follower[cId].size();
	}
}

void registerUser(int mTime, int mUID, int mNum, int mChannelIDs[])
{
	send_news(mTime);
	int uId = set_uId(mUID);
	for (int i = 0; i < mNum; i++) {
		int cId = set_cId(mChannelIDs[i]);
		news_follower[cId].push_back(uId);
	}
}

int offerNews(int mTime, int mNewsID, int mDelay, int mChannelID)
{
	int nId = set_nId(mNewsID);
	int cId = set_cId(mChannelID);

	Node* N = &nodes[nId];
	N->reset(mNewsID, cId, mTime + mDelay);

	npq.push(N);

	return news_follower[cId].size();
}

void cancelNews(int mTime, int mNewsID)
{
	int nId = set_nId(mNewsID);
	Node* N = &nodes[nId];
	N->rm = true;
}

int checkUser(int mTime, int mUID, int mRetIDs[])
{
	send_news(mTime);

	int uId = set_uId(mUID);

	int cnt = 0;

	vector<Node*>* ptr = &upq[uId];

	for (int i = ptr->size() - 1; i >= 0; i--) {
		Node* N = (*ptr)[i];
		if (N->rm)	continue;
		if (cnt < 3) {
			mRetIDs[cnt] = N->oId;
		}
		cnt++;
	}
	ptr->clear();
	
	return cnt;
}
/////////////////////////////////////////////////////////////////////////

#define INIT	0
#define REGI	1
#define OFFER	2
#define CANCEL	3
#define CHECK	4

static int gids[30];
static int ansids[3];
static int retids[3];
static bool run()
{
	int N, K;
	int cmd, ans, ret;
	int time, num, uid, gid, nid, delay;

	int Q = 0;
	bool okay = false;

	scanf("%d", &Q);
	for (int q = 0; q < Q; ++q)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case INIT:
			scanf("%d %d", &N, &K);
			init(N, K);
			okay = true;
			break;

		case REGI:
			scanf("%d %d %d", &time, &uid, &num);
			for (int m = 0; m < num; m++) {
				scanf("%d", &gids[m]);
			}
			registerUser(time, uid, num, gids);
			break;

		case OFFER:
			scanf("%d %d %d %d %d", &time, &nid, &delay, &gid, &ans);
			ret = offerNews(time, nid, delay, gid);
			if (ans != ret) {
				okay = false;
			}
			break;

		case CANCEL:
			scanf("%d %d", &time, &nid);
			cancelNews(time, nid);
			break;

		case CHECK:
			scanf("%d %d %d", &time, &uid, &ans);
			ret = checkUser(time, uid, retids);

			num = ans;
			if (num > 3) num = 3;
			for (int m = 0; m < num; m++) {
				scanf("%d", &ansids[m]);
			}
			if (ans != ret) {
				okay = false;
			}
			else {
				for (int m = 0; m < num; m++) {
					if (ansids[m] != retids[m]) {
						okay = false;
					}
				}
			}
			break;

		default:
			okay = false;
		}
	}

	return okay;
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