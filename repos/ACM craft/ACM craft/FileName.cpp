#include <iostream>
#include <memory.h>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int check[1001];

class Node {
public:
	bool complete = false;
	int time;
	int end_time;
	int id;
	vector<int> next;
	set<int> waiting;

	Node(){}
	void reset() {
		time = 0; next.clear();	end_time = 0;
		complete = false;	waiting.clear();
	}
};

struct compare {
	bool operator()(Node* A, Node* B) {
		return A->end_time > B->end_time;
	}
};

Node nodes[1001];
priority_queue<Node*, vector<Node*>, compare> pq;

int main() {

	int T, N, K;
	int time;
	int a, b;
	int W;
	cin >> T;

	while (T--) {
		int answer = 0;
		memset(check, 0, sizeof check);
		pq = priority_queue<Node*, vector<Node*>, compare>();
		for (int i = 0; i < 1001; i++) {
			nodes[i].reset();
		}

		cin >> N >> K;

		for (int i = 1; i <= N; i++) {
			cin >> time;
			nodes[i].time = time;
			nodes[i].id = i;
		}
		
		while (K--) {
			cin >> a >> b;
			nodes[a].next.push_back(b);
			nodes[b].waiting.insert(a);
		}
		cin >> W;

		for (int i = 1; i <= N; i++) {
			if (nodes[i].waiting.empty()) { 
				nodes[i].end_time = nodes[i].time;
				pq.push(&nodes[i]); 
			}
		}

		while (!pq.empty()) {
			Node* N = pq.top(); pq.pop();
			answer = N->end_time;
			if (W == N->id)	break;

			check[N->id] = 1;

			for (int i = 0; i < N->next.size(); i++) {
				int nxt_id = N->next[i];
				Node* nxt = &nodes[nxt_id];
				vector<int> temp;

				for (auto p = nxt->waiting.begin(); p != nxt->waiting.end(); p++) {
					if (check[*p]) {
						temp.push_back(*p);
					}
					else break;
				}

				for (int i = 0; i < temp.size(); i++) {
					nxt->waiting.erase(temp[i]);
				}
				
				if (nxt->waiting.empty()) {
					nxt->end_time = answer + nxt->time;
					pq.push(nxt);
				}
			}
		}

		cout << answer << "\n";
	}
}