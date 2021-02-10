#ifndef TRANSPORTDATA
#define TRANSPORTDATA

/*
    (新玩家加入：
        S2C：
          现在人数)

    连接服务器： 1
        C2S:
            名字；
        S2C：
            座位号;
            现有人数；
    投票开始阶段： 2
        C2S:
            准备信号
        S2C：
            从谁开始/也当做开始信号
    游戏进行中（德州）：用一个类容器 保存还在玩的玩家，弃牌的玩家就移除  3
        C2S：
            押注
            过
            弃牌
        S2C://向所有玩家广播
            座位号
            押注
            过
            弃牌
    结算阶段： 4
        S2C:
            获胜方
            积分变化
    回到投票开始阶段进行循环。


*/
struct c2sConSer{
    quint8 requestType = 1;
    QString name = "";
};
struct c2sVote{
    quint8 requestType = 2;
    bool isOK = true;
};

struct c2sPlaying{
    quint8 requestType = 3;
    int money;//大数得用string
    bool isPass = false;
    bool isGiveup = false;

};

















#endif // TRANSPORTDATA

