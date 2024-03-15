#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <bitset>

using namespace std;

long long ll_chr_to_int(char a) {
	return a - 'a' + 1LL;
}

int i_chr_to_int(char a) {
	return a - 'a' + 1;
}

struct Word {
	int time;
	long long word;
	int word_idx;
	string cword;

	Word() {}
	Word(int _time, long long _word, int _word_idx, string _cword) {
		time = _time;
		word = _word;
		word_idx = _word_idx;
		cword = _cword;
	}

};

struct compare {
	bool operator()(Word &a, Word &b) {
		if (a.time != b.time)
			return a.time < b.time; // 산을 올라간다 큰거부터 ㅇ아래로
		return a.cword > b.cword;
	}
};

struct Result
{
	int mOrder;
	int mRank;
};

class Node {
public:
	int chr;
	Node* child[27];
	int child_on[27];
	priority_queue<Word, vector<Word>, compare> pq;

	Node(){}
	void reset() {
		chr = 0;
		memset(child_on, 0, sizeof child_on);
		priority_queue<Word, vector<Word>, compare> temp;
		swap(temp, pq);
	}
};

Node nodes[110021];
int words_record[15002];
int words_relate[15002];
unordered_map<long long, int> words_record_id_map;
unordered_map<long long, string> ll_string;
vector<string> relation[102];

Node* root;
int node_ptr;
int word_ptr;
int relate_ptr;

void init()
{
	node_ptr = 0;
	word_ptr = 1;
	relate_ptr = 1;
	nodes[node_ptr++].reset();
	root = &nodes[0];

	memset(words_record, 0, sizeof words_record);
	memset(words_relate, 0, sizeof words_relate);
	words_record_id_map.clear();
	ll_string.clear();
	for (int i = 0; i < 102; i++) {
		relation[i].clear();
	}

	return;
}

long long make_word(char mStr[]) {
	long long word = 0;
	for (int i = 0; i < strlen(mStr); i++) {
		word = word << 5;
		word += ll_chr_to_int(mStr[i]);
	}
	return word;
}

int check_word_idx(long long word, char mStr[]) {
	int word_idx;
	if (words_record_id_map[word]) {
		word_idx = words_record_id_map[word];

	}
	else {
		word_idx = word_ptr;
		words_record_id_map[word] = word_ptr++;
		ll_string[word] = string(mStr);
	}
	return word_idx;
}


void search(char mStr[], int mCount)
{	
	long long word = 0;
	string s_word;
	int word_idx, time = 0, query;

	word = make_word(mStr);
	word_idx = check_word_idx(word, mStr);

	// relation이 없을 경우 처리
	if (words_relate[word_idx] == 0) {
		words_record[word_idx] += mCount;
		time = words_record[word_idx];
		Node* now = root;
		Word* B = new Word(time, word, word_idx, mStr);
		now->pq.push(*B);

		for (int i = 0; i < strlen(mStr); i++) {

			query = 0;
			query += i_chr_to_int(mStr[i]);
			if (now->child_on[query] == 0) {
				now->child_on[query] = 1;
				nodes[node_ptr].reset();
				now->child[query] = &nodes[node_ptr];
				node_ptr++;
			}
			now = now->child[query];
			Word* A = new Word(time, word,word_idx, mStr);
			now->pq.push(*A);
		}

	}
	else {
		int r_idx = words_relate[word_idx];
		word = 0;
		for (int i = 0; i < relation[r_idx].size(); i++) {
			char ch[8];
			s_word = relation[r_idx][i];
			strcpy(ch, s_word.c_str());
	
			word = make_word(ch);
			word_idx = words_record_id_map[word];
		
			words_record[word_idx] += mCount;
			time = words_record[word_idx];
			//cout << time << endl;

			Node* now = root;
			Word* B = new Word(time, word, word_idx,s_word);
			now->pq.push(*B);

			for (int i = 0; i < strlen(ch); i++) {

				query = 0;
				query += i_chr_to_int(ch[i]);
				if (now->child_on[query] == 0) {
					now->child_on[query] = 1;
					nodes[node_ptr].reset();
					now->child[query] = &nodes[node_ptr];
					node_ptr++;
				}
				now = now->child[query];
				Word* A = new Word(time, word, word_idx, s_word);
				now->pq.push(*A);
			}
		}
	}
	return;
}

Result recommend(char mStr[])
{
	long long word = 0;
	int word_idx, find = 0, query;
	Node* now = root;
	Result ret = { 0, 0 };

	word = make_word(mStr);
	word_idx = words_record_id_map[word];

	int idx = 0;
	vector<Word> word_vec;

	while (!now->pq.empty()) {
		Word A = now->pq.top();
		now->pq.pop();
		if (A.time != words_record[A.word_idx]) continue;
		word_vec.push_back(A);
		idx++;
		if (A.word == word) {
			find = 1;
			break;
		}
		if (idx == 5) break;
	}	

	for (int i = 0; i < idx; i++) {
		Word A(word_vec[i].time, word_vec[i].word, word_vec[i].word_idx, word_vec[i].cword);
		now->pq.push(A);
	}

	if (find) {
		ret.mOrder = 0;
		ret.mRank = idx;
		//cout << ret.mOrder << " " << ret.mRank << endl;
		search(mStr, 1);
		return ret;
	}

	for (int i = 0; i < strlen(mStr); i++) {

		query = 0;
		query += i_chr_to_int(mStr[i]);
		now = now->child[query];

		idx = 0;
		word_vec.clear();

		while (!now->pq.empty()) {
			Word A = now->pq.top();
			//cout << "A : " << bitset<64>(A.word) << endl;
			now->pq.pop();
			if (A.time != words_record[A.word_idx]) continue;
			word_vec.push_back(A);
			idx++;
			//cout << idx << endl;
			if (A.word == word) {
				find = 1;
				break;
			}
			if (idx == 5) break;
		}
		
		for (int i = 0; i < idx; i++) {
			Word A(word_vec[i].time, word_vec[i].word, word_vec[i].word_idx, word_vec[i].cword);
			now->pq.push(A);
		}

		if (find) {
			ret.mOrder = i + 1;
			ret.mRank = idx;
			//cout << ret.mOrder << " " << ret.mRank << endl;
			search(mStr, 1);
			return ret;
		}
	}
}

void change_rel(string S, int now_rel_ptr) {

	char ch[8];
	long long word = 0;
	int word_idx, query;
	strcpy(ch, S.c_str());
	word = make_word(ch);
	word_idx = words_record_id_map[word];
	words_relate[word_idx] = now_rel_ptr;
}


void after_rel(string S, int time) {

	char ch[8];
	long long word = 0;
	int word_idx, query;
	strcpy(ch, S.c_str());

	word = make_word(ch);
	word_idx = words_record_id_map[word];
	words_record[word_idx] = time;

	Node* now = root;
	Word* B = new Word(time, word, word_idx, S);
	now->pq.push(*B);

	for (int i = 0; i < strlen(ch); i++) {

		query = 0;
		query += i_chr_to_int(ch[i]);
		if (now->child_on[query] == 0) {
			now->child_on[query] = 1;
			nodes[node_ptr].reset();
			now->child[query] = &nodes[node_ptr];
			node_ptr++;
		}
		now = now->child[query];
		Word* A = new Word(time, word, word_idx, S);
		now->pq.push(*A);
	}
}

int relate(char mStr1[], char mStr2[])
{
	long long word1 = 0, word2 = 0;
	int word_idx1, word_idx2;
	int now_rel_ptr;
	int time = 0;

	word1 = make_word(mStr1);
	word2 = make_word(mStr2);
	
	word_idx1 = words_record_id_map[word1];
	word_idx2 = words_record_id_map[word2];

	if (words_relate[word_idx1] == 0 && words_relate[word_idx2] == 0) { // 둘다 관계 없는경우
		now_rel_ptr = relate_ptr++;
		words_relate[word_idx1] = now_rel_ptr;
		words_relate[word_idx2] = now_rel_ptr;
		relation[now_rel_ptr].push_back(string(mStr1));
		relation[now_rel_ptr].push_back(string(mStr2));		
	}
	else if (words_relate[word_idx1] && words_relate[word_idx2] == 0) { // 왼쪾 하나만 있는 경우
		now_rel_ptr = words_relate[word_idx1];
		words_relate[word_idx2] = now_rel_ptr;
		relation[now_rel_ptr].push_back(string(mStr2));
	}
	else if (words_relate[word_idx1] == 0 && words_relate[word_idx2]) { // 왼쪾 하나만 있는 경우
		now_rel_ptr = words_relate[word_idx2];
		words_relate[word_idx1] = now_rel_ptr;
		relation[now_rel_ptr].push_back(string(mStr1));
	}
	else {
		now_rel_ptr = words_relate[word_idx1];
		relation[words_relate[word_idx1]].insert(relation[words_relate[word_idx1]].end(), relation[words_relate[word_idx2]].begin(), relation[words_relate[word_idx2]].end());
		for (int i = 0; i < relation[now_rel_ptr].size(); i++) {
			change_rel(relation[now_rel_ptr][i], now_rel_ptr);
		}
		words_relate[word_idx2] = now_rel_ptr;
	}	
	time += words_record[word_idx1];
	time += words_record[word_idx2];
	for (int i = 0; i < relation[now_rel_ptr].size(); i++) {	
		after_rel(relation[now_rel_ptr][i], time);
	}
	//cout << words_record[word_idx1] << " " << words_record[word_idx2] << endl;
	return time;
	
}

void _rank(char mPrefix[], int mRank, char mReturnStr[])
{

	int query;
	long long word;
	Node* now = root;
	for (int i = 0; i < strlen(mPrefix); i++) {
		query = 0;
		query += i_chr_to_int(mPrefix[i]);
		now = now->child[query];
	}

	int idx = 0, find = 0;
	vector<Word> word_vec;

	while (!now->pq.empty()) {
		Word A = now->pq.top();
		
		now->pq.pop();
		if (A.time != words_record[A.word_idx]) continue;
		word_vec.push_back(A);
		idx++;
		if (idx == mRank) {
			find = 1;
			word = A.word;
			break;
		}
	}
	for (int i = 0; i < idx; i++) {
		Word A(word_vec[i].time, word_vec[i].word, word_vec[i].word_idx, word_vec[i].cword);
		now->pq.push(A);
	}

	strcpy(mReturnStr, ll_string[word].c_str());
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
			_rank(mStr, mRank, mReturnStr);
			scanf("%s", mStr2);
			if (strncmp(mStr2, mReturnStr, MAX_LENGTH) != 0)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
		if (okay == false) {
			cout << cmd << endl;
			okay = true;
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

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}