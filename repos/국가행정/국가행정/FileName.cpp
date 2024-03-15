

#include <queue>
using namespace std;

int base;
int seg[1 << 15];
void update(int index, int value) {
	seg[index += base] = value;
	while (index >>= 1) seg[index] = seg[index * 2] + seg[index * 2 + 1];
}
int get_sum(int qs, int qe) {
	int ret = 0;
	for (qs += base, qe += base; qs <= qe; qs >>= 1, qe >>= 1) {
		if (qs % 2 == 1) ret += seg[qs++];
		if (qe % 2 == 0) ret += seg[qe--];
	}
	return ret;
}
struct pq_edge {
	int index, weight;
};
bool operator<(const pq_edge& a, const pq_edge& b) {
	if (a.weight != b.weight) return a.weight < b.weight;
	else return a.index > b.index;
}

priority_queue< pq_edge > pq;
int* population;
int C[10000];

void init(int N, int mPopulation[]) {
	population = mPopulation;
	pq = priority_queue< pq_edge >();
	base = 1;
	while (base < N - 1) base *= 2;
	for (int i = 0; i + 1 < N; ++i) {
		C[i] = 1;
		int weight = (mPopulation[i] + mPopulation[i + 1]);
		pq.push({ i, weight });
		seg[base + i] = weight;
	}
	for (int i = base - 1; i > 0; --i) {
		seg[i] = seg[i * 2] + seg[i * 2 + 1];
	}
}
int expand(int M) {
	int new_weight = -1;
	while (M--) {
		auto [index, weight] = pq.top(); pq.pop();
		++C[index];
		new_weight = (population[index] + population[index + 1]) / C[index];
		pq.push({ index, new_weight });
		update(index, new_weight);
	}
	return new_weight;
}

int calculate(int from, int to) {
	if (from > to) swap(from, to);
	return get_sum(from, to - 1);
}

int divide(int from, int to, int K) {
	int lf = 1, rg = (int)1e7;
	while (lf < rg) {
		int mid = (lf + rg) / 2;
		int P = 0;
		for (int i = from; i <= to && P <= K; ++P) {
			int sum = 0, j = i;
			while (j <= to && sum + population[j] <= mid) {
				sum += population[j++];
			}
			i = j;
		}
		if (P <= K) {
			rg = mid;
		}
		else {
			lf = mid + 1;
		}
	}
	return rg;
}