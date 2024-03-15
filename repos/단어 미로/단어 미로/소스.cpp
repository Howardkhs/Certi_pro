#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <memory.h>
#include <cstring>
#include <set>
#include <unordered_map>
#include <string>

using namespace std;

class Word {
public:
	string word;
	int N;
	int M;
	int K;
	int id;
	Word(){}
	Word(string _word, int _N, int _M, int _K, int _id) {
		word = _word; N = _N; M = _M; K = _K; id = _id;
	}
	void word_set(string _word, int _N, int _M, int _K, int _id) {
		word = _word; N = _N; M = _M; K = _K; id = _id;
	}
};

Word words[30001];
int word_ptr;

struct compare {
	bool operator()(const Word* A, const Word* B) const {
		return A->word < B->word;
	}
};

set<Word*, compare> front[66002];
set<Word*, compare> mid[66002];
set<Word*, compare> back[66002];

unordered_map<string, int> word_id;
unordered_map<string, int> front_id;
unordered_map<string, int> mid_id;
unordered_map<string, int> back_id;

int front_ptr; int mid_ptr; int back_ptr;
Word* cur; 

void init()
{
	for (int i = 0; i < 30001; i++) {
		words[i].word.clear();
	}
	word_ptr = 1;

	for (int i = 0; i < 66002; i++) {
		front[i].clear(); mid[i].clear(); back[i].clear();
	}
	
	word_id.clear(); front_id.clear(); mid_id.clear(); back_id.clear();
	front_ptr = 1; mid_ptr = 1; back_ptr = 1;
	cur = nullptr;
}


int make_front_mid_back(string sliced_word, int idx) { //idx 1 front 2 mid 3 back
	int id;
	if (idx == 0) {
		id = front_id[sliced_word];
		if (id == 0) {
			id = front_ptr++;
			front_id[sliced_word] = id;
		}
	}
	else if (idx == 1) {
		id = mid_id[sliced_word];
		if (id == 0) {
			id = mid_ptr++;
			mid_id[sliced_word] = id;
		}
	}
	else  {
		id = back_id[sliced_word];
		if (id == 0) {
			id = back_ptr++;
			back_id[sliced_word] = id;
		}
	}
	return id;
}

void addRoom(int mID, char mWord[], int mDirLen[])
{
	word_id[mWord] = mID;
	Word* N = &words[mID];
	int n = mDirLen[0]; int m = mDirLen[1]; int k = mDirLen[2];
	N->word_set(mWord, n, m, k, mID);
	
	int f_id = make_front_mid_back(N->word.substr(0, 2), 0);
	front[f_id].insert(N);
	f_id = make_front_mid_back(N->word.substr(0, 4), 0);
	front[f_id].insert(N);

	int m_id = make_front_mid_back(N->word.substr(4, 3), 1);
	mid[m_id].insert(N);

	int b_id = make_front_mid_back(N->word.substr(7, 4), 2);
	back[b_id].insert(N);
	b_id = make_front_mid_back(N->word.substr(9, 2), 2);
	back[b_id].insert(N);
}

void setCurrent(char mWord[])
{
	int wid = word_id[mWord];
	cur = &words[wid];
}

int moveDir(int mDir)
{
	int id;
	set<Word*, compare>* set_ptr;

	if (mDir == 0) {
		id = back_id[cur->word.substr(0, cur->N)];
		set_ptr = &back[id];
		auto p = set_ptr->begin();
		while (1) {
			if (p == set_ptr->end())	return 0;
			if (*p == cur) {
				p++;	continue;
			}
			cur = *p;
			return cur->id;
		}
	}
	else if (mDir == 1) {
		id = mid_id[cur->word.substr(4, 3)];
		set_ptr = &mid[id];

		auto p = set_ptr->begin();
		while (1) {
			if (p == set_ptr->end())	return 0;
			if (*p == cur) {
				p++;	continue;
			}
			cur = *p;
			return cur->id;
		}
	}
	else {
		id = front_id[cur->word.substr(11 - cur->K, cur->K)];
		set_ptr = &front[id];

		auto p = set_ptr->begin();
		while (1) {
			if (p == set_ptr->end())	return 0;
			if (*p == cur) {
				p++;	continue;
			}
			cur = *p;
			return cur->id;
		}
	}
}

void changeWord(char mWord[], char mChgWord[], int mChgLen[])
{
	int wid = word_id[mWord];
	Word* N = &words[wid];
	word_id[mWord] = 0;
	word_id[mChgWord] = wid;

	int f_id = make_front_mid_back(N->word.substr(0, 2), 0);
	front[f_id].erase(N);
	f_id = make_front_mid_back(N->word.substr(0, 4), 0);
	front[f_id].erase(N);

	int m_id = make_front_mid_back(N->word.substr(4, 3), 1);
	mid[m_id].erase(N);

	int b_id = make_front_mid_back(N->word.substr(7, 4), 2);
	back[b_id].erase(N);
	b_id = make_front_mid_back(N->word.substr(9, 2), 2);
	back[b_id].erase(N);

	int n = mChgLen[0]; int m = mChgLen[1]; int k = mChgLen[2];
	N->word_set(mChgWord, n, m, k, N->id);

	f_id = make_front_mid_back(N->word.substr(0, 2), 0);
	front[f_id].insert(N);
	f_id = make_front_mid_back(N->word.substr(0, 4), 0);
	front[f_id].insert(N);

	m_id = make_front_mid_back(N->word.substr(4, 3), 1);
	mid[m_id].insert(N);

	b_id = make_front_mid_back(N->word.substr(7, 4), 2);
	back[b_id].insert(N);
	b_id = make_front_mid_back(N->word.substr(9, 2), 2);
	back[b_id].insert(N);
}
/////////////////////////////////////////////////////////////////////////

#define INIT	0
#define ADD		1
#define SET		2
#define MOVE	3
#define CHANGE	4

#define MAX_LENGTH	(11 + 1)

static bool run()
{
	int cmd, ans, ret, mid;
	int dir[3];
	char mWord[MAX_LENGTH];
	char mRetWord[MAX_LENGTH];

	int Q = 0;
	bool okay = false;

	scanf("%d", &Q);
	for (int q = 0; q < Q; ++q)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case INIT:
			init();
			okay = true;
			break;

		case ADD:
			scanf("%d %s %d %d %d", &mid, mWord, &dir[0], &dir[1], &dir[2]);
			addRoom(mid, mWord, dir);
			break;

		case SET:
			scanf("%s", mWord);
			setCurrent(mWord);
			break;

		case MOVE:
			scanf("%d %d", &dir[0], &ans);
			ret = moveDir(dir[0]);
			if (ret != ans) {
				okay = false;
			}
			break;

		case CHANGE:
			scanf("%s %s %d %d %d", mWord, mRetWord, &dir[0], &dir[1], &dir[2]);
			changeWord(mWord, mRetWord, dir);
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