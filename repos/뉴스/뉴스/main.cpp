#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <queue>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory.h>

using namespace std;

class News {
public:
	int mTime;
	int mNewsID;
	int mChannelID;
	int oNewsID; // 원래 뉴스 아이디

	News() {}
	News(int _mTime, int _mNewsID, int _mChannelID, int _oNewsID) {
		mTime = _mTime;
		mNewsID = _mNewsID;
		mChannelID = _mChannelID;
		oNewsID = _oNewsID;
	}

	void set(int _mTime, int _mNewsID, int _mChannelID, int _oNewsID) {
		mTime = _mTime;
		mNewsID = _mNewsID;
		mChannelID = _mChannelID;
		oNewsID = _oNewsID;
	}

};

struct compare {
	bool operator()(const News* a, const News* b) {
		if (a->mTime != b->mTime)
			return a->mTime > b->mTime; // 산을 올라가니까 오름차순
		return a->oNewsID > b->oNewsID;
	}
};

struct compare2 {
	bool operator()(News& a, News& b) {
		if (a.mTime != b.mTime)
			return a.mTime < b.mTime;
		return a.oNewsID < b.oNewsID;
	}
};


vector<News> Users[500];
vector<int> f_channel[500];
News Newspool[30000];
bool newss[30000]; // 뉴스의 신뢰성 체크
unordered_map<int, int> User_id;
unordered_map<int, int> Channel_id;
unordered_map<int, int> News_id;
unordered_set<int>		User_set;
unordered_set<int>		Channel_set;

priority_queue<News*, vector<News*>, compare> pq;
//priority_queue<News, vector<News>, compare2> Upq[500];
vector<News*> Upq[500];

int User_ptr;
int News_ptr;
int Channel_ptr;


void init(int N, int K)
{
	for (int i = 0; i < 500; i++) {
		Users[i].clear();
		f_channel[i].clear();
		Upq[i].clear();

	}
	memset(newss, 0, sizeof newss);
	User_id.clear();
	Channel_id.clear();
	News_id.clear();
	User_set.clear();
	Channel_set.clear();

	priority_queue<News*, vector<News*>, compare> empty;
	swap(pq, empty);

	User_ptr = 0;
	News_ptr = 0;
	Channel_ptr = 0;

}

void send_News(int mTime) {
	while (!pq.empty()) {
		if (pq.top()->mTime > mTime) return;
		News *A = pq.top();

		if (A->mTime <= mTime) {
			//cout << "시간 : " << A.mTime << endl;
			pq.pop();
			if (newss[A->mNewsID] == false)	continue;

			for (int i = 0; i < f_channel[A->mChannelID].size(); i++) {
				Upq[f_channel[A->mChannelID][i]].push_back(A);
			}
		}
		else {
			break;
		}
	}
}

void registerUser(int mTime, int mUID, int mNum, int mChannelIDs[])
{
	send_News(mTime);

	auto it = User_set.find(mUID);
	int User_idx;
	if (it == User_set.end()) {
		User_set.insert(mUID);
		User_idx = User_ptr;
		User_id[mUID] = User_ptr++;
	}
	else {
		User_idx = User_id[mUID];
	}

	for (int i = 0; i < mNum; i++) {
		int CID = mChannelIDs[i];
		it = Channel_set.find(CID);
		int Channel_idx;
		if (it == Channel_set.end()) {
			Channel_set.insert(CID);
			Channel_idx = Channel_ptr;
			Channel_id[CID] = Channel_ptr++;
		}
		else {
			Channel_idx = Channel_id[CID];
		}
		f_channel[Channel_idx].push_back(User_idx);
	}

}

int offerNews(int mTime, int mNewsID, int mDelay, int mChannelID)
{
	int NID = News_ptr;
	int CID = Channel_id[mChannelID];
	News_id[mNewsID] = News_ptr++;
	newss[NID] = true;
	News* A = &Newspool[NID];
	A->set(mTime + mDelay, NID, CID, mNewsID);
	pq.push(A);

	//cout << f_channel[CID].size() << endl;

	return f_channel[CID].size();
}

void cancelNews(int mTime, int mNewsID)
{
	int NID = News_id[mNewsID];
	newss[NID] = false;
}



int checkUser(int mTime, int mUID, int mRetIDs[])
{
	send_News(mTime);

	int UID = User_id[mUID];
	int cnt = 0;

	for (int i = Upq[UID].size() - 1; i >= 0; i--) {
		News* B = Upq[UID][i];
		if (newss[B->mNewsID] == false)	continue;
		if (cnt < 3) {
			mRetIDs[cnt] = B->oNewsID;
			//cout << B.oNewsID << " ";
		}
		cnt++;
	}
	Upq[UID].clear();


	//priority_queue<News, vector<News>, compare2> empty;
	//swap(Upq[UID], empty);

	//cout << cnt << endl;
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