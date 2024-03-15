#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

#include <cstring>
#include <iostream>
#include <set>
#include <unordered_map>
#include <queue>
#include <bitset>

using namespace std;

typedef long long ll;

ll char_to_int(char a) {
	return a - 'a' + 1;
}

class Word {
public:
	int N;
	int M = 3;
	int K;
	int idx;
	int word_idx;
	ll name;

	string word;

	ll fow;
	ll mid;
	ll end;

	int fow_ptr = -1;
	int mid_ptr = -1;
	int end_ptr = -1;

	int rm = 0;

	Word() {}

	void reset() {
		fow_ptr = -1;
		mid_ptr = -1;
		end_ptr = -1;
		rm = 0;
		name = 0;
	}

	void set_word(char Word[], int DirLen[]) {
		word = string(Word);

		N = DirLen[0];
		ll names = 0;
		for (int i = 0; i < N; i++) {
			names <<= 5;
			names += char_to_int(word[i]);
		}
		fow = names;

		M = 3;
		names = 0;
		for (int i = 4; i < 7; i++) {
			names <<= 5;
			names += char_to_int(word[i]);
		}
		mid = names;

		K = DirLen[2];
		names = 0;
		for (int i = 11 - K; i < 11; i++) {
			names <<= 5;
			names += char_to_int(word[i]);
		}
		end = names;
	}
};

Word words[33001];
int word_ptr;
int now_ptr;

struct compare {
	bool operator()(const Word* A, const Word* B) const {
		return A->word > B->word;
	}
};

unordered_map<ll, priority_queue<Word*, vector<Word*>, compare>> fow_map;
unordered_map<ll, priority_queue<Word*, vector<Word*>, compare>> mid_map;
unordered_map<ll, priority_queue<Word*, vector<Word*>, compare>> end_map;
unordered_map<ll, int> word_id_map;

void init()
{
	fow_map.clear(); mid_map.clear(); end_map.clear(); word_id_map.clear();
	fow_map.reserve(30000);
	mid_map.reserve(30000);
	end_map.reserve(30000);
	word_id_map.reserve(30000);
	word_ptr = 1;
	now_ptr = 1;
}

inline void add_map(Word* W) {

	char S[12];
	strcpy(S, W->word.c_str());

	// for
	ll name = 0;
	for (int i = 0; i < 2; i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	fow_map[name].push(W);

	name = 0;
	for (int i = 0; i < 4; i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	fow_map[name].push(W);

	name = 0;
	for (int i = 4; i < 7; i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	mid_map[name].push(W);
	
	name = 0;
	for (int i = 9; i < 11; i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	end_map[name].push(W);

	name = 0;
	for (int i = 7; i < 11; i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	end_map[name].push(W);
}

void addRoom(int mID, char mWord[], int mDirLen[])
{
	int idx = mID;

	Word* nword = &words[word_ptr++];
	nword->reset();
	nword->idx = idx;
	nword->word_idx = idx;
	nword->set_word(mWord, mDirLen);
	
	ll name = 0;
	for (int i = 0; i < nword->word.size(); i++) {
		name <<= 5;
		name += char_to_int(nword->word[i]);
	}
	word_id_map[name] = word_ptr-1;
	nword->name = name;

	add_map(nword);
}

inline void set_ptr(Word* W, int dir) {

	priority_queue<Word*, vector<Word*>, compare>* ptr;
	vector<Word*> temp;
	if (dir == 0) {
		ptr = &end_map[W->fow];
		int find = 0;
		while (!ptr->empty()) {
			if (ptr->empty())	break;
			auto it = ptr->top();
			ptr->pop();
			if (it->rm)	continue;
			temp.push_back(it);
			string str = (it)->word;
			if (it->name == W->name) {
				continue;
			}
			W->fow_ptr = word_id_map[it->name];
			find = 1;
			break;
		}
		if (!find) {
			W->fow_ptr = -1;
		}

	}

	else if (dir == 1) {
		ptr = &mid_map[W->mid];
		int find = 0;
		while (!ptr->empty()) {
			if (ptr->empty())	break;
			auto it = ptr->top();
			ptr->pop();
			if (it->rm)	continue;
			temp.push_back(it);
			string str = (it)->word;
			if (it->name == W->name) {
				continue;
			}
			W->mid_ptr = word_id_map[it->name];
			find = 1;
			break;
		}
		if (!find) {
			W->mid_ptr = -1;
		}
	}
	else {
		ptr = &fow_map[W->end];
		int find = 0;
		while (!ptr->empty()) {
			if (ptr->empty())	break;
			auto it = ptr->top();
			ptr->pop();
			if (it->rm)	continue;
			temp.push_back(it);
			string str = (it)->word;
			if (it->name == W->name) {
				continue;
			}
			W->end_ptr = word_id_map[it->name];
			find = 1;
			break;
		}
		if (!find) {
			W->end_ptr = -1;
		}
	}
	for (int i = 0; i < temp.size(); i++) {
		ptr->push(temp[i]);
	
	}
}

void setCurrent(char mWord[])
{
	string S(mWord);
	ll name = 0;
	for (int i = 0; i < S.size(); i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	now_ptr = word_id_map[name];
}

int moveDir(int mDir)
{
	Word* nword = &words[now_ptr];
	set_ptr(nword, mDir);

	if (mDir == 0) {
		if (nword->fow_ptr == -1)	return 0;
		now_ptr = nword->fow_ptr;
	}
	else if (mDir == 1) {
		if (nword->mid_ptr == -1)	return 0;
		now_ptr = nword->mid_ptr;
	}
	else {
		if (nword->end_ptr == -1)	return 0;
		now_ptr = nword->end_ptr;
	}

	return words[now_ptr].idx;
}

inline void addRoom2(int mID, char mWord[], int mDirLen[])
{
	int idx = mID;
	int ori_idx = word_ptr;
	Word* nword = &words[word_ptr++];
	nword->reset();
	nword->idx = idx;
	nword->word_idx = ori_idx;
	nword->set_word(mWord, mDirLen);

	ll name = 0;
	for (int i = 0; i < nword->word.size(); i++) {
		name <<= 5;
		name += char_to_int(nword->word[i]);
	}
	word_id_map[name] = ori_idx;
	nword->name = name;
	add_map(nword);
}

void changeWord(char mWord[], char mChgWord[], int mChgLen[])
{
	string S(mWord);
	ll name = 0;
	for (int i = 0; i < S.size(); i++) {
		name <<= 5;
		name += char_to_int(S[i]);
	}
	int n_ptr = word_id_map[name];
	
	Word* nword = &words[n_ptr];
	int idx = nword->idx;
	nword->rm = 1;

	string E(mChgWord);
	addRoom2(idx, mChgWord, mChgLen);

	if (n_ptr == now_ptr) {
		now_ptr = word_ptr - 1;
	}
	
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