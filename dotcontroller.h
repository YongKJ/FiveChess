#ifndef DOTCONTROLLER_H
#define DOTCONTROLLER_H

#include "chessboard.h"
#include "dot.h"

struct DotStatus{
    int status;
    int senior;
    Dot dot;

    bool operator <(const DotStatus &d)const
    {
        if(d.dot.chess.x()==dot.chess.x())
        {
            return d.dot.chess.y()>dot.chess.y();
        }
        return d.dot.chess.x()>dot.chess.x();
    }
    bool operator == (const DotStatus &d)const
    {
        if(d.dot.chess.x()==dot.chess.x()&&dot.chess.y()==d.dot.chess.y())
        {
            return true;
        }
        return false;
    }
};

struct DotMap
{
    Dot map[15][15];

    void init(){
        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                map[i][j].init();
                map[i][j].chess =  QPoint(i,j);
            }
        }
    }
};
class DotController
{
public:
    DotController();

    Board m_board;
    DotMap m_chessDot;

    int m_succeed;
    int m_ssucceed4;

    int e_status;
    int c_status;

    void init();
    void setChessBoard(const ChessBoard &board,const QPoint &chess);
    void setUnChessBoard(const ChessBoard &board,const QPoint &chess);
    bool isPointValid(const QPoint &point)const;
    bool isAlreadySearch(const QPoint &point) const;
    bool isDotHas(const QPoint &chess, const int &color, const int &type)const;

    Dot getDotByPoint(const QPoint &point);
    static bool compareDotStatus(const DotStatus &sta1, const DotStatus &sta2);
    static QList<DotStatus> DeleteDotByStatus(QList<DotStatus>dotlist,const int &status);
    //重新扫描并获取棋盘上点的数据
    QList<DotStatus> OverSearch(const int &color);


private:

    void setIllegalOfPoint(const QPoint &point);
    void setOpenOfPoint(const QPoint &point);

};

#endif // DOTCONTROLLER_H
