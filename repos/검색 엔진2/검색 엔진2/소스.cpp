#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

#include <string>
#include <memory.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <set>
#include <ctime>

using namespace std;

int char_to_int(char a) {
	return a - 'a' + 1;
}

struct Result
{
	int mOrder;
	int mRank;
};

class Word {
public:
	string word;
	int id; // 진위 여부 파단
	int cnt; // cnt도 안같으면 가짜 만들어서 넣을때 부모의 cnt를 넣으면 되나?
	Word* parent;  // 부모가 안같으면 가짜
	vector<Word*> childs;

	Word() {}
	Word(string _word, int _id, int _cnt) {
		word = _word;	id = _id; cnt = _cnt;
	}
	void set(string _word, int _id) {
		word = _word;	id = _id;
	}
	void reset() {
		word.clear(); id = 0; cnt = 0;  parent = nullptr; childs.clear();
	}

	Word* find_parent() {
		if (parent)
			return parent = parent->find_parent();
		return this;
	}
};

struct compare {
	bool operator()(const Word& A,const Word& B) const{
		if (A.cnt != B.cnt)
			return A.cnt > B.cnt;
		return A.word < B.word;
	}
};

class Node {
public:
	set<Word, compare> pq;
	Node* childs[27] = { nullptr, };
	Node() {}
	void reset() {
		pq.clear();
		memset(childs, 0, sizeof childs);
	}
};

Word words[15001];
Node nodes[15001 * 7];
unordered_map<string, int> word_id;

int word_ptr;
int node_ptr;
Node* root;

void init()
{
	for (int i = 0; i < 15001; i++) {
		words[i].reset();
	}

	word_id.clear();

	word_ptr = 1;
	node_ptr = 1;
	root = &nodes[node_ptr++];
	root->reset();
	return;
}

int get_id(char mStr[]) {
	int id = word_id[mStr];
	if (id == 0) {
		id = word_ptr;
		word_id[mStr] = word_ptr++;
	}
	return id;
}

void set_word(Word* word_ptr, char mStr[], int id, int mCount) {
	if (word_ptr->id == 0) {
		word_ptr->set(string(mStr), id);
	}

	Word* P = word_ptr->find_parent(); // 부모를 찾아서 cnt올려주고
	P->cnt += mCount;
}

void push_pq(Word* word_ptr, string mStr) {
	Node* N = root;
	Word* P = word_ptr->find_parent();
	N->pq.insert({ word_ptr->word, word_ptr->id, P->cnt });

	for (int i = 0; i < mStr.length(); i++) {
		int a = char_to_int(mStr[i]);

		Node* C = N->childs[a];
		if (!C) {
			C = &nodes[node_ptr++];
			C->reset();
			N->childs[a] = C;
		}
		N = C;
		N->pq.insert({ word_ptr->word, word_ptr->id, P->cnt });
	}
}

void search(char mStr[], int mCount)
{
	int id = get_id(mStr);

	Word* word_ptr = &words[id];
	set_word(word_ptr, mStr, id, mCount);
	Word* P = word_ptr->find_parent();

	push_pq(P, P->word);

	for (int i = 0; i < P->childs.size(); i++) {
		push_pq(P->childs[i], P->childs[i]->word);
	}

	return;
}

Result recommend(char mStr[])
{
	
	Node* N = root;
	Result ret = { 0, 0 };

	string name(mStr);

	int cnt = 0;
	int find = 0;
	int i;

	auto p = N->pq.begin();
	while(1){
		Word A = *p;
		int cid = A.id;

		Word* C = &words[cid];
		Word* P = C->find_parent();
		int pid = P->id;
		if (words[pid].cnt != A.cnt) {
			p++;
			continue; 
		}
		cnt++;
		
		if (A.word == name) {
			find = 1;
			N->pq.erase(p);
			break;
		}
		p++;
		if (cnt == 5)	break;
	}

	if (find) {
		search(mStr, 1);
		ret.mOrder = 0; ret.mRank = cnt;
		return ret;
	}


	for (i = 0; i < strlen(mStr); i++) {
		int a = char_to_int(mStr[i]);

		N = N->childs[a];
		cnt = 0;
		auto p = N->pq.begin();
		while (1) {
	
			Word A = *p;
			int cid = A.id;

			Word* C = &words[cid];
			Word* P = C->find_parent();
			int pid = P->id;

			if (words[pid].cnt != A.cnt) { 
				p++;
				continue; 
			}

			cnt++;
			
			if (A.word == name) {
				find = 1;
				N->pq.erase(p);
				break;
			}
			p++;
			if (cnt == 5)	break;
		}
		if (find) {
			search(mStr, 1);
			ret.mOrder = i + 1; ret.mRank = cnt;
			return ret;
		}
	}
}

int relate(char mStr1[], char mStr2[])
{
	int id1 = get_id(mStr1);
	int id2 = get_id(mStr2);

	if (id1 > id2)	swap(id1, id2);

	Word* W1 = &words[id1]; Word* W2 = &words[id2];
	W1 = W1->find_parent(); W2 = W2->find_parent();

	W1->cnt += W2->cnt;


	for (int i = 0; i < W2->childs.size(); i++) {
		Word* N = W2->childs[i];
		W1->childs.push_back(N);
	}
	W1->childs.push_back(W2);
	W2->parent = W1;

	W2->childs.clear();

	push_pq(W1, W1->word);
	for (int i = 0; i < W1->childs.size(); i++) {
		push_pq(W1->childs[i], W1->childs[i]->word);
	}

	return W1->cnt;
}

void ranks(char mPrefix[], int mRank, char mReturnStr[])
{
	Node* N = root;
	for (int i = 0; i < strlen(mPrefix); i++) {
		int a = char_to_int(mPrefix[i]);
		N = N->childs[a];
	}
	int cnt = 0;
	auto p = N->pq.begin();
	while (1) {
		Word A = *p;
		int cid = A.id;

		Word* C = &words[cid];
		Word* P = C->find_parent();
		int pid = P->id;

		if (words[pid].cnt != A.cnt) {
			p++;
			continue;
		}

		cnt++;
		if (cnt == mRank)	break;
		p++;
	}

	strcpy(mReturnStr, p->word.c_str());

	return;
}

#define MAX_LENGTH      (7 + 1)
#define CMD_INIT		(100)
#define CMD_SEARCH		(200)
#define CMD_RECOMMEND	(300)
#define CMD_RELATE		(400)
#define CMD_RANK		(500)

static bool run()
{
	int query_num;
	bool okay = false;

	scanf("%d", &query_num);

	for (int q = 0; q < query_num; ++q) {
		int cmd, ret, ans, ans2, mCount, mRank;
		char mStr[MAX_LENGTH], mStr2[MAX_LENGTH], mReturnStr[MAX_LENGTH];
		Result res;

		scanf("%d", &cmd);

		switch (cmd) {
		case CMD_INIT:
			init();
			okay = true;
			break;
		case CMD_SEARCH:
			scanf("%s %d", mStr, &mCount);
			search(mStr, mCount);
			break;
		case CMD_RECOMMEND:
			scanf("%s", mStr);
			res = recommend(mStr);
			scanf("%d %d", &ans, &ans2);
			if (res.mOrder != ans || res.mRank != ans2)
				okay = false;
			break;
		case CMD_RELATE:
			scanf("%s %s", mStr, mStr2);
			ret = relate(mStr, mStr2);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_RANK:
			scanf("%s %d", mStr, &mRank);
			ranks(mStr, mRank, mReturnStr);
			scanf("%s", mStr2);
			if (strncmp(mStr2, mReturnStr, MAX_LENGTH) != 0)
				okay = false;
			break;
	
		}
		
	
		
	}

	return okay;
}

int main()
{
	clock_t start, finish;
	double duration;
	start = clock();
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << duration << "초" << endl;
	return 0;
}