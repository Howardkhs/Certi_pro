#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
/////////////////////////////////////

#define MAX_ROW 40
#define MAX_COL 30

#include <vector>
#include <iostream>

using namespace std;

struct P {
    int x;
    int y;
    P(int _x, int _y) {
        x = _x; y = _y;
    }
};

struct Result {
    int row;
    int col;
};

vector<P> type1[5][5];
vector<P> type2[5][5][5];
vector<P> type3[5][5][5];
vector<P> type4[5][5][5][5];
vector<P> type5[5][5][5][5][5];

int type1_id[5][5];
int type2_id[5][5][5];
int type3_id[5][5][5];
int type4_id[5][5][5][5];
int type5_id[5][5][5][5][5];

vector<P> path[6] = { {{0,0}}, { { 0,0 }, { 0,1 } }, { { 0,0 }, { 0,1 } ,{ 0,1 } },
                    { { 0,0 }, { 1,0 }, { 1,0 } }, { { 0,0 }, { 0,1 }, { 1,0 }, { 0,1 } },
                      { { 0,0 }, { 1,0 }, { 0,1 }, { 0,1 }, { 1,0 }} };

int (* graph)[30];   // y열이여야 포인터 크기가 맞다.
int visited[40][30];

int H, W;
vector<int> temp;   int fail = 0;

void set_temp() {
    int min_n = *min_element(temp.begin(), temp.end());

    for (int i = 0; i < temp.size(); i++) {
        temp[i] -= min_n;
    }
}

void type_clear() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            type1[i][j].clear();
            for (int k = 0; k < 5; k++) {
                type2[i][j][k].clear();
                type3[i][j][k].clear();
                for (int l = 0; l < 5; l++) {
                    type4[i][j][k][l].clear();
                    for (int m = 0; m < 5; m++) {
                        type5[i][j][k][l][m].clear();
                    }
                }
            }            
        }
    }
    memset(visited, 0, sizeof visited);
    memset(type1_id, 0, sizeof type1_id);
    memset(type2_id, 0, sizeof type2_id);
    memset(type3_id, 0, sizeof type3_id);
    memset(type4_id, 0, sizeof type4_id);
    memset(type5_id, 0, sizeof type5_id);
}

void init(int mRows, int mCols, int mCells[MAX_ROW][MAX_COL])
{
    graph = mCells;
    H = mRows; W = mCols;
    type_clear();

    int a, b, c, d, e;
    
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int x = i; int y = j;
        
            int nx = x; int ny = y;
            temp.clear(); fail = 0;

            for (int l = 0; l < path[1].size(); l++) {
                nx = nx + path[1][l].x; ny = ny + path[1][l].y;
                if (nx < 0 || ny < 0 || nx >= H || ny >= W) {
                    fail = 1;
                    break; 
                }
                temp.push_back(graph[nx][ny]);
            }
            if (!fail) {
                set_temp();
                a = temp[0]; b = temp[1];
                type1[a][b].push_back({ i, j });
            }

            nx = x; ny = y;
            temp.clear(); fail = 0;

            for (int l = 0; l < path[2].size(); l++) {
                nx = nx + path[2][l].x; ny = ny + path[2][l].y;
                if (nx < 0 || ny < 0 || nx >= H || ny >= W) {
                    fail = 1;
                    break;
                }
                temp.push_back(graph[nx][ny]);
            }  
            if (!fail) {
                set_temp();
                a = temp[0]; b = temp[1]; c = temp[2];
                type2[a][b][c].push_back({i, j});
            }

            nx = x; ny = y;
            temp.clear(); fail = 0;

            for (int l = 0; l < path[3].size(); l++) {
                nx = nx + path[3][l].x; ny = ny + path[3][l].y;
                if (nx < 0 || ny < 0 || nx >= H || ny >= W) {
                    fail = 1;
                    break;
                }
                temp.push_back(graph[nx][ny]);
            }
            if (!fail) {
                set_temp();
                a = temp[0]; b = temp[1]; c = temp[2];
                type3[a][b][c].push_back({i, j});
            }

            nx = x; ny = y;
            temp.clear(); fail = 0;

            for (int l = 0; l < path[4].size(); l++) {
                nx = nx + path[4][l].x; ny = ny + path[4][l].y;
                if (nx < 0 || ny < 0 || nx >= H || ny >= W) {
                    fail = 1;
                    break;
                }
                temp.push_back(graph[nx][ny]);
            }
            if (!fail) {
                set_temp();
                a = temp[0]; b = temp[1]; c = temp[2]; d = temp[3];
                type4[a][b][c][d].push_back({i, j});
            }

            nx = x; ny = y;
            temp.clear(); fail = 0;

            for (int l = 0; l < path[5].size(); l++) {
                nx = nx + path[5][l].x; ny = ny + path[5][l].y;
                if (nx < 0 || ny < 0 || nx >= H || ny >= W) {
                    fail = 1;
                    break;
                }
                temp.push_back(graph[nx][ny]);
            }
            if (!fail) {
                set_temp();
                a = temp[0]; b = temp[1]; c = temp[2]; d = temp[3]; e = temp[4];
                type5[a][b][c][d][e].push_back({i, j});
            }
        }
    }
   
}

int set_type(int m[3][3]) {
    
    if (m[2][2])    return 5;
    if (m[1][1])    return 4;
    if (m[2][0])    return 3;
    if (m[0][2])    return 2;
    else            return 1;

}

void get_temp(int type, int mPuzzle[3][3]) {

    temp.clear();
    
    int x = 0; int y = 0;

    for (int i = 0; i < path[type].size(); i++) {
        x = x + path[type][i].x; y = y + path[type][i].y;
        temp.push_back(mPuzzle[x][y]);
    }
    int min_n = *min_element(temp.begin(), temp.end());
    for (int i = 0; i < temp.size(); i++) {
        temp[i] -= min_n;
    }
}

int check_path(int x, int y, int type) {
    for (int i = 0; i < path[type].size(); i++) {
        x = x + path[type][i].x; y = y + path[type][i].y;
        if (visited[x][y])  return 0;
    }
    return 1;
}

void set_path(int x, int y, int type) {
    for (int i = 0; i < path[type].size(); i++) {
        x = x + path[type][i].x; y = y + path[type][i].y;
        visited[x][y] = 1;
    }
}

Result putPuzzle(int mPuzzle[3][3])
{
    int type;
    type = set_type(mPuzzle);

    get_temp(type, mPuzzle);

    int a, b, c, d, e, x, y, find = 0;

    if (type == 1) {
        a = temp[0]; b = temp[1];
        int idx = type1_id[a][b];
        while (idx < type1[a][b].size()) {
            P* N = &type1[a][b][idx];
            x = N->x; y = N->y;
            if (check_path(x, y, type)) {
                set_path(x, y, type);
                idx += 1;
                type1_id[a][b] = idx;
                find = 1;
                break;
            };
            idx += 1;
        }
        type1_id[a][b] = idx;
    }
    else if (type == 2) {
        a = temp[0]; b = temp[1]; c = temp[2];
        int idx = type2_id[a][b][c];
        while (idx < type2[a][b][c].size()) {
            P* N = &type2[a][b][c][idx];
            x = N->x; y = N->y;
            if (check_path(x, y, type)) {
                set_path(x, y, type);
                idx += 1;
                type2_id[a][b][c] = idx;
                find = 1;
                break;
            };
            idx += 1;
        }
        type2_id[a][b][c] = idx;
    }
    else if (type == 3) {
        a = temp[0]; b = temp[1]; c = temp[2];
        int idx = type3_id[a][b][c];
        while (idx < type3[a][b][c].size()) {
            P* N = &type3[a][b][c][idx];
            x = N->x; y = N->y;
            if (check_path(x, y, type)) {
                set_path(x, y, type);
                idx += 1;
                type3_id[a][b][c] = idx;
                find = 1;
                break;
            };
            idx += 1;
        }
        type3_id[a][b][c] = idx;
    }
    else if (type == 4) {
        a = temp[0]; b = temp[1]; c = temp[2]; d = temp[3];
        int idx = type4_id[a][b][c][d];
        while (idx < type4[a][b][c][d].size()) {
            P* N = &type4[a][b][c][d][idx];
            x = N->x; y = N->y;
            if (check_path(x, y, type)) {
                set_path(x, y, type);
                idx += 1;
                type4_id[a][b][c][d] = idx;
                find = 1;
                break;
            };
            idx += 1;
        }
        type4_id[a][b][c][d] = idx;
    }
    else if (type == 5) {
        a = temp[0]; b = temp[1]; c = temp[2]; d = temp[3]; e = temp[4];
        int idx = type5_id[a][b][c][d][e];
        while (idx < type5[a][b][c][d][e].size()) {
            P* N = &type5[a][b][c][d][e][idx];
            x = N->x; y = N->y;
            if (check_path(x, y, type)) {
                set_path(x, y, type);
                idx += 1;
                type5_id[a][b][c][d][e] = idx;
                find = 1;
                break;
            };
            idx += 1;
        }
        type5_id[a][b][c][d][e] = idx;
    }
    Result answer;


    if (!find) { 
        answer.row = -1; answer.col = -1;
        return answer;
    }
    answer.row = x; answer.col = y;
   // cout << x << " " << y << endl;
    return answer;
}

void clearPuzzles()
{
    memset(visited, 0, sizeof visited);
    memset(type1_id, 0, sizeof type1_id);
    memset(type2_id, 0, sizeof type2_id);
    memset(type3_id, 0, sizeof type3_id);
    memset(type4_id, 0, sizeof type4_id);
    memset(type5_id, 0, sizeof type5_id);
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
            for (int i = 0; i < mRows; i++) {
                for (int j = 0; j < mCols; j++) {
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
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
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