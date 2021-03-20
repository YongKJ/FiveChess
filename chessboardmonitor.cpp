#include "chessboardmonitor.h"
#include "ui_chessboardmonitor.h"

ChessBoardMonitor::ChessBoardMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessBoardMonitor)
{
    ui->setupUi(this);
    init();
}

ChessBoardMonitor::~ChessBoardMonitor()
{
    delete ui;
}

void ChessBoardMonitor::init()
{
    for(int i = 0;i<10;i++){
        display_type[i] = false;
    }
    m_controlChess = CHESS_black;
}

void ChessBoardMonitor::setChessBoard(ChessBoard chessBoard)
{
    if(chessBoard.isGameClose()){
       ui->lb_closeFlag->setText("结束");
       ui->lb_winner->setText(QString::number(chessBoard.getWinner()));
    }else{
        ui->lb_closeFlag->setText("仍未结束");
        ui->lb_winner->setText("暂无");
    }

    ui->lb_round->setText(QString::number(chessBoard.getCurrentRound()));

    QPoint chess = chessBoard.getLastChess();

    ui->lb_lastPoint->setText(QString::number(chess.x())+","+QString::number(chess.y()));
    if(chessBoard.getNextChessColor()==1){
        ui->lb_nowPlayer->setText("黑色");
    }
    else{
        ui->lb_nowPlayer->setText("白色");
    }


}

void ChessBoardMonitor::setDot(const Dot &dot)
{
    ui->lb_c_chess->setText(QString::number(dot.chess.x())+","+QString::number(dot.chess.y()));
    DotMessage mes = dot.getDotMessage();
    ui->lb_b_succeed->setText(QString::number(mes.black[0]));
    ui->lb_b_active4->setText(QString::number(mes.black[1]));
    ui->lb_b_death4->setText(QString::number(mes.black[2]));
    ui->lb_b_active3->setText(QString::number(mes.black[3]));
    ui->lb_b_death3->setText(QString::number(mes.black[4]));
    ui->lb_b_active2->setText(QString::number(mes.black[5]));

    ui->lb_w_succeed->setText(QString::number(mes.white[0]));
    ui->lb_w_active4->setText(QString::number(mes.white[1]));
    ui->lb_w_death4->setText(QString::number(mes.white[2]));
    ui->lb_w_active3->setText(QString::number(mes.white[3]));
    ui->lb_w_death3->setText(QString::number(mes.white[4]));
    ui->lb_w_active2->setText(QString::number(mes.white[5]));

    ui->lb_score->setText(QString::number(dot.getDotScore()));
    int b_status = dot.getDotStatusByColor(CHESS_black);
    ui->lb_b_status->setText(Config::getStatusIntToString(b_status));

    int w_status = dot.getDotStatusByColor(CHESS_white);

    ui->lb_w_status->setText(Config::getStatusIntToString(w_status));

}

void ChessBoardMonitor::on_cb_succeed_stateChanged(int arg1)
{
    display_type[0] = (bool) arg1;
}

void ChessBoardMonitor::on_cb_active4_stateChanged(int arg1)
{
    display_type[1] = (bool) arg1;
}

void ChessBoardMonitor::on_cb_death4_stateChanged(int arg1)
{
    display_type[2] = (bool) arg1;
}

void ChessBoardMonitor::on_cb_active3_stateChanged(int arg1)
{
    display_type[3] = (bool) arg1;
}

void ChessBoardMonitor::on_cb_death3_stateChanged(int arg1)
{
    display_type[4] = (bool) arg1;
}

void ChessBoardMonitor::on_cb_active2_stateChanged(int arg1)
{
    display_type[5] = (bool) arg1;
}
