#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

#define MAX_USER 500
#define MAX_CHANNEL 500
#define MAX_NEWS 30000

struct News {
    int newsId;
    int channelId;
    int time;
    bool isDeleted;

    News() {};

    News(int _newsId, int _channelId, int _time) {
        newsId = _newsId;
        channelId = _channelId;
        time = _time;
        isDeleted = false;
    }
}newsPool[MAX_NEWS];
int newsIndex;

struct cmp {
    // time이 낮을수록, newId가 낮을수록 우선순위 큼
    bool operator()(const News* a, const News* b) {
        if (a->time != b->time) return !(a->time < b->time);
        return !(a->newsId < b->newsId);
    }
};

vector< int > channelArr[MAX_CHANNEL + 1];              // key: 채널ID, value: 해당 채널을 구독하고 있는 유저ID리스트
vector< News*>  userArr[MAX_USER + 1];                 // key: 유저ID, value: 해당 유저가 받은 뉴스들
News* newsArr[MAX_NEWS + 1];                            // key: 뉴스ID, value: 해당 뉴스 저장
priority_queue< News*, vector< News*>, cmp > notiPQ;   // 보낼 뉴스들 저장(시간 낮은 뉴스 우선)

// 1~10억 범위를 배열 인덱스로 매핑
unordered_map< int, int > cMapper;
unordered_map< int, int > uMapper;
unordered_map< int, int > nMapper;
int cIndex;
int uIndex;
int nIndex;

// 초기화
void init(int N, int K) {
    newsIndex = 0;
    cIndex = uIndex = nIndex = 0;

    cMapper.clear();
    uMapper.clear();
    nMapper.clear();

    for (int i = 0; i < MAX_USER; i++)
    {
        channelArr[i].clear();
        userArr[i].clear();
    }

    notiPQ = {};
}

// time까지 보내야할 뉴스들 보내기
void sendNews(int time) {
    while (!notiPQ.empty()) {
        if (notiPQ.top()->time > time) return;

        News* news = notiPQ.top(); notiPQ.pop();

        // 삭제된 뉴스인 경우
        if (news->isDeleted) continue;

        // 해당 채널을 구독하고 있는 유저들에게 뉴스 보내기
        vector< int >* userList = &channelArr[news->channelId];
        for (int i : *userList) {
            userArr[i].push_back(news);
        }
    }
}

// 유저가 채널을 구독
void registerUser(int mTime, int mUID, int mNum, int mChannelIDs[]) {
    sendNews(mTime);

    int userID = uMapper[mUID];
    if (userID == 0)
        userID = uMapper[mUID] = ++uIndex;

    // 각 채널 구독자 목록에 해당 유저 추가
    for (int i = 0; i < mNum; i++) {
        int channelId = cMapper[mChannelIDs[i]];
        if (channelId == 0)
            channelId = cMapper[mChannelIDs[i]] = ++cIndex;

        channelArr[channelId].push_back(userID);
    }
}

// 뉴스 등록
int offerNews(int mTime, int mNewsID, int mDelay, int mChannelID) {
    int nID = nMapper[mNewsID] = ++nIndex;
    int channelID = cMapper[mChannelID];

    // 뉴스 생성 후 뉴스 등록
    News* newNews = &newsPool[newsIndex++];
    newNews->newsId = mNewsID;
    newNews->channelId = channelID;
    newNews->time = mTime + mDelay;
    newNews->isDeleted = false;

    notiPQ.emplace(newNews);
    newsArr[nID] = newNews;

    return channelArr[channelID].size();
}

// 뉴스 취소
void cancelNews(int mTime, int mNewsID) {
    sendNews(mTime);

    int nID = nMapper[mNewsID];

    // 삭제 표시
    newsArr[nID]->isDeleted = true;
}

// 유저가 받은 뉴스 확인
int checkUser(int mTime, int mUID, int mRetIDs[]) {
    sendNews(mTime);

    int uID = uMapper[mUID];

    // list의 뒤에서부터는 최신 뉴스가 쌓여있다
    vector< News*>* newsList = &userArr[uID];

    int total = newsList->size();
    int cnt = 0;    // 받은 뉴스 카운트

    while (total > 0) {
        News* news = newsList->at(total - 1);

        // 삭제된 뉴스인 경우
        if (news->isDeleted) {
            total--;
            continue;
        }

        // 최신 뉴스 최대 3개 저장
        if (cnt < 3) mRetIDs[cnt] = news->newsId;

        total--;
        cnt++;
    }

    // 유저가 받았던 뉴스 초기화
    newsList->clear();
    return cnt;
}