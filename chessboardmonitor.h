#ifndef CHESSBOARDMONITOR_H
#define CHESSBOARDMONITOR_H

#include <QWidget>
#include "chessboard.h"
#include "dot.h"
namespace Ui {
class ChessBoardMonitor;
}

class ChessBoardMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoardMonitor(QWidget *parent = 0);
    ~ChessBoardMonitor();

    void init();

    void setChessBoard(ChessBoard chessBoard);
    void setDot(const Dot &dot);

    bool display_type[10];
    int m_controlChess;

private slots:
    void on_cb_succeed_stateChanged(int arg1);

    void on_cb_active4_stateChanged(int arg1);

    void on_cb_death4_stateChanged(int arg1);

    void on_cb_active3_stateChanged(int arg1);

    void on_cb_death3_stateChanged(int arg1);

    void on_cb_active2_stateChanged(int arg1);

private:
    Ui::ChessBoardMonitor *ui;
};

#endif // CHESSBOARDMONITOR_H
