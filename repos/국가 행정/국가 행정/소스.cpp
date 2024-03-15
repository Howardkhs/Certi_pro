#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#define MAX_N				10000

#define CMD_INIT			100
#define CMD_EXPAND			200
#define CMD_CALCULATE		300
#define CMD_DIVIDE			400
#include <stdio.h>


#include <iostream>
#include <queue>
#include <memory.h>

using namespace std;

int answer;

class Node {
public:
	int id;

	int start;
	int end;
	int len;
	int Olen;
	int expands;

	Node* lft; Node* rgt; Node* parent;
	Node(){}
	void find(int _start, int _end) {
		if (start == _start && end == _end) {
			answer += len;
			return;
		}

		int mid = (start + end) / 2;
		
		if (_end <= mid) {
			lft->find(_start, _end);
		}
		else if (mid < _start) {
			rgt->find(_start, _end);
		}
		else {
			lft->find(_start, mid);
			rgt->find(mid + 1, _end);
		}
	}
};

struct compare {
	bool operator()(Node* A, Node* B) {
		if (A->len != B->len)
			return A->len < B->len;
		return A->id > B->id;
	}
};

Node nodes[200002];
priority_queue<Node*, vector<Node*>, compare> pq;
int* pop;
int N;
Node* root;
int node_ptr;

Node* reset(int _start, int _end) { // 도로 N-2넣기
	Node* N = &nodes[node_ptr++];

	N->start = _start; N->end = _end;
	if (_start == _end) {
		N->len = pop[_start] + pop[_start + 1]; N->Olen = N->len;
		N->id = _start; N->expands = 1;
		pq.push(N);
		return N;
	}

	int mid = (_start + _end) / 2;

	N->lft = reset(_start, mid);
	N->rgt = reset(mid + 1, _end);

	N->lft->parent = N; N->rgt->parent = N;
	N->len = N->lft->len + N->rgt->len;
	return N;
}

void init(int _N, int mPopulation[])
{
	N = _N;
	pq = priority_queue<Node*, vector<Node*>, compare>();
	pop = mPopulation;
	node_ptr = 1;
	memset(nodes, 0, sizeof nodes);

	root = reset(0, N - 2);
	return;
}

void after_expands(Node* N) {
	while (N->parent) {
		N = N->parent;
		N->len = N->lft->len + N->rgt->len;
	}
}

int expand(int M)
{
	while (M--) {
		Node* N = pq.top(); pq.pop();
		int Olen = N->Olen;
		int expands = N->expands;


		expands += 1;
		Olen = Olen / expands;
		N->len = Olen; N->expands = expands;
		answer = N->len;
		pq.push(N);
		after_expands(N);
	}
	return answer;
}

int calculate(int mFrom, int mTo)
{
	answer = 0;

	if (mFrom > mTo) swap(mFrom, mTo);
	root->find(mFrom, mTo - 1);


	return answer;
}

int divide(int mFrom, int mTo, int K)
{
	int start = 0;
	int end = 10'000'000;
	
	while (start <= end) {
		int mid = (start + end) / 2;

		int cnt = 1;
		int sum = 0;
		int fail = 0;

		for (int i = mFrom; i <= mTo; i++) {
			if (pop[i] > mid) { // 선거구 하나보다 미드가 작음
				start = mid + 1;
				fail = 1;
				break;
			}
			sum += pop[i];
			if (sum > mid) {
				sum = pop[i];
				cnt += 1;
			}
		}

		if (fail)	continue;

		if (cnt > K) { // 선거구가 너무 나뉨 즉 mid를 올려라
			start = mid + 1;
		}
		else {
			answer = mid;
			end = mid - 1;
		}
	}
	return answer;
}

/////////////////////////////////////////////////////////////////////////



static bool run()
{
	int population[MAX_N];
	int cmd, ans, ret;
	int Q = 0;
	int N, from, to, num;
	bool okay = false;

	scanf("%d", &Q);

	for (int q = 0; q < Q; ++q)
	{
		scanf("%d", &cmd);

		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &N);

			for (int i = 0; i < N; i++)
				scanf("%d", &population[i]);

			init(N, population);
			okay = true;
			break;

		case CMD_EXPAND:
			scanf("%d", &num);
			ret = expand(num);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;

		case CMD_CALCULATE:
			scanf("%d %d", &from, &to);
			ret = calculate(from, to);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;

		case CMD_DIVIDE:
			scanf("%d %d %d", &from, &to, &num);
			ret = divide(from, to, num);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
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