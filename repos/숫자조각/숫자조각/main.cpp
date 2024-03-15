#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAX_ROW 40
#define MAX_COL 30

struct Result{
    int row;
    int col;
};

#include <unordered_map>
// #include <iostream>
#include <deque>
#include <vector>
#include <memory.h>
#include <algorithm>

using namespace std;

typedef pair<int, int> P; // x,y ÁÂÇ¥

struct Block {
    int name;
    int x;
    int y;
};

//unordered_map<int, vector<P>> blocks[5];
//vector<vector<P>> blocks
vector<P> blocks[5][30000];
vector<Block> block_archive[5];
int idxs[5][30000];
//unordered_map<int, int> idxs[5];


int graph[40][30];
int visited[40][30];
int H, W;

vector<P> move0 = { {0,0}, {0,1} };
vector<P> move1 = { {0,0}, {0,1}, {0,1} };
vector<P> move2 = { {0,0}, {1,0}, {1,0} };
vector<P> move3 = { {0,0}, {0,1}, {1,0}, {0,1} };
vector<P> move4 = { {0,0}, {1,0}, {0,1}, {0,1}, {1,0} };


typedef pair<int, int> P; // x,y ÁÂÇ¥
vector<vector<P>> move_vec = { {{0,0}, {0,1}}, {{0,0}, {0,1}, {0,1}}, {{0,0}, {1,0}, {1,0}}, {{0,0}, {0,1}, {1,0}, {0,1}}, {{0,0}, {1,0}, {0,1}, {0,1}, {1,0}} };

// void print_blocks(){
//     for (int i = 0; i < 5; i++){
//         cout << "block type : " << i << endl;
//         for(int j = 0; j < block_archive[i].size(); j++){
//             cout << block_archive[i][j].name << " " << block_archive[i][j].x << " " << block_archive[i][j].y << " ";
//             cout << endl;
//         }      
//     }
// }

void get_blocks(int x, int y) {

    int nx = x;
    int ny = y;
    int name = 0;
    int fail = 0;

    for (int type = 0; type < 5; type++) {
        name = 0;
        fail = 0;
        nx = x;
        ny = y;
        vector<int> nums;
        vector<int> names;
        for (int i = 0; i < move_vec[type].size(); i++) {
            nx = nx + move_vec[type][i].first;
            ny = ny + move_vec[type][i].second;
            nums.push_back(graph[nx][ny]);

            if (nx >= H || ny >= W) {
                fail = 1;
                break;
            }
        }
        if (fail)   continue;
        int max_num = *max_element(nums.begin(), nums.end());
        int min_num = *min_element(nums.begin(), nums.end());

        max_num -= (min_num - 1);

        for (int i = 0; i < nums.size(); i++) {
            nums[i] -= (min_num - 1);
        }

        for (int i = 0; i < 1; i++) {
            name = 0;
            for (int j = 0; j < nums.size(); j++) {
                name = name << 3;
                name = name | (nums[j] + i);
            }
            names.push_back(name);
        }
        for (int i = 0; i < names.size(); i++) {
            Block B{ names[i], x, y };
            block_archive[type].push_back(B);
        }
    }

}

int set = 0;

void init(int mRows, int mCols, int mCells[MAX_ROW][MAX_COL])
{
    H = mRows;
    W = mCols;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 30000; j++) {
            blocks[i][j].clear();
        }
        block_archive[i].clear();
    }
    

    memset(idxs, 0, sizeof(idxs));
    memset(graph, 0, sizeof(int) * 1200);
    memset(visited, 0, sizeof(int) * 1200);

    for (int i = 0; i < mRows; i++) {
        for (int j = 0; j < mCols; j++) {
            graph[i][j] = mCells[i][j];
        }
    }

    for (int x = 0; x < mRows; x++) {
        for (int y = 0; y < mCols; y++) {
            get_blocks(x, y);
        }
    }

    for (int type = 0; type < 5; type++) {
        for (int j = 0; j < block_archive[type].size(); j++) {
            blocks[type][block_archive[type][j].name].push_back({ block_archive[type][j].x, block_archive[type][j].y });
        }
    }
}

int get_name(int mPuzzle[3][3], int type) {
    int x = 0;
    int y = 0;
    int name = 0;
    int nx, ny;
    nx = x;
    ny = y;
    vector<int> nums;
    for (int i = 0; i < move_vec[type].size(); i++) {
        nx = nx + move_vec[type][i].first;
        ny = ny + move_vec[type][i].second;
        nums.push_back(mPuzzle[nx][ny]);

    }
    int max_num = *max_element(nums.begin(), nums.end());
    int min_num = *min_element(nums.begin(), nums.end());

    for (int i = 0; i < nums.size(); i++) {
        nums[i] -= (min_num - 1);
    }

    nx = x;
    ny = y;
    for (int i = 0; i < 1; i++) {
        name = 0;
        for (int j = 0; j < nums.size(); j++) {
            name = name << 3;
            name = name | (nums[j] + i);
        }
    }
    return name;
}

bool check(int x, int y, int type) {
    int nx, ny;
    nx = x;
    ny = y;

    for (int i = 0; i < move_vec[type].size(); i++) {
        nx = nx + move_vec[type][i].first;
        ny = ny + move_vec[type][i].second;

        if (visited[nx][ny] == 1)
            return false;
    }
    nx = x;
    ny = y;
    for (int i = 0; i < move_vec[type].size(); i++) {
        nx = nx + move_vec[type][i].first;
        ny = ny + move_vec[type][i].second;
        visited[nx][ny] = 1;
    }

    return true;
}

Result putPuzzle(int mPuzzle[3][3])
{
    int type, x, y;
    if (mPuzzle[2][2]) {
        type = 4;
    }
    else if (mPuzzle[2][0]) {
        type = 2;
    }
    else if (mPuzzle[0][2]) {
        type = 1;
    }
    else if (mPuzzle[0][1] && mPuzzle[1][2]) {
        type = 3;
    }
    else {
        type = 0;
    }
    int name = get_name(mPuzzle, type);


    while (true) {
        if (idxs[type][name] >= blocks[type][name].size()) {
            break;
        }
        P B = blocks[type][name][idxs[type][name]];
        idxs[type][name] += 1;
        x = B.first;
        y = B.second;
        int success = check(x, y, type);
        if (success) {
            Result ret = { x, y };
            return ret;
        }

    }
    Result ret = { -1, -1 };
    return ret;
}

void clearPuzzles()
{

    memset(idxs, 0, sizeof(idxs));
    memset(visited, 0, sizeof(int) * 1200);
    return;
}


#define CMD_INIT 1
#define CMD_PUT 2
#define CMD_CLR 3


static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);
        if (query == CMD_INIT)
        {
            int mRows, mCols;
            int mCells[MAX_ROW][MAX_COL];
            scanf("%d %d", &mRows, &mCols);
            for (int i = 0; i < mRows; i++){
                for (int j = 0; j < mCols; j++){
                    scanf("%d", &mCells[i][j]);
                }
            }
            init(mRows, mCols, mCells);
            ok = true;
        }
        else if (query == CMD_PUT)
        {
            char strPuzzle[10];
            int mPuzzle[3][3];
            int ans_row, ans_col;
            scanf("%s", strPuzzle);
            int cnt = 0;
            for (int i = 0; i < 3; i++){
                for (int j = 0; j < 3; j++){
                    mPuzzle[i][j] = strPuzzle[cnt] - '0';
                    cnt++;
                }
            }
            Result ret = putPuzzle(mPuzzle);
            scanf("%d %d", &ans_row, &ans_col);
            if (ans_row != ret.row || ans_col != ret.col)
            {
                ok = false;
            }
        }
        else if (query == CMD_CLR)
        {
            clearPuzzles();
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