#include "computer.h"

Computer::Computer()
{

}

void Computer::init()
{
  c_score = 0;
  e_score = 0;
  dotControl.init();

}

void Computer::ComputerUndoUpdatePlayChess(const ChessBoard &board, const QPoint &chess)
{
   dotControl.setUnChessBoard(board,chess);
}

void Computer::ComputerupdatePlayChess(const ChessBoard &board, const QPoint &chess)
{
    dotControl.setChessBoard(board,chess);
}

QPoint Computer::playChess(const ChessBoard &board, const int &color)
{
    //开局库
    if(board.getCurrentRound()<=2){
        getOpeningStart();
        return QPoint(-1,-1);
    }

    if(board.getCurrentRound()==4)
    {
        getFiveRoundNPlay(board,color);
        return QPoint(-1,-1);
    }

    QList<DotStatus>dotlist = dotControl.OverSearch(color);
    int cu = 0;

    QList<ComputerDot>comList;

    int Defentflag = false;
    int i = 0;

    //开始循环遍历可下的点，并对可下点进行搜索
    foreach (DotStatus sta, dotlist)
    {
        //如果是胜利点或者是我方的活四点则放弃搜索，节省开支
        if(sta.status==Succeed||(sta.senior==1&&sta.status==SSucceed4))
        {
            return sta.dot.chess;
        }
        if(sta.status==SSucceed4&&sta.senior==0)
        {
            Defentflag = true;
        }
        if(Defentflag&&sta.status<Attack4)
        {
            break;
        }

        //我方冲四攻击点，展开搜索
        if(sta.status==SAttact4||sta.status==Attack4)
        {
//            //冲四攻击必然胜利,下棋，因为是冲四，所以无需找到最优，一旦发现，即刻返回。
//            if(VCF(board,dotControl,sta.dot.chess,7,cu))
//            {
//                qDebug()<<"发现最优";
//                return sta.dot.chess;
//            }
//            else
//            {
                ComputerDot dot = ComputerSearch(board,dotControl,sta.dot.chess,5,color,1,cu);
                dot.score = -sta.dot.getDotScore(color);
                comList.append(dot);
                continue;
//            }
        }

        ComputerDot dot = ComputerSearch(board,dotControl,sta.dot.chess,5,color,1,cu);
        dot.score = -sta.dot.getDotScore(color);
        comList.append(dot);

        //统计搜索数据
        cu++;
        qDebug()<<cu<<":节点搜索完毕";
        i++;
        if(i>10)
        {
            break;
        }
    }
    qSort(comList.begin(),comList.end(),Computer::compareComputerDot);
    foreach (ComputerDot dot, comList) {
        qDebug()<<dot.winner<<" "<<dot.cu;
        qDebug()<<"状态："<<dot.status;
        qDebug()<<"分数："<<dot.score;
        qDebug()<<dot.chess;
        qDebug()<<"------------------";
    }
    qDebug()<<comList.length();
//    i = 0;
//    foreach (DotStatus c, dotlist) {
//        qDebug()<<c.dot.chess;
//        qDebug()<<"状态:"<<Config::getStatusIntToString(c.status);
//        qDebug()<<"ser:"<<c.senior;
//        qDebug()<<"sc:"<<c.dot.getDotScore();
//        i++;
//        if(i>10) break;
//    }

    return comList.at(0).chess;
    return dotlist.at(0).dot.chess;
}

void Computer::getOpeningStart()
{
    pointlist.clear();

    pointlist.append(QPoint(7,7));
    pointlist.append(QPoint(8,8));
    pointlist.append(QPoint(8,7));
}

void Computer::getFiveRoundNPlay(const ChessBoard &board,const int &color)
{
    pointlist.clear();
    QList<DotStatus>dotlist = dotControl.OverSearch(color);

    pointlist.append(dotlist.at(0).dot.chess);
    pointlist.append(dotlist.at(1).dot.chess);
}

bool Computer::VCF(ChessBoard board, DotController control, const QPoint &chess, int deep, int cu)
{
    cu++;
    board.playChess(chess);
    control.setChessBoard(board,chess);

    //对方下棋
    QList<DotStatus>dotlist = control.OverSearch(board.getNextChessColor());

    DotStatus e = dotlist.at(0);

    if(e.dot.isDotBan(board.getNextChessColor()))
    {
        //对方下棋点是禁手，胜利
        return true;
    }

    board.playChess(e.dot.chess);
    control.setChessBoard(board,e.dot.chess);
    dotlist = control.OverSearch(board.getNextChessColor());

    foreach (DotStatus sta,dotlist ) {
        if(sta.status<Attack4) return false;
        if((sta.status==SAttact4&&sta.status==SSucceed4||sta.status==Succeed)&&sta.senior==1) return true;

        if(deep>0&&sta.status==Attack4&&sta.senior==1)
        {
            if(VCF(board,control,sta.dot.chess,deep-2,cu))
            {
                return true;
            }
        }
    }
    return false;
}

ComputerDot Computer::ComputerSearch(ChessBoard board, DotController control, const QPoint &chess, const int &deep, const int &controlColor,int cu,int &alCU)
{

    board.playChess(chess);
    control.setChessBoard(board,chess);
    ComputerDot com;
    if(board.isGameClose())
    {
        com.chess = chess;
        com.winner = 0;
        com.cu = cu;
        com.score = -1;
        com.status = ACTIVE;
        return com;
    }
    int color = board.getNextChessColor();


    QList<DotStatus>dotlist = control.OverSearch(color);
    if(control.m_ssucceed4>0) dotlist = DotController::DeleteDotByStatus(dotlist,SAttact4);
    if(deep==0)
    {
        DotStatus s= dotlist.at(0);

        if((s.status==Succeed||s.status==SSucceed4)&&s.senior==1)
        {
            com.winner = -1;
            com.cu = cu;
            com.status = ATTRACT;
            com.score = s.dot.getDotScore(color);
            com.chess = chess;
        }
        else if((s.status==Succeed||s.status==SSucceed4)&&s.senior==0)
        {
            if(s.status==Succeed&&s.dot.isDotBan(color))
            {
                com.winner = 1;
            }
            else
            {
                com.winner = 0;
            }
            com.cu = cu;
            com.status = DEFENT;
            com.score = s.dot.getDotScore(color);
            com.chess = chess;
        }
        else if(s.status==SAttact3&&s.senior==1)
        {
            com.winner = -1;
            com.cu = cu;
            com.status =ATTRACT;
            com.score = s.dot.getDotScore(color);
            com.chess = chess;
        }
        else if(s.status>=Attack3)
        {
            com.winner = 0;
            com.cu = cu;
            com.status =ATTRACT;
            com.score = s.dot.getDotScore(color);
            com.chess = chess;
        }
        else
        {
            com.winner = 0;
            com.cu = cu;
            com.status =ACTIVE;
            com.score = s.dot.getDotScore(color);
            com.chess = chess;
        }
        return com;
    }

    int DefentFlag = false;
    QList<ComputerDot>comlist;
    int i=0;
    foreach (DotStatus sta,dotlist)
    {
        //如果有我方点胜利或者攻击43，则我方胜利，直接返回
        if((sta.status==Succeed||sta.status==SSucceed4)&&sta.senior==1)
        {
            com.winner = -1;
            com.cu = cu;
            com.status = ATTRACT;
            com.score = sta.dot.getDotScore(color);
            com.chess = chess;
            return com;
        }
        if(sta.status==Succeed&&sta.senior==0)
        {
            if(sta.dot.isDotBan(color))
            {
                com.winner = 1;
            }
            else
            {
                com.winner = 0;
            }
            com.cu = cu;
            com.status = DEFENT;
            com.score = sta.dot.getDotScore(color);
            com.chess = chess;
            return com;
        }

        if(sta.status==SSucceed4&&sta.senior==0)
        {
            DefentFlag = true;
        }
        if(DefentFlag&&sta.status<Attack4)
        {
            break;
        }
        if(DefentFlag&&sta.status==SAttact3)
        {
            continue;
        }

        com = ComputerSearch(board,control,sta.dot.chess,deep-1,controlColor,cu+1,alCU);
        comlist.append(com);
        alCU++;
        i++;
        if(i>6)
        {
            break;
        }
    }
    qSort(comlist.begin(),comlist.end(),Computer::compareComputerDot);
    com = comlist.at(0);
    foreach (DotStatus sta,dotlist){
        if(sta.dot.chess==com.chess)
        {
            com.chess = chess;
            com.winner = -com.winner;
            com.score = sta.dot.getDotScore(color);
            if(sta.status>=Attack3&&sta.senior==1)
            {
                com.status = ATTRACT;
            }
            else if (sta.senior==0&&sta.status>=Attack3)
            {
                com.status = DEFENT;
            }
            else
            {
                com.status = ACTIVE;
            }
            return com;
        }

    }

    return comlist.at(0);
}

Computer::compareComputerDot(const ComputerDot &com1, const ComputerDot &com2)
{
    if(com1.winner>com2.winner)
    {
        return true;
    }
    else if(com1.winner==com2.winner&&com1.winner==1)
    {
        return com1.cu<=com2.cu;
    }
    else if(com1.winner==com2.winner&&com1.winner==-1)
    {
        return com1.cu>com2.cu;
    }
    else if(com1.winner==com2.winner&&com1.winner==0)
    {
        if(com1.status>com2.status)
        {
            return true;
        }
        else if(com1.status==com2.status)
        {
            return com1.score<com2.score;
        }
    }
    return false;
}
