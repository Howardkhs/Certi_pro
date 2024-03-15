#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>


#define CMD_INIT    1
#define CMD_APPEND  2
#define CMD_CUT     3
#define CMD_REVERSE 4
#define CMD_COUNT   5


#include <deque>
#include <memory.h>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

int char_to_int(char a) {
	return a - 'a' + 1;
}

int front[27][27][27][27];
int back[27][27][27][27];

deque<int> q;
bool rights;

void set_front(char mStr[]) {
	int a, b, c, d;
	int len = strlen(mStr);
	for (int i = 0; i < len; i++) {
		a = char_to_int(mStr[i]);
		front[a][0][0][0] += 1;
	}

	for (int i = 0; i < len - 1; i++) {
		a = char_to_int(mStr[i]);
		b = char_to_int(mStr[i + 1]);
		front[a][b][0][0] += 1;
	}

	for (int i = 0; i < len - 2; i++) {
		a = char_to_int(mStr[i]);
		b = char_to_int(mStr[i + 1]);
		c = char_to_int(mStr[i + 2]);
		front[a][b][c][0] += 1;
	}

	for (int i = 0; i < len - 3; i++) {
		a = char_to_int(mStr[i]);
		b = char_to_int(mStr[i + 1]);
		c = char_to_int(mStr[i + 2]);
		d = char_to_int(mStr[i + 3]);
		front[a][b][c][d] += 1;
	}
}

void set_back(char mStr[]) {
	int a, b, c, d;
	int len = strlen(mStr);
	for (int i = 0; i < len; i++) {
		a = char_to_int(mStr[i]);
		back[a][0][0][0] += 1;
	}

	for (int i = len - 1; i > 0; i--) {
		a = char_to_int(mStr[i]);
		b = char_to_int(mStr[i - 1]);
		back[a][b][0][0] += 1;
	}

	for (int i = len - 1; i > 1; i--) {
		a = char_to_int(mStr[i]);
		b = char_to_int(mStr[i - 1]);
		c = char_to_int(mStr[i - 2]);
		back[a][b][c][0] += 1;
	}

	for (int i = len - 1; i > 2; i--) {
		a = char_to_int(mStr[i]);
		b = char_to_int(mStr[i - 1]);
		c = char_to_int(mStr[i - 2]);
		d = char_to_int(mStr[i - 3]);
		back[a][b][c][d] += 1;
	}
}

void set_q(char mStr[]) {
	int len = strlen(mStr);
	for (int i = 0; i < len; i++) {
		q.push_back(char_to_int(mStr[i]));
	}
}

void init(char mStr[])
{
	memset(front, 0, sizeof front);
	memset(back, 0, sizeof back);
	q.clear();
	rights = true;

	set_front(mStr);
	set_back(mStr);
	set_q(mStr);
}

void add_word(int mlen) {
	int a, b, c, d;
	int len = q.size();

	if (rights) {
		int idx = len - mlen;
		idx = idx - 3;

		for (int i = idx; i < len - 3; i++) {
			if (i < 0)	continue;
			if (i + 1 >= len || i + 2 >= len || i + 3 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = q[i + 3];
			front[a][b][c][d] += 1;
		}

		for (int i = idx + 1; i < len - 2; i++) {
			if (i < 0)	continue;
			if (i + 1 >= len || i + 2 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = 0;
			front[a][b][c][d] += 1;
		}

		for (int i = idx + 2; i < len - 1; i++) {
			if (i < 0)	continue;
			if (i + 1 >= len)	break;
			a = q[i]; b = q[i + 1]; c = 0; d = 0;
			front[a][b][c][d] += 1;
		}

		for (int i = idx + 3; i < len; i++) {
			a = q[i]; b = 0; c = 0; d = 0;
			front[a][b][c][d] += 1;
		}

		for (int i = len - 1; i >= len - mlen; i--) {
			if (i - 1 < 0 || i - 2 < 0 || i - 3 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = q[i - 3];
			back[a][b][c][d] += 1;
		}

		for (int i = len - 1; i >= len - mlen; i--) {
			if (i - 1 < 0 || i - 2 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = 0;
			back[a][b][c][d] += 1;
		}

		for (int i = len - 1; i >= len - mlen; i--) {
			if (i - 1 < 0 )	break;
			a = q[i]; b = q[i - 1]; c = 0; d = 0;
			back[a][b][c][d] += 1;
		}

		for (int i = len - 1; i >= len - mlen; i--) {
			a = q[i]; b = 0; c = 0; d = 0;
			back[a][b][c][d] += 1;
		}
	}
	else {
		for (int i = 0; i < mlen; i++) {
			if (i + 1 >= len || i + 2 >= len || i + 3 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = q[i + 3];
			front[a][b][c][d] += 1;
		}

		for (int i = 0; i < mlen; i++) {
			if (i + 1 >= len || i + 2 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = 0;
			front[a][b][c][d] += 1;
		}

		for (int i = 0; i < mlen; i++) {
			if (i + 1 >= len)	break;
			a = q[i]; b = q[i + 1]; c = 0; d = 0;
			front[a][b][c][d] += 1;
		}

		for (int i = 0; i < mlen; i++) {
			a = q[i]; b = 0; c = 0; d = 0;
			front[a][b][c][d] += 1;
		}

		for (int i = mlen + 2; i >= 0; i--) {
			if (i >= len)	continue;
			if (i - 1 < 0 || i - 2 < 0 || i - 3 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = q[i - 3];
			back[a][b][c][d] += 1;
		}

		for (int i = mlen + 1; i >= 0; i--) {
			if (i >= len)	continue;
			if (i - 1 < 0 || i - 2 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = 0;
			back[a][b][c][d] += 1;
		}

		for (int i = mlen ; i >= 0; i--) {
			if (i >= len)	continue;
			if (i - 1 < 0)	break;
			a = q[i]; b = q[i - 1]; c = 0; d = 0;
			back[a][b][c][d] += 1;
		}

		for (int i = mlen - 1; i >= 0; i--) {
			a = q[i]; b = 0; c = 0; d = 0;
			back[a][b][c][d] += 1;
		}
	}
}

void appendWord(char mWord[])
{
	int mlen = strlen(mWord);

	if (rights) {
		for (int i = 0; i < mlen; i++) {
			q.push_back(char_to_int(mWord[i]));
		}
	}
	else {
		for (int i = 0; i < mlen; i++) {
			q.push_front(char_to_int(mWord[i]));
		}
	}

	add_word(mlen);
}

void cut(int k)
{
	int a, b, c, d;
	int len = q.size();

	if (rights) {
		int idx = len - k;
		idx = idx - 3;

		for (int i = idx; i < len - 3; i++) {
			if (i < 0)	continue;
			if (i + 1 >= len || i + 2 >= len || i + 3 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = q[i + 3];
			front[a][b][c][d] -= 1;
		}

		for (int i = idx + 1; i < len - 2; i++) {
			if (i < 0)	continue;
			if (i + 1 >= len || i + 2 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = 0;
			front[a][b][c][d] -= 1;
		}

		for (int i = idx + 2; i < len - 1; i++) {
			if (i < 0)	continue;
			if (i + 1 >= len)	break;
			a = q[i]; b = q[i + 1]; c = 0; d = 0;
			front[a][b][c][d] -= 1;
		}

		for (int i = idx + 3; i < len; i++) {
			a = q[i]; b = 0; c = 0; d = 0;
			front[a][b][c][d] -= 1;
		}

		for (int i = len - 1; i >= len - k; i--) {
			if (i - 1 < 0 || i - 2 < 0 || i - 3 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = q[i - 3];
			back[a][b][c][d] -= 1;
		}

		for (int i = len - 1; i >= len - k; i--) {
			if (i - 1 < 0 || i - 2 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = 0;
			back[a][b][c][d] -= 1;
		}

		for (int i = len - 1; i >= len - k; i--) {
			if (i - 1 < 0)	break;
			a = q[i]; b = q[i - 1]; c = 0; d = 0;
			back[a][b][c][d] -= 1;
		}

		for (int i = len - 1; i >= len - k; i--) {
			a = q[i]; b = 0; c = 0; d = 0;
			back[a][b][c][d] -= 1;
		}

		for (int i = 0; i < k; i++) {
			q.pop_back();
		}
	}
	else {
		for (int i = 0; i < k; i++) {
			if (i + 1 >= len || i + 2 >= len || i + 3 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = q[i + 3];
			front[a][b][c][d] -= 1;
		}

		for (int i = 0; i < k; i++) {
			if (i + 1 >= len || i + 2 >= len)	break;
			a = q[i]; b = q[i + 1]; c = q[i + 2]; d = 0;
			front[a][b][c][d] -= 1;
		}

		for (int i = 0; i < k; i++) {
			if (i + 1 >= len)	break;
			a = q[i]; b = q[i + 1]; c = 0; d = 0;
			front[a][b][c][d] -= 1;
		}

		for (int i = 0; i < k; i++) {
			a = q[i]; b = 0; c = 0; d = 0;
			front[a][b][c][d] -= 1;
		}

		for (int i = k + 2; i >= 0; i--) {
			if (i >= len)	continue;
			if (i - 1 < 0 || i - 2 < 0 || i - 3 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = q[i - 3];
			back[a][b][c][d] -= 1;
		}

		for (int i = k + 1; i >= 0; i--) {
			if (i >= len)	continue;
			if (i - 1 < 0 || i - 2 < 0)	break;
			a = q[i]; b = q[i - 1]; c = q[i - 2]; d = 0;
			back[a][b][c][d] -= 1;
		}

		for (int i = k; i >= 0; i--) {
			if (i >= len)	continue;
			if (i - 1 < 0)	break;
			a = q[i]; b = q[i - 1]; c = 0; d = 0;
			back[a][b][c][d] -= 1;
		}

		for (int i = k - 1; i >= 0; i--) {
			a = q[i]; b = 0; c = 0; d = 0;
			back[a][b][c][d] -= 1;
		}

		for (int i = 0; i < k; i++) {
			q.pop_front();
		}

	}

}

void reverse()
{
	if (rights)	rights = false;
	else		rights = true;
}

int countOccurrence(char mWord[])
{
	int a, b, c, d;

	int len = strlen(mWord);

	if (len == 1) {
		a = char_to_int(mWord[0]); b = 0; c = 0; d = 0;
	}
	if (len == 2) {
		a = char_to_int(mWord[0]); b = char_to_int(mWord[1]); c = 0; d = 0;
	}
	if (len == 3) {
		a = char_to_int(mWord[0]); b = char_to_int(mWord[1]); c = char_to_int(mWord[2]); d = 0;
	}
	if (len == 4) {
		a = char_to_int(mWord[0]); b = char_to_int(mWord[1]); c = char_to_int(mWord[2]); d = char_to_int(mWord[3]);
	}

	int answer;
	
	if (rights) {
		answer = front[a][b][c][d];
	}
	else {
		answer = back[a][b][c][d];
	}
	//cout << answer << endl;

	return answer;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static bool run()
{
	bool correct = false;
	int queryCnt;
	scanf("%d", &queryCnt);
	static char mStr[30001], mWord[5];

	while (queryCnt--)
	{
		int cmd;
		scanf("%d", &cmd);

		if (cmd == CMD_INIT)
		{
			scanf("%s", mStr);
			init(mStr);
			correct = true;
		}
		else if (cmd == CMD_APPEND)
		{
			scanf("%s", mWord);

			if (correct)
			{
				appendWord(mWord);
			}
		}
		else if (cmd == CMD_CUT)
		{
			int k;
			scanf("%d", &k);

			if (correct)
			{
				cut(k);
			}
		}
		else if (cmd == CMD_REVERSE)
		{
			if (correct)
			{
				reverse();
			}
		}
		else if (cmd == CMD_COUNT)
		{
			scanf("%s", mWord);

			int ret = -1;
			if (correct)
			{
				ret = countOccurrence(mWord);
			}

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