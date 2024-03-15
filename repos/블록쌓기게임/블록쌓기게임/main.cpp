#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <algorithm>
#include <memory.h>
#include <iostream>
using namespace std;
struct Result
{
    int top;
    int count;
};

typedef pair<int, int> R;

int ptr = 0;
long long cnt;

class Node {
public:
    int sum;
    int start;
    int end;
    int lazy;
    int top;
    int flr;
    
    Node* lft;
    Node* rgt;

    Node (){}

    Node(int _start, int _end) {
        sum = 0;
        start = _start;
        end = _end;
        lazy = 0;
        top = 0;
        flr = 0;
        lft = nullptr;
        rgt = nullptr;
    }
    void set(int _start, int _end) {
        sum = 0;
        start = _start;
        end = _end;
        lazy = 0;
        top = 0;
        flr = 0;
        lft = nullptr;
        rgt = nullptr;
    }
    void add_blocks(int _start, int _end) {
        //cout << _start << " " << _end << endl;
        if (start == _start && end == _end) {
            top += 1;
            flr += 1;
            if (start != end) {
                lazy += 1;
            }
            return;
        }
        int mid = (end + start) / 2;
        //cout << mid << endl;
        if (lazy) {
            lft->lazy += lazy;
            lft->top += lazy;
            lft->flr += lazy;
            rgt->lazy += lazy;
            rgt->top += lazy;
            rgt->flr += lazy;
            lazy = 0;
        }
        if (_end <= mid) {
            lft->add_blocks(_start, _end);
            
        }
        else if (mid < _start) {
            rgt->add_blocks(_start, _end);
        }
        else {
            lft->add_blocks(_start, mid);
            rgt->add_blocks(mid + 1, _end);
        }
        top = max(rgt->top, lft->top);
        flr = min(rgt->flr, lft->flr);
        return;

    }
};

Node nodes[3000000];

Node* make_nodes(int _start, int _end) {
    Node* P = &nodes[ptr++];
    P->set(_start, _end);
    //cout << _start << " " << _end << endl;

    if (_start == _end) return P;

    P->lft = make_nodes(_start, (_end + _start) / 2);
    P->rgt = make_nodes((_end + _start) / 2 + 1, _end);
    
    return P;

}


Node* root;
int _C;
void init(int C)
{   
    ptr = 0;
    cnt = 0;
    root = make_nodes(0, C - 1);
    _C = C;
}


Result dropBlocks(int mCol, int mHeight, int mLength)
{
    int start = mCol;
    int end = start + mLength - 1;
    cnt += mHeight * mLength;
    Result ret;
    for (int i = 0; i < mHeight; i++) {
        root->add_blocks(start, end);
        
        ret.top = root->top - root->flr;
        ret.count = ( cnt - _C * root->flr) % 1000000;
        
    }
    // cout << ret.top << " " << ret.count << endl;
    //print();
    return ret;
}


#define CMD_INIT 100
#define CMD_DROP 200

static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int ans_top, ans_count;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);
        if (query == CMD_INIT)
        {
            int C;
            scanf("%d", &C);
            init(C);
            ok = true;
        }
        else if (query == CMD_DROP)
        {
            int mCol, mHeight, mLength;
            scanf("%d %d %d", &mCol, &mHeight, &mLength);
            Result ret = dropBlocks(mCol, mHeight, mLength);
            scanf("%d %d", &ans_top, &ans_count);
            if (ans_top != ret.top || ans_count != ret.count)
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