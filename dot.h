#ifndef DOT_H
#define DOT_H

#include <QPoint>
#include <QDebug>

#include "chessboard.h"
#define DOT_SUCCEET 0
#define DOT_ACTIVE4 1
#define DOT_DEATH4 2
#define DOT_ACTIVE3 3
#define DOT_DEATH3 4
#define DOT_ACTIVE2 5
class Dot;


struct DotMessage{
    int white[10];
    int black[10];

    DotMessage(){
        memset(white,0,sizeof(white));
        memset(black,0,sizeof(black));
    }

    void init(){
        memset(white,0,sizeof(white));
        memset(black,0,sizeof(black));
    }


};



class Dot
{
public:
    Dot();
    void init();

    bool setBoard(const ChessBoard &board, const QPoint &point);
    DotMessage getDotMessage()const;
    bool isDotNoMessage(const int &color)const;

    int getDotStatusByColor(const int &color) const;
    int getDotScore()const;
    int getDotScore(const int &color)const;

    bool operator <(const Dot &dot)const;
    bool operator == (const Dot &dot)const;


    int getEmmironmentScore()const;

    DotMessage dot_mes;
    QPoint chess;
    bool isDotBan(const int &color)const;
    bool changlian;
    static bool compareDotScore(const Dot &dot1,const Dot &dot2);

private:

    void DotScan0(const int &judge,bool &f4);
    void DotScan1(const int &judge,bool &f4);
    void DotScan2(const int &judge,bool &f4);
    void DotScan3(const int &judge,bool &f4);
    void DotScan4(const int &judge,bool &f4);
    void DotScan5(const int &judge,bool &f4);

    void LineScan(ChessLine line);
    int getLineToJudge(const ChessLine &line,const int &left,const int &right);
    int getNumber(const int &number);
    void AddTypeCountOfColor(const int &type,const int &color);

    int w_tiaosan;
    int w_tiaosi;
    int b_tiaosan;
    int b_tiaosi;

};

#endif // DOT_H
