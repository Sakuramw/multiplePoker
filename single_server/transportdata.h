﻿#ifndef TRANSPORTDATA
#define TRANSPORTDATA

/*
 * 聊天 ：100
 *  日志：101
 *
 *

    开始阶段： 1
        C2S:
            名字 11
            准备：12
        S2C：

   f
     全部人都准备阶段：2
        s2c
            座位号;21
            从谁开始22
    游戏进行中（德州）：3 用一个类容器 保存还在玩的玩家，弃牌的玩家就移除
        C2S：
            哪个玩家
            押注
            过
            弃牌
        S2C://向所有玩家广播
            哪个玩家
            押注
            过
            弃牌
            下一个玩家
    结算阶段：4
        S2C:
            获胜方1
            积分变化
    回到投票开始阶段进行循环。


*/
//struct c2sConSer{
//    quint8 requestType = 1;
//    QString name = "";
//};
//struct c2sVote{
//    quint8 requestType = 2;
//    bool isOK = true;
//};

//struct c2sPlaying{
//    quint8 requestType = 3;
//    int money;//大数得用string
//    bool isPass = false;
//    bool isGiveup = false;

//};
struct tranData{
    quint8 requestType;
    QString playerName;
    int playerId;
    bool isOK;
    int beginInWho;
    int money;
    bool isPass;
    bool isGiveup;
    int score;
    int winnerNo;

};
















#endif // TRANSPORTDATA

