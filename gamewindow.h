#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <startdialog.h>

#include "config.h"
#include "chessboard.h"
#include "chessboardmonitor.h"
#include "greatbutton.h"
#include "dot.h"
#include "dotcontroller.h"
#include "computer.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0,int mode = 1);
    ~GameWindow();

    void initWindow(int mode = 0);

    GreatButton *bt_undo;
    GreatButton *bt_tip;
    GreatButton *bt_voice;
    GreatButton *bt_setting;
    GreatButton *bt_start;
    GreatButton *bt_threeSwap;
    GreatButton *bt_nextRound;

    QLabel *lb_ControlColor;
    QLabel *lb_OtherColor;
    QLabel *lb_nowRoundColor;
    QLabel *lb_roundNumber;

    void setControlColor(int color = 1);
    void setNowPlayerColor(int color = 1);
    bool GamePlayChess(const QPoint &chess);
    bool SaveGameOverChessBoard(const ChessBoard &board);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);


private:
    Ui::GameWindow *ui;

    bool drag_flag;
    QPoint drag_flagPoint;
    QPoint boardMap[15][15];

    ChessBoard m_boardControl;
    QPoint game_currentPointOfChess;

    int game_mode = 0;
    bool game_overTipFlag;
    int game_playerControlColor;
    bool game_isStarting;
    bool game_stopflag;

    //控制台
    bool control_flag;
    bool display_alreadySearch;


    ChessBoardMonitor control_monitor;
    QTimer *game_timer;


    void initData();
    void initLayout(int mode = 0);
    void initConnect(int mode = 0);
    void DrawChess();
    void DrawGameOverTip(const int &type);
    void DrawChoosePoint(const QPoint &point);
    void DrawAlreadySearchDot(DotController controller);
    void DrawRequestDisplayDot(DotController controller);
    void DrawSetPoint(DotController controller);
    void DrawFiveRoundNPlayChess(const Computer &cm);



    QPoint getDrawChessPoint(QPoint chess);
    QPoint getChessPointByMouseClick(QPoint click);

    Computer ai;

public slots:
    void undoChess();
    void updateWindow();
    void ComputerPlayChess();
    void GameTimerSlot();
    void ReStart();
    void ThreeSwap();
    void GameGoOn();
signals:
    void TimeToComputerPlayChess();


};

#endif // GAMEWINDOW_H
