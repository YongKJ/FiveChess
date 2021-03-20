#ifndef COMPUTER_H
#define COMPUTER_H

#include "chessboard.h"
#include "dotcontroller.h"

#define URGENT 1000
enum{
    ATTRACT,
    ACTIVE,
    DEFENT
};
struct ComputerDot{
    int status;
    int winner;
    int cu;
    int score;
    QPoint chess;
};

//智能人机
class Computer
{
public:
    int c_score;
    int e_score;
    QList<QPoint>pointlist;
    Computer();
    void init();

    void ComputerUndoUpdatePlayChess(const ChessBoard &board,const QPoint &chess);
    void ComputerupdatePlayChess(const ChessBoard &board,const QPoint &chess);
    QPoint playChess(const ChessBoard &board,const int &color);
    void getOpeningStart();
    void getFiveRoundNPlay(const ChessBoard &board,const int &color);
    bool VCF(ChessBoard board, DotController control, const QPoint &chess, int deep, int cu);

    static ComputerDot ComputerSearch(ChessBoard board, DotController control, const QPoint &chess, const int &deep, const int &controlColor, int cu, int &alCU);
    static compareComputerDot(const ComputerDot &com1,const ComputerDot &com2);
    DotController dotControl;



};

#endif // COMPUTER_H
