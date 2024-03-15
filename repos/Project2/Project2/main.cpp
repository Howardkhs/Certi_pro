#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include<queue>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

class Player {
public:
    int abi;
    int id;
    int league;


    Player() {}
    Player(int _id, int _abi) {
        id = _id;
        abi = _abi;
    }
};

struct compare1 { // 최소 오름차순
    bool operator()(const Player* A, const Player* B) const {
        if (A->abi != B->abi)
            return A->abi > B->abi;
        return A->id < B->id;
    }
};

struct compare2 { // 최대 내림차순
    bool operator()(const Player* A, const Player* B) const {
        if (A->abi != B->abi)
            return A->abi < B->abi;
        return A->id > B->id;
    }
};

bool compare3(const Player A, const Player B) { //오름차순 정렬
    if (A.abi != B.abi)
        return A.abi > B.abi;
    return A.id < B.id;
}

//typedef priority_queue<Player, vector<Player>, compare1> lpq; // 최소 힙
//typedef priority_queue<Player, vector<Player>, compare2> hpq; // 최대 힙
typedef set<Player*, compare2> lpq; // 최소 힙
typedef set<Player*, compare1> hpq; // 최소 힙
Player players[40000];

hpq Hleague2[10]; //move 전용

lpq Lleague[10];
hpq Hleague[10];

int N, L;
int l_cnt;

bool compare_P(const Player* A, const Player* B) { // b가 더 좋은 선수면 1반납
    if (A->abi != B->abi)
        return A->abi < B->abi;
    return A->id > B->id;
}

void set_q(int N, int L) {

    for (int i = 0; i < N; i++) {

        int league = players[i].league;
        int lsize = Lleague[league].size();
        int hsize = Hleague[league].size();

        //cout << id << " " << abi << " " << league << endl;

        //Lleague2[league].insert(&players[i]);
        Hleague2[league].insert(&players[i]);

        if (lsize == hsize) {
            Hleague[league].insert(&players[i]);
            if (Lleague[league].empty())  continue;
            else if (compare_P(*Lleague[league].begin(), *Hleague[league].begin())) { // 최대가 더 좋은 경우
                Player* A = *Lleague[league].begin(); Lleague[league].erase(Lleague[league].begin());
                Player* B = *Hleague[league].begin(); Hleague[league].erase(Hleague[league].begin());
                Hleague[league].insert(A);
                Lleague[league].insert(B);

                continue;
            }
        }
        else if (lsize < hsize) {
            Lleague[league].insert(&players[i]);
            if (compare_P(*Lleague[league].begin(), *Hleague[league].begin())) {// 최대가 더 좋은 경우
                Player* A = *Lleague[league].begin(); Lleague[league].erase(Lleague[league].begin());
                Player* B = *Hleague[league].begin(); Hleague[league].erase(Hleague[league].begin());
                Hleague[league].insert(A);
                Lleague[league].insert(B);
                continue;
            }
        }
    }
}

void init(int _N, int _L, int mAbility[])
{
    N = _N;
    L = _L;
    int league = -1;
    l_cnt = N / (N / L);
    for (int i = 0; i < N; i++) {
        if (i % (N / L) == 0) league++;
        players[i].id = i;
        players[i].abi = mAbility[i];
        players[i].league = league;

    }

    for (int i = 0; i < L; i++) {
        Lleague[i] = lpq();
        Hleague[i] = hpq();
        // Lleague2[i] = lpq();
        Hleague2[i] = hpq();
    }
    set_q(_N, _L);

}

Player* get_real_player(int league, int horl) {


    if (horl == 1) { // 최대에서 가져오기
        while (1) {
            Player* A = *Hleague2[league].begin();
            Hleague2[league].erase(Hleague2[league].begin());
            //Lleague2[league].erase(*Lleague2[league].rbegin());
            Lleague[league].erase(*Lleague[league].rbegin());

            return A;
        }
    }
    else { // 최소에서 가져오기
        while (1) {
            Player* A = *Hleague2[league].rbegin();
            //Lleague2[league].erase(Lleague2[league].begin());
            Hleague2[league].erase(*Hleague2[league].rbegin());
            Hleague[league].erase(*Hleague[league].rbegin());

            return A;
        }
    }
}

void push_mid(Player* A) {


    int league = A->league;
    int lsize = Lleague[league].size();
    int hsize = Hleague[league].size();

    if (lsize == hsize) {
        Hleague[league].insert(A);
        if (Lleague[league].empty()) return;
        else if (compare_P(*Lleague[league].begin(), *Hleague[league].begin())) { // 최대가 더 좋은 경우
            Player* A = *Lleague[league].begin(); Lleague[league].erase(Lleague[league].begin());
            Player* B = *Hleague[league].begin(); Hleague[league].erase(Hleague[league].begin());
            Hleague[league].insert(A);
            Lleague[league].insert(B);

        }
    }
    else if (lsize < hsize) {
        Lleague[league].insert(A);
        if (compare_P(*Lleague[league].begin(), *Hleague[league].begin())) {// 최대가 더 좋은 경우
            Player* A = *Lleague[league].begin(); Lleague[league].erase(Lleague[league].begin());
            Player* B = *Hleague[league].begin(); Hleague[league].erase(Hleague[league].begin());
            Hleague[league].insert(A);
            Lleague[league].insert(B);
        }
    }
}


int move()
{

    int answer = 0;
    vector<Player*> change_list;

    for (int i = 0; i < l_cnt - 1; i++) { // 마지막 리그는 따로

        Player* A = get_real_player(i, 0);
        Player* B = get_real_player(i + 1, 1);

        answer += A->id + B->id;
        B->league = i; // players[B.id].league = i;
        A->league = i + 1; //players[A.id].league = i + 1;


        change_list.push_back(A);
        change_list.push_back(B);
    }

    for (int i = 0; i < change_list.size(); i++) {
        Player* A = change_list[i];
        int league = A->league;

        // Lleague2[league].insert(A);
        Hleague2[league].insert(A);
        push_mid(A);
    }


    return answer;
}

Player* get_mid_player(int league) {

    while (1) {

        int lsize = Lleague[league].size();
        int hsize = Hleague[league].size();

        if (lsize < hsize) {
            Player* A = *Hleague[league].begin();
            Hleague[league].erase(Hleague[league].begin());
            //Hleague2[league].erase(A);
            //Lleague2[league].erase(A);
            return A;

        }
        else {
            Player* A = *Lleague[league].begin();
            Lleague[league].erase(Lleague[league].begin());
            //Hleague2[league].erase(A);
           // Lleague2[league].erase(A);
            return A;

        }

    }
}


int trade()
{
    int answer = 0;
    vector<Player*> change_list;

    for (int i = 0; i < l_cnt - 1; i++) { // 마지막 리그는 따로
        Player* A = get_mid_player(i);
        Player* B = get_real_player(i + 1, 1);

        // cout << A.id << " " << B.id << " ";

        answer += A->id + B->id;
        B->league = i; // players[B.id].league = i;
        A->league = i + 1; //players[A.id].league = i + 1;

        change_list.push_back(A);
        change_list.push_back(B);
    }

    for (int i = 0; i < change_list.size(); i++) {
        Player* A = change_list[i];
        int league = A->league;

        // Lleague2[league].insert(A);
        Hleague2[league].insert(A);
        push_mid(A);
    }

    return answer;
}
#define MAX_N 39990

#define CMD_INIT 100
#define CMD_MOVE 200
#define CMD_TRADE 300

static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int ans;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);
        if (query == CMD_INIT)
        {
            int N;
            int L;
            int mAbility[MAX_N];
            scanf("%d %d", &N, &L);
            for (int i = 0; i < N; i++) {
                scanf("%d", &mAbility[i]);
            }
            init(N, L, mAbility);
            ok = true;
        }
        else if (query == CMD_MOVE)
        {
            int ret = move();
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
        else if (query == CMD_TRADE)
        {
            int ret = trade();
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
    }
    return ok;
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