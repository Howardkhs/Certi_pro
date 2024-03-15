#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT       100
#define CMD_INSERT     200
#define CMD_MOVECURSOR 300
#define CMD_COUNT      400

/////////////////////////////////////////////////////////////////////////

#include <list>
#include <cstring>
#include <iostream>
#include <memory.h>
using namespace std;

int char_to_int(char a) {
	return a - 'a' + 1;
}

char int_to_char(int a) {
	return a + 'a' - 1;
}

struct P {
	int x;
	int y;
	P(){}
	void set(int _x, int _y) {
		x = _x; y = _y;
	}
};

int H, W;
P cur;
P back;

list<int> texts[300];
int words[300][27];

void init(int _H, int _W, char mStr[])
{
	H = _H; W = _W;
	for (int i = 0; i < 300; i++) {
		texts[i].clear();
	}
	memset(words, 0, sizeof words);

	int len = strlen(mStr);

	int idx = 0;
	int cnt = 0;

	for (int i = 0; i < len; i++) {
		int word = char_to_int(mStr[i]);
		texts[idx].push_back(word);
		words[idx][word]++;
		cnt++;
		if (cnt == W) {
			cnt = 0;
			idx++;
		}
	}
	cur.set(0, 0);
	back.set(idx, cnt);
}

void insert(char mChar)
{
	int x = cur.x;	int y = cur.y;
	
	auto it = next(texts[x].begin(), y);

	int word = char_to_int(mChar);
	texts[x].insert(it, word);
	words[x][word]++;

	while (1) {

		if (x == H)	break;

		if (texts[x].size() == W + 1) {
			word = *texts[x].rbegin();
			texts[x].pop_back();
			words[x][word]--;
			texts[x + 1].push_front(word);
			words[x + 1][word]++;
			x += 1;
			continue;
		}
		else	break;
	}

	x = cur.x; y = cur.y;
	y = y + 1;
	if (y == W) {
		y = 0; x++;
	}
	cur.set(x, y);
	
	x = back.x; y = back.y;
	y = y + 1;
	if (y == W) {
		y = 0; x++;
	}
	back.set(x, y);
	
}

bool compare(int R, int C) {
	
	if (R != back.x)
		return R < back.x;
	return C <= back.y;
}

char moveCursor(int mRow, int mCol)
{
	mRow--; mCol--;
	if (compare(mRow, mCol)) {
		cur.set(mRow, mCol);
	}
	else {
		cur.set(back.x, back.y);
	}

	auto it = next(texts[cur.x].begin(), cur.y);
	
	if (it == texts[cur.x].end())	return '$';
	
	return int_to_char(*it);
}

int countCharacter(char mChar)
{
	int word = char_to_int(mChar);
	int answer = 0;
	for (int i = cur.x; i <= back.x; i++) {
		answer += words[i][word];
	}
	auto it = texts[cur.x].begin();

	for (int i = 0; i < cur.y; i++) {
		int nword = *it;
		if (nword == word) answer--;
		it++;
	}
	return answer;
}

/////////////////////////////////////////////////////////////////////////

static bool run()
{
	bool correct = false;
	int queryCnt;
	scanf("%d", &queryCnt);
	int H, W;
	char mChar;
	static char mStr[90001];

	while (queryCnt--)
	{
		int cmd;
		scanf("%d", &cmd);

		if (cmd == CMD_INIT)
		{
			scanf("%d %d %s", &H, &W, mStr);
			init(H, W, mStr);
			correct = true;
		}
		else if (cmd == CMD_INSERT)
		{
			scanf(" %c", &mChar);
			insert(mChar);
		}
		else if (cmd == CMD_MOVECURSOR)
		{
			int mRow, mCol;
			scanf("%d %d", &mRow, &mCol);

			char ret = moveCursor(mRow, mCol);

			char ans;
			scanf(" %c", &ans);
			if (ret != ans)
			{
				correct = false;
			}
		}
		else if (cmd == CMD_COUNT)
		{
			scanf(" %c", &mChar);

			int ret = countCharacter(mChar);

			int ans;
			scanf("%d", &ans);
			if (ret != ans)
			{
				correct = false;
			}
		}
	}
	return correct;
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