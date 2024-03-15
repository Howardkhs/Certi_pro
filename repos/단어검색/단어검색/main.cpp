#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>



/// ///////////////////////////////////////////////

#include <memory.h>
#include <cstring>
#include <iostream>

using namespace std;

int char_to_int(char a) {
	return a - 'a' + 1;
}

class Node {
public:
	int cnt = 0;
	Node* childs[27];
	Node(){}
	void reset() {
		cnt = 0;
		memset(childs, 0, sizeof childs);
	}

	void add_word(int a) {

	}
};

Node nodes[601000];

int node_ptr = 1;
Node* root = &nodes[1];
int rm_cnt;
int s_cnt;

void init() {
	node_ptr = 2;
	root->reset();
	return;
}

int add(char str[]) {
	
	int len = strlen(str);

	Node* N = root;

	for (int i = 0; i < len; i++) {
		int name = char_to_int(str[i]);
		if (N->childs[name]) {
			N = N->childs[name];
		}
		else {
			Node* T = &nodes[node_ptr++];
			T->reset();
			N->childs[name] = T;
			N = T;
		}
	}

	N->cnt++;

	return N->cnt;
}

void rm(Node* N, char str[], int idx) {

	int len = strlen(str);

	if (len == idx) {
		rm_cnt += N->cnt;
		N->cnt = 0;
		return;
	}

	if (str[idx] == '?') {
		for (int i = 0; i < 27; i++) {
			if (N->childs[i]) {
				rm(N->childs[i], str, idx + 1);
			}
		}
	}
	else {
		int name = char_to_int(str[idx]);
		if (N->childs[name]) {
			rm(N->childs[name], str, idx + 1);
		}
		else {
			return;
		}
	}
	
}

int remove(char str[]) {

	Node* N = root;
	rm_cnt = 0;

	rm(N, str, 0);
	return rm_cnt;
}

void se(Node* N, char str[], int idx) {

	int len = strlen(str);


	if (len == idx) {
		s_cnt += N->cnt;
		return;
	}

	if (str[idx] == '?') {
		for (int i = 0; i < 27; i++) {
			if (N->childs[i]) {
				se(N->childs[i], str, idx + 1);
			}
		}
	}
	else {
		int name = char_to_int(str[idx]);
		if (N->childs[name]) {
			se(N->childs[name], str, idx + 1);
		}
		else {
			return;
		}
	}
	
}

int search(char str[]) {

	Node* N = root;
	s_cnt = 0;

	se(N, str, 0);
	return s_cnt;
}

/////////////////////////////////////////////////////////////////////////

#define MAX_LEN 30
#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_SEARCH 4

static bool run() {
	int q;
	scanf("%d", &q);

	char str[MAX_LEN + 1];
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			init();
			okay = true;
			break;
		case CMD_ADD:
			scanf("%s %d", str, &ans);
			ret = add(str);
			if (ans != ret)
				okay = false;
			break;
		case CMD_REMOVE:
			scanf("%s %d", str, &ans);
			ret = remove(str);
			if (ans != ret)
				okay = false;
			break;
		case CMD_SEARCH:
			scanf("%s %d", str, &ans);
			ret = search(str);
			if (ans != ret)
				okay = false;
			break;
		default:
			okay = false;
			break;
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