#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif

#include<stdio.h>

#define CMD_MKDIR 1
#define CMD_RM 2
#define CMD_CP 3
#define CMD_MV 4
#define CMD_FIND 5

#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999

#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999


#include <set>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void str_cpy(char buf[], int start, int end, char path[PATH_MAXLEN + 1]) {
	
	int cnt = 0;
	for (int i = start; i < end; i++) {
		buf[cnt] = path[i];
		cnt++;
	}
	buf[cnt] = '\0';
}

class Node {
public:

	int cnt = 0;
	int name;
	Node* parent;
	
	vector<Node*> childs;
	
	Node(){}
	void reset() {
		cnt = 0; name = 0; parent = nullptr; childs.clear();  childs.reserve(5);
	}
	void make(int _name) {
		name = _name;
	}
};

bool compare(const Node* A, const Node* B) {
	return A->name < B->name;
};

Node nodes[50010];
Node* root;
int node_ptr = 1;

void init(int n) {

	node_ptr = 1;
	root = &nodes[node_ptr++];
	root->reset();
}

int make_name(char buf[]) {
	int name = 0;
	int len = strlen(buf);

	for (int i = 0; i < len; i++) {
		int a = buf[i] - 'a' + 1;
		name = name << 5;
		name += a;
	}
	return name;
}

Node* find_path(char path[PATH_MAXLEN + 1]) {
	Node* N = root;
	int len = strlen(path);

	int start; int end;
	char buf[7];

	for (int i = 1; i < len; i++) {

		if (path[i - 1] == '/') {
			start = i;
			continue;
		}

		if (path[i] == '/') {
			end = i;
			str_cpy(buf, start, end, path);

			Node T;	T.name = make_name(buf);  // 있는 놈중에 찾으려고 이렇게 한거고
			auto it = upper_bound(N->childs.begin(), N->childs.end(), &T, compare);
			N = *(--it);
			continue;
		}	
	}

	return N;
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {

	Node* N = find_path(path);
	
	Node* C = &nodes[node_ptr++];
	C->reset();	C->make(make_name(name));	C->parent = N;
	auto it = upper_bound(N->childs.begin(), N->childs.end(), C, compare);
	N->childs.insert(it, C); // 없는 놈중에 찾는거
	N->cnt += 1;
	while (N->parent) {
		N = N->parent;
		N->cnt += 1;
	}
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {

	Node* N = find_path(path);
	Node* P = N->parent;

	int cnt = N->cnt + 1;

	auto it = upper_bound(P->childs.begin(), P->childs.end(), N, compare);
	it--;
	P->childs.erase(it);
	N->parent = nullptr;

	P->cnt -= cnt;
	while (P->parent) {
		P = P->parent;
		P->cnt -= cnt;
	}
}

Node* cmd_rm2(char path[PATH_MAXLEN + 1]) {

	Node* N = find_path(path);
	Node* P = N->parent;

	int cnt = N->cnt + 1;

	auto it = upper_bound(P->childs.begin(), P->childs.end(), N, compare);
	it--;
	P->childs.erase(it);
	N->parent = nullptr;

	P->cnt -= cnt;
	while (P->parent) {
		P = P->parent;
		P->cnt -= cnt;
	}
	return N;
}

Node* copy(Node* O) {
	Node* N = &nodes[node_ptr++];
	N->reset();
	N->cnt = O->cnt;
	N->name = O->name;

	for (auto p = O->childs.begin(); p != O->childs.end(); p++) {
		Node* C = copy(*p);
		auto it = upper_bound(N->childs.begin(), N->childs.end(), C, compare);
		N->childs.insert(it, C);
		C->parent = N;
	}
	return N;
}


void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {

	Node* S = find_path(srcPath);
	Node* D = find_path(dstPath);

	Node* N = copy(S);

	auto it = upper_bound(D->childs.begin(), D->childs.end(), N, compare);
	D->childs.insert(it, N); // 없는 놈중에 찾는거
	N->parent = D;

	int cnt = N->cnt + 1;

 	D->cnt += cnt;
	while (D->parent) {
		D = D->parent;
		D->cnt += cnt;
	}
}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {

	Node* S = cmd_rm2(srcPath);
	Node* D = find_path(dstPath);

	auto it = upper_bound(D->childs.begin(), D->childs.end(), S, compare);
	
	D->childs.insert(it, S); // 없는 놈중에 찾는거
	S->parent = D;

	int cnt = S->cnt + 1;

	D->cnt += cnt;
	while (D->parent) {
		D = D->parent;
		D->cnt += cnt;
	}
}

int cmd_find(char path[PATH_MAXLEN + 1]) {
	Node* N = find_path(path);

	//cout << N->cnt << endl;
	return N->cnt;
}

static bool run(int m) {

	bool isAccepted = true;
	int cmd;
	char name[NAME_MAXLEN + 1];
	char path1[PATH_MAXLEN + 1], path2[PATH_MAXLEN + 1];

	while (m--) {

		scanf("%d", &cmd);

		if (cmd == CMD_MKDIR) {
			scanf("%s%s", path1, name);
			cmd_mkdir(path1, name);
		}
		else if (cmd == CMD_RM) {
			scanf("%s", path1);
			cmd_rm(path1);
		}
		else if (cmd == CMD_CP) {
			scanf("%s%s", path1, path2);
			cmd_cp(path1, path2);
		}
		else if (cmd == CMD_MV) {
			scanf("%s%s", path1, path2);
			cmd_mv(path1, path2);
		}
		else {
			int ret;
			int answer;

			scanf("%s", path1);
			ret = cmd_find(path1);
			scanf("%d", &answer);

			isAccepted &= (ret == answer);
		}
		//print(root); cout << endl;
	}

	return isAccepted;
}

int main(void) {

	int test, T;
	int n, m;

	freopen("sample_input.txt", "r", stdin);

	setbuf(stdout, NULL);

	scanf("%d", &T);

	for (test = 1; test <= T; ++test) {

		scanf("%d%d", &n, &m);

		init(n);

		if (run(m)) {
			printf("#%d 100\n", test);
		}
		else {
			printf("#%d 0\n", test);
		}
	}

	return 0;
}