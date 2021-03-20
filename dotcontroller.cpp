#include "dotcontroller.h"

uint qHash(const DotStatus &key){
    return key.dot.chess.x()*100+key.dot.chess.y()*10;
}

DotController::DotController()
{

}

void DotController::init()
{
   m_board.init();
   m_chessDot.init();
   m_succeed = 0;
   m_ssucceed4 = 0;
}

void DotController::setChessBoard(const ChessBoard &board, const QPoint &chess)
{
    QList<QPoint> list = ChessBoard::getAreaOfChessPoint(chess);

    setIllegalOfPoint(chess);

    int count = 0;


    foreach (QPoint point, list) {

        if(!isPointValid(point)) continue;

        setOpenOfPoint(point);

        m_chessDot.map[point.x()][point.y()].setBoard(board,point);
        count++;

    }
//    qDebug()<<"扫描节点:"<<list.length()<<"/"<<count;
}

void DotController::setUnChessBoard(const ChessBoard &board, const QPoint &chess)
{
    QList<QPoint> list = ChessBoard::getAreaOfChessPoint(chess);

    setOpenOfPoint(chess);

    list.append(chess);

    int count = 0;

    foreach (QPoint point, list) {

        if(!isPointValid(point)) continue;

        setOpenOfPoint(point);

        m_chessDot.map[point.x()][point.y()].setBoard(board,point);

        count++;

    }
//    qDebug()<<"扫描节点:"<<list.length()<<"/"<<count;
}

void DotController::setIllegalOfPoint(const QPoint &point)
{
    m_board.map[point.x()][point.y()] = 3;
}

void DotController::setOpenOfPoint(const QPoint &point)
{
    m_board.map[point.x()][point.y()] = 1;
}

bool DotController::isPointValid(const QPoint &point)const
{
    if(m_board.map[point.x()][point.y()]==CHESS_none||m_board.map[point.x()][point.y()]==1){
        return true;
    }
    return false;
}

bool DotController::isAlreadySearch(const QPoint &point)const
{
    if(m_board.map[point.x()][point.y()]==1){
        return true;
    }
    return false;
}

bool DotController::isDotHas(const QPoint &chess, const int &color,const int &type) const
{
    if(!isAlreadySearch(chess)){
        return false;
    }

    if(color==CHESS_black){
        if(m_chessDot.map[chess.x()][chess.y()].dot_mes.black[type]>0){
            return true;
        }
    }
    else{
        if(m_chessDot.map[chess.x()][chess.y()].dot_mes.white[type]>0){
            return true;
        }
    }
    return false;
}

QList<DotStatus> DotController::OverSearch(const int &color)
{
    QList<DotStatus>dotlist;
    int enemy = Config::getEnemyColor(color);
    DotStatus sta;

    m_succeed = 0;
    m_ssucceed4 = 0;

    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            if(!isAlreadySearch(QPoint(i,j))) continue;

            Dot dot = getDotByPoint(QPoint(i,j));

            if(dot.getDotStatusByColor(color)==Succeed)
            {
                sta.dot = dot;
                sta.senior = 1;
                sta.status = Succeed;
                dotlist.append(sta);
                m_succeed++;
                continue;
            }

            if(dot.getDotStatusByColor(enemy)==Succeed)
            {
                sta.dot = dot;
                sta.senior = 0;
                sta.status = Succeed;
                dotlist.append(sta);
                m_succeed++;
                continue;
            }

            if(dot.isDotBan(color)) continue;

            sta.dot = dot;
            sta.senior = 1;
            sta.status = dot.getDotStatusByColor(color);
            dotlist.append(sta);

            if(sta.status==SSucceed4) m_ssucceed4++;

            sta.dot = dot;
            sta.senior = 0;
            int e_status = dot.getDotStatusByColor(enemy);
            switch (e_status) {
            case SSucceed4:
                sta.status = SSucceed4;
                m_ssucceed4++;
                break;
            case SAttact4:
                sta.status = SSucceed4;
                m_ssucceed4++;
                break;
            case SAttact3:
            case Attack4:
            case Attack3:
                sta.status = Active;
                break;
            default:
                sta.status = e_status;
                break;
            }
            dotlist.append(sta);
        }
    }

    qSort(dotlist.begin(),dotlist.end(),DotController::compareDotStatus);
    QSet<DotStatus>setList = dotlist.toSet();
    dotlist = setList.toList();
    qSort(dotlist.begin(),dotlist.end(),DotController::compareDotStatus);

    return dotlist;
}

Dot DotController::getDotByPoint(const QPoint &point)
{
    return m_chessDot.map[point.x()][point.y()];
}

bool DotController::compareDotStatus(const DotStatus &sta1, const DotStatus &sta2)
{
    if(sta1.status>sta2.status)
    {
        return true;
    }
    else if(sta1.status==sta2.status)
    {
        if(sta1.senior>sta2.senior)
        {
            return true;
        }
        else if(sta1.senior==sta2.senior)
        {
            return Dot::compareDotScore(sta1.dot,sta2.dot);
        }
    }

    return false;
}

QList<DotStatus> DotController::DeleteDotByStatus(QList<DotStatus> dotlist, const int &status)
{
    QList<DotStatus>::iterator i = dotlist.begin();

    for(int i = dotlist.length()-1;i>=0;i--)
    {
        DotStatus sta = dotlist.at(i);

        if(sta.status<status) dotlist.removeAt(i);
    }
    return dotlist;
}

