#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include <iostream>
#include <memory.h>
#include <queue>
#include <cmath>

using namespace std;

int sum;

struct Road {
	int num;
	int len;

	Road(){}
	Road(int _num, int _len) {
		num = _num;
		len = _len;
	}
	void set(int _num, int _len) {
		num = _num;
		len = _len;
	}

};

struct compare {
	bool operator()(Road* a, Road* b) {
		if (a->len != b->len)
			return a->len < b->len;
		return a->num > b->num;
	}
};

class Node {
public:

	int start;
	int end;
	int length;

	Node* lft;
	Node* rgt;

	Node() {}

	Node(int _start, int _end, int _length) {
		start = _start;
		end = _end;
		length = _length;
		lft = nullptr;
		rgt = nullptr;
	}

	void set(int _start, int _end, int _length) {
		start = _start;
		end = _end;
		length = _length;
		lft = nullptr;
		rgt = nullptr;
	}


	void change(int num, int len) {
		if (start == num && end == num) {
			length = len;
			return;
		}
		int mid = (start + end) / 2;
		if (num <= mid) {
			lft->change(num, len);
		}
		else {
			rgt->change(num, len);
		}
		length = lft->length + rgt->length;
	}

	void cal(int _start, int _end) {
		if (start == _start && end == _end) {
			sum += length;
			return;
		}

		int mid = (start + end) / 2;
		if (_end <= mid) {
			lft->cal(_start, _end);
		}
		else if (_start > mid) {
			rgt->cal(_start, _end);
		}
		else {
			lft->cal(_start, mid);
			rgt->cal(mid + 1, _end);
		}
	}

};


Node nodes[20000];
int road_len[10000];
int after_road_len[10000];
int expand_cnt[10000];
Road roads[10001];

int *pop;
int node_ptr;
int pop_ptr;
Node* root_node;

priority_queue<Road*, vector<Road*>, compare> pq;

Node* node_init(int start, int end, int mPopulation[]) {
	Node* N = &nodes[node_ptr++];
	N->set(start, end, 0);
	if (start == end) {
		N->length = mPopulation[pop_ptr] + mPopulation[pop_ptr+1];
		road_len[pop_ptr] = N->length;
		after_road_len[pop_ptr] = N->length;
		pop_ptr += 1;
		return N;
	}

	N->lft = node_init(start, (start + end) / 2, mPopulation);
	N->rgt = node_init((start + end) / 2 + 1, end,  mPopulation);
	N->length = N->lft->length + N->rgt->length;

	return N;
}

void init(int N, int mPopulation[])
{
	node_ptr = 0;
	pop_ptr = 0;
	//memset(road_len, 0, sizeof road_len);
	//memset(after_road_len, 0, sizeof after_road_len);
	memset(expand_cnt, 0, sizeof expand_cnt);
	//memset(pop, 0, sizeof pop);
	
	pq = priority_queue<Road*, vector<Road*>, compare>();

	root_node = node_init(0, N - 2, mPopulation);
	//print();

	for (int i = 0; i < N - 1; i++) {
		roads[i].set(i, road_len[i]);
		//Road A(i, road_len[i]);
		pq.push(&roads[i]);
	}

	pop = mPopulation;
	return;
}

int expand(int M)
{
	int answer;
	for (int i = 0; i < M; i++) {	
		Road* A = pq.top();
		pq.pop();

		// 도로 확장 하고
		expand_cnt[A->num] += 1;
		after_road_len[A->num] = floor(road_len[A->num] / (expand_cnt[A->num] + 1));
		A->len = after_road_len[A->num];

		pq.push(A);

		root_node->change(A->num, A->len);
		answer = A->len;
		
	}

	//cout << answer << endl;

	return answer;
}

int calculate(int mFrom, int mTo)
{
	sum = 0;
	
	if (mFrom > mTo) {
		swap(mFrom, mTo);
	}

	root_node->cal(mFrom, mTo - 1);
	//cout << sum << endl;
	return sum;
}


int check(int start, int end, int K, int mid) {

	int cnt = 0;
	int G = 0;
	int max_p = 0;

	for (int i = start; i <= end; i++) {
		if (cnt == K)	return -1; // 너무 많이 분할된 경우 -> mid를 더 올려줘야함
		if (pop[i] > mid) return -1; // 너무 mid가 작은경우
		G += pop[i];
		if (G > mid) {
			G = pop[i];
			cnt++;
		}
	}
	if (cnt == K)	return -1;

	return mid;

}

int divide(int mFrom, int mTo, int K)
{
	int end = 50000000;
	int start = 0;

	int answer;

	while (start <= end) {
		int mid = (start + end) / 2;

		int ret = check(mFrom, mTo, K, mid);
		//cout << ret << " " << mid<< endl;
		if (ret == -1) {
			start = mid + 1;
			continue;
		}

		if (ret == 0) {
			end = mid - 1;
			continue;
		}
		answer = ret;
		end = mid - 1;
	}
	//cout << answer << endl;
	return answer;
}

/////////////////////////////////////////////////////////////////////////

#define MAX_N				10000

#define CMD_INIT			100
#define CMD_EXPAND			200
#define CMD_CALCULATE		300
#define CMD_DIVIDE			400

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
			//cout << cmd << " " << from << " " << to << " " << num << endl;
			ret = divide(from, to, num);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;

		default:
			okay = false;
		}
		if (okay == false) {
			cout << cmd << " " << from << " " << to << endl;
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

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}