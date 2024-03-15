#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>

using namespace std;

struct P {
	int id;
	int score;
	int classes;
	int gender;
	P(){}
	P(int _id, int _score, int _classes, int _gender){
		id = _id; score = _score; classes = _classes; gender = _gender;
	}
	void set(int _id, int _score, int _classes, int _gender) {
		id = _id; score = _score; classes = _classes; gender = _gender;
	}
};

struct compare {
	bool operator()(const P* A, const P* B) const {
		if (A->score != B->score)
			return A->score < B->score;
		return A->id < B->id;
	}
};

P students[20010];
int students_ptr;

set<P*, compare> classes[4][2];
unordered_map<int, int> id_map;

void init() {

	students_ptr = 1;
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			classes[i][j].clear();
		}
	}
	id_map.clear();
	
	return;
}

int add(int mId, int mGrade, char mGender[7], int mScore) {

	id_map[mId] = students_ptr; // 삭제시 0으로
	P* N = &students[students_ptr++];

	int gender;

	if (mGender[0] == 'm')	gender = 0;
	else					gender = 1;

	N->set(mId, mScore, mGrade, gender);

	classes[mGrade][gender].insert(N);

	int answer = (*classes[mGrade][gender].rbegin())->id; // 큰놈


	return answer;
}

int remove(int mId) {

	int ptr = id_map[mId];

	if (ptr == 0)	return 0;

	id_map[mId] = 0;
	P* N = &students[ptr];

	int id, score, grade, gender;
	id = N->id; score = N->score; grade = N->classes; gender = N->gender;

	classes[grade][gender].erase(N); // 확인 필요
	
	if (classes[grade][gender].empty())	return 0;

	int answer = (*classes[grade][gender].begin())->id;
	return answer;
}

int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore) {


	set<P*, compare> temp;
	P A(0, mScore, 0, 0);
	int grade, gender;

	for (int i = 0; i < mGradeCnt; i++) {
		for (int j = 0; j < mGenderCnt; j++) {
		
			grade = mGrade[i]; 
			if (mGender[j][0] == 'm')	gender = 0;
			else                        gender = 1;

			auto it = classes[grade][gender].upper_bound(&A);
			if (it != classes[grade][gender].end())
				temp.insert(*it);
		}
	}

	if (temp.empty())	return 0;
	int answer = (*temp.begin())->id;


	return answer;
}
/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_REMOVE 300
#define CMD_QUERY 400

static bool run() {
	int q;
	scanf("%d", &q);

	int id, grade, score;
	char gender[7];
	int cmd, ans, ret;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			init();
			okay = true;
			break;
		case CMD_ADD:
			scanf("%d %d %s %d %d", &id, &grade, gender, &score, &ans);
			ret = add(id, grade, gender, score);
			if (ans != ret)
				okay = false;
			break;
		case CMD_REMOVE:
			scanf("%d %d", &id, &ans);
			ret = remove(id);
			if (ans != ret)
				okay = false;
			break;
		case CMD_QUERY: {
			int gradeCnt, genderCnt;
			int gradeArr[3];
			char genderArr[2][7];
			scanf("%d", &gradeCnt);
			if (gradeCnt == 1) {
				scanf("%d %d", &gradeArr[0], &genderCnt);
			}
			else if (gradeCnt == 2) {
				scanf("%d %d %d", &gradeArr[0], &gradeArr[1], &genderCnt);
			}
			else {
				scanf("%d %d %d %d", &gradeArr[0], &gradeArr[1], &gradeArr[2], &genderCnt);
			}
			if (genderCnt == 1) {
				scanf("%s %d %d", genderArr[0], &score, &ans);
			}
			else {
				scanf("%s %s %d %d", genderArr[0], genderArr[1], &score, &ans);
			}
			ret = query(gradeCnt, gradeArr, genderCnt, genderArr, score);
			if (ans != ret)
				okay = false;
			break;
		}
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