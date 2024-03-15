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

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>

using namespace std;
struct P {
	int i;
	int j;
	P(int _i, int _j) {
		i = _i; j = _j;
	}
	P(){}
};

class Country;

struct compare {

	bool operator()(const Country* A, const Country* B) const {
		return A > B;
	}
};

class Country {
public:
	string name;
	int soldiers;

	P pos;
	Country* parent;
	set<Country*, compare> childs;  // 시간 날때 set로
	set<Country*, compare> enemies;
	Country(){}
	void reset(string _name, int _soldiers, int i, int j) {
		name = _name; soldiers = _soldiers;	
		parent = nullptr; childs.clear(); enemies.clear();
		pos.i = i; pos.j = j;
	}
	Country* find_parent() {
		if (parent)
			return parent = parent->find_parent();
		return this;
	}
};



Country countrys[25][25];

int dx[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1,-1, 1,-1, 0, 1};

int N;
unordered_map<string, Country*> name_id;


void init(int _N, int mSoldier[25][25], char mMonarch[25][25][11])
{	
	N = _N;
	name_id.clear();

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			countrys[i][j].reset(mMonarch[i][j], mSoldier[i][j], i, j);
			name_id[mMonarch[i][j]] = &countrys[i][j];
		}
	}	
}

void destroy()
{

}

int check_enemies(Country* P_A, Country* P_B) {

	set<Country*, compare>* ptr = &P_B->enemies;
	auto it = find(ptr->begin(), ptr->end(), P_A);

	if (it == ptr->end())	return 0;
	else return 1;
}

void set_allies(Country* P_A, Country* P_B) {
	if (P_A->childs.size() < P_B->childs.size()) {
		swap(P_A, P_B);
	}

	for (auto p = P_B->enemies.begin(); p != P_B->enemies.end(); p++) {
		(*p)->enemies.insert(P_A);
		(*p)->enemies.erase(P_B); // 이래야 새로태어난 왕국이 적대적 관계를 청산하겠군
	}
	for (auto p = P_B->childs.begin(); p != P_B->childs.end(); p++) {
		(*p)->parent = P_A;   // B 아이들 부모 변경
	}

	P_A->enemies.insert(P_B->enemies.begin(), P_B->enemies.end());
	P_B->enemies.clear();

	P_A->childs.insert( P_B->childs.begin(), P_B->childs.end());
	
	P_B->childs.clear();
	P_B->parent = P_A;
	P_A->childs.insert(P_B);
}

int ally(char mMonarchA[11], char mMonarchB[11])
{
	Country* A = name_id[mMonarchA];
	Country* B = name_id[mMonarchB];

	Country* P_A = A->find_parent(); Country* P_B = B->find_parent();

	if (P_A == P_B) {
		//cout << -1 << endl;	
		return -1;
	}

	if (check_enemies(P_A, P_B)) {
		//cout << -2 << endl; 
		return -2;
	}

	set_allies(P_A, P_B);

	//cout << 1 << endl;
	return 1;
}

void erase_allies(Country* B) {

	Country* P_B = B->find_parent();

	if (P_B == B) {

		for (auto p = B->enemies.begin(); p != B->enemies.end(); p++) {
			(*p)->enemies.erase(B);
		}

		if (B->childs.empty()) {
			B->reset(B->name, 0, B->pos.i, B->pos.j);
			return;
		}

		Country* T_B = *(B->childs.begin());
		B->childs.erase(B->childs.begin());

		swap(T_B->childs, B->childs);  //체크해볼것
		swap(T_B->enemies, B->enemies);

		for (auto p = T_B->enemies.begin(); p != T_B->enemies.end(); p++) {
			(*p)->enemies.insert(T_B);
		}

		T_B->parent = nullptr;

		for (auto p = T_B->childs.begin(); p != T_B->childs.end(); p++) {
			(*p)->parent = T_B;   // B 아이들 부모 변경
		}
		B->reset(B->name, 0, B->pos.i, B->pos.j);
		return;
	}
	else {
		P_B->childs.erase(B);
		B->reset(B->name, 0, B->pos.i, B->pos.j);
		return;
	}

}

int attack(char mMonarchA[11], char mMonarchB[11], char mGeneral[11])
{
	Country* A = name_id[mMonarchA];
	Country* B = name_id[mMonarchB];

	Country* P_A = A->find_parent(); Country* P_B = B->find_parent();

	if (P_A == P_B) { // 동맹
		//cout << -1 << endl;	
		return -1;
	}

	int add = 0;

	if (!check_enemies(P_A, P_B)) { // 적 추가
		P_B->enemies.insert(P_A);
		P_A->enemies.insert(P_B);
		add = 1;
	}

	vector<Country*> attackers;
	vector<Country*> defenders;

	int x = B->pos.i; int y = B->pos.j;

	for (int i = 0; i < 8; i++) {
		int nx = x + dx[i]; int ny = y + dy[i];
		if (nx < 0 || ny < 0 || nx >= N || ny >= N)	continue;
		Country* T = &countrys[nx][ny];
		Country* P_T = T->find_parent();
		
		if (P_T == P_A)	attackers.push_back(T);
		else if (P_T == P_B) defenders.push_back(T);
	}

	int attack_num = 0; int defend_num = 0;

	
	for (int i = 0; i < attackers.size(); i++) {
		int k = attackers[i]->soldiers / 2;
		attack_num += k;
		attackers[i]->soldiers -= k;
	}

	if (attackers.empty()) {
		if (add) {
			P_B->enemies.erase(P_A);
			P_A->enemies.erase(P_B);
		}
		//cout << -2 << endl;
		return -2;
	}

	defend_num += B->soldiers;
	B->soldiers = 0;

	for (int i = 0; i < defenders.size(); i++) {
		int k = defenders[i]->soldiers / 2;
		defend_num += k;
		defenders[i]->soldiers -= k;
	}

	//cout << "공격 : " << attack_num << " 수비 : " << defend_num << endl;

	if (defend_num >= attack_num) {
		B->soldiers = defend_num - attack_num;
		//cout << 0 << endl;
		return 0;
	}

	else {
		erase_allies(B);
		B->reset(mGeneral, attack_num - defend_num, B->pos.i, B->pos.j);
		set_allies(P_A, B);
		name_id[mGeneral] = B;
		//cout << 1 << endl;
		return 1;
	}

	return -3;
}

int recruit(char mMonarch[11], int mNum, int mOption)
{
	Country* A = name_id[mMonarch];
	
	int answer = 0;
	

	if (mOption == 0) {
		A->soldiers += mNum;
		answer += A->soldiers;
		//cout << answer << endl;
		return answer;
	}
	else {

		Country* P_A = A->find_parent();
		set<Country*, compare>* ptr = &P_A->childs;
		for (auto p = ptr->begin(); p != ptr->end(); p++) {
			(*p)->soldiers += mNum;
			answer += (*p)->soldiers;
		}
		P_A->soldiers += mNum;
		answer += P_A->soldiers;
		//cout << answer << endl;
		return answer;
	}

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
		if (isOK == 0) {
			//cout << "cmd  : " << check << " " << result << " " << cmd << endl;
			//isOK = 1;
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