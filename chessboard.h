#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QPoint>
#include <QList>

#include "config.h"

#define HLINE 0
#define VLINE 1
#define LLINE 2
#define RLINE 3
struct Board
{
    char map[15][15];

    Board(){
        memset(map,0,sizeof(map));
    }

    void init(){
        memset(map,0,sizeof(map));
    }
};

struct BoardMessage
{
    Board board;
    QList<QPoint> qunue;
};

struct ChessLine
{
    char shape[12];

    ChessLine(){
        memset(shape,0,sizeof(shape));
    }

    void init(){
        memset(shape,0,sizeof(shape));
    }

    ChessLine toPaint(){
        ChessLine line;
        for(int i=0;i<11;i++){
            line.shape[i] = shape[i]+48;
        }
        line.shape[11] = '\0';
        return line;
    }
};



class ChessBoard
{
public:
    ChessBoard();

    void init();
    void setWinner(const int &color);
    void setUnWinner();
    bool playChess(const QPoint &chess);
    bool undoChess();

    bool isPlayChessValid(const QPoint &chess)const;

    bool isGameClose()const;

    int getWinner()const;
    BoardMessage getBoardMessage()const;
    Board getChessBoard()const;
    QList<QPoint> getChessQunue()const;

    QPoint getLastChess()const;
    QPoint getUndoChess()const;

    int getCurrentRound()const;
    int getNextChessColor()const;
    int getChessColorByPoint(QPoint point)const;

    ChessLine getChessLine(const int &type, const QPoint &chess) const;

    static bool isChessPointValid(const QPoint &chess);
    static bool isChessPointValid(const int &x,const int &y);
    static int getChessColorByRound(const int &round);
    static QPoint FindNextQPoint(QPoint chess,const int &type,const int &direct);
    static QList<QPoint> getAreaOfChessPoint(const QPoint &point);




private:
    BoardMessage boardMessage;
    bool game_closeFlag;
    int game_winner;
    QPoint m_undoChess;

    bool WinEvent(const QPoint &point);
    int isColorWinInLine(const ChessLine &line);

signals:
    void appearWinner(int color);


};

#endif // CHESSBOARD_H
