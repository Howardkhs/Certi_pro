#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT 100
#define CMD_DESTROY 200
#define CMD_ALLY 300
#define CMD_ATTACK 400
#define CMD_RECRUIT 500

/////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <set>
#include <unordered_map>

using namespace std;

class Node {
public:
	string name;
	int x;
	int y;
	int soldier;
	Node* parent;
	set<Node*> enemies;
	Node(){}
	void node_reset(string _name, int _x, int _y, int _soldier) {
		 parent = nullptr; enemies.clear();
		 name = _name; x = _x; y = _y; soldier = _soldier;
	}
	Node* find_parent() {
		if (parent)
			return parent = parent->find_parent();
		return this;
	}
};

Node nodes[9000];
unordered_map<string, Node*> name_pos;
int node_ptr;
int n;
Node* graph[25][25];

void init(int _N, int mSoldier[25][25], char mMonarch[25][25][11])
{
	n = _N;
	name_pos.clear();
	node_ptr = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Node* N = &nodes[node_ptr++];
			N->node_reset(mMonarch[i][j], i, j, mSoldier[i][j]);
			name_pos[N->name] = N;
			graph[i][j] = N;
		}
	}
}

void destroy()
{

}

void merge(Node* A, Node* B) {

	for (auto p = B->enemies.begin(); p != B->enemies.end(); p++) {
		Node* E = (*p)->find_parent();
		E->enemies.insert(A); // 적의 부모에게 적을 추가
	}

	A->enemies.insert(B->enemies.begin(), B->enemies.end());
	B->parent = A;
	B->enemies.clear();
}

int ally(char mMonarchA[11], char mMonarchB[11])
{
	Node* A = name_pos[mMonarchA]; Node* B = name_pos[mMonarchB];
	Node* P_A = A->find_parent();  Node* P_B = B->find_parent();

	if (P_A == P_B)	return -1;  // 동일하거나 동맹
	
	auto it = P_A->enemies.find(P_B);
	if (it != P_A->enemies.end()) { 
		return -2; 
	} // 적 최상위 노드로만 적을 판단

	merge(P_A, P_B);
	return 1;
}

int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dy[8] = { -1,  0,  1,-1, 1,-1, 0, 1 };

int do_attack(vector<Node*>* A_allies, vector<Node*>* B_allies, Node* B, Node* P_A, Node* P_B, char mGeneral[11]) {

	int A_soldiers = 0; int B_soldiers = B->soldier;
	
	P_B->enemies.insert(P_A);
	P_A->enemies.insert(P_B);

	for (int i = 0; i < B_allies->size(); i++) {
		int soldier = (*B_allies)[i]->soldier / 2;
		(*B_allies)[i]->soldier -= soldier;
		B_soldiers += soldier;
	}

	for (int i = 0; i < A_allies->size(); i++) {
		int soldier = (*A_allies)[i]->soldier / 2;
		(*A_allies)[i]->soldier -= soldier;
		A_soldiers += soldier;
	}
	if (B_soldiers >= A_soldiers) { // 방어 성공
		B->soldier = B_soldiers - A_soldiers;
		return 0; 
	}

	Node* N = &nodes[node_ptr++];
	N->node_reset(mGeneral, B->x, B->y, A_soldiers - B_soldiers);
	graph[N->x][N->y] = N;
	N->parent = P_A;
	name_pos[N->name] = N;
	for (auto p = P_A->enemies.begin(); p != P_A->enemies.end(); p++) {
		Node* E = (*p)->find_parent();
		E->enemies.insert(N); // 적의 부모에게 적을 추가
	}
	return 1;
}

int attack(char mMonarchA[11], char mMonarchB[11], char mGeneral[11])
{
	Node* A = name_pos[mMonarchA]; Node* B = name_pos[mMonarchB];
	Node* P_A = A->find_parent();  Node* P_B = B->find_parent();

	if (P_A == P_B) { 
		return -1; 
	}

	int x = B->x; int y = B->y;
	vector<Node*> A_allies;
	vector<Node*> B_allies;

	for (int i = 0; i < 8; i++) {
		int nx = x + dx[i]; int ny = y + dy[i];
		if (nx < 0 || ny < 0 || nx >= n || ny >= n)	continue;
		Node* N = graph[nx][ny];
		Node* P_N = N->find_parent();
		if (P_N == P_A)	A_allies.push_back(N);
		if (P_N == P_B)	B_allies.push_back(N);
	}
	if (A_allies.empty()) { 
		return -2; 
	}

	int answer = do_attack(&A_allies, &B_allies, B, P_A, P_B, mGeneral);
	return answer;
}

int recruit(char mMonarch[11], int mNum, int mOption)
{
	if (mOption == 0) {
		Node* N = name_pos[mMonarch];
		N->soldier += mNum;
		return N->soldier;
	}

	Node* P = name_pos[mMonarch]->find_parent();
	int answer = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Node* N = graph[i][j];
			if (N->find_parent() == P) {
				N->soldier += mNum;
				answer += N->soldier;
			}
		}
	}
	return answer;

}


/////////////////////////////////////////////////////////////////////////

#define MAX_N 25
#define MAX_L 10

static int Sol[MAX_N][MAX_N];
static char General[MAX_N][MAX_N][MAX_L + 1];

static int run()
{
	int isOK = 0;

	int N;
	int cmd;
	int result;
	int check;

	int mN;
	char mMonarchA[11];
	char mMonarchB[11];
	char mGeneral[11];
	int mOption;
	int num;

	scanf("%d", &N);

	for (int c = 0; c < N; ++c)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &mN);
			for (int j = 0; j < mN; j++)
				for (int i = 0; i < mN; i++)
					scanf("%d", &Sol[j][i]);

			for (int j = 0; j < mN; j++)
				for (int i = 0; i < mN; i++)
					scanf("%s", General[j][i]);

			init(mN, Sol, General);
			isOK = 1;
			break;

		case CMD_ALLY:
			scanf("%s %s", mMonarchA, mMonarchB);
			result = ally(mMonarchA, mMonarchB);
			scanf("%d", &check);
			if (result != check)
				isOK = 0;
			break;

		case CMD_ATTACK:
			scanf("%s %s %s", mMonarchA, mMonarchB, mGeneral);
			result = attack(mMonarchA, mMonarchB, mGeneral);
			scanf("%d", &check);
			if (result != check)
				isOK = 0;
			break;

		case CMD_RECRUIT:
			scanf("%s %d %d", mMonarchA, &num, &mOption);
			result = recruit(mMonarchA, num, mOption);
			scanf("%d", &check);
			if (result != check)
				isOK = 0;
			break;

		default:
			isOK = 0;
			break;
		}
	}

	destroy();

	return isOK;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		if (run()) printf("#%d %d\n", tc, MARK);
		else printf("#%d %d\n", tc, 0);
	}
	return 0;
}