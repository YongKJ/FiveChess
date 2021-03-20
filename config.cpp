#include "config.h"


Config::Config()
{

}

QString Config::getGameModeString(int mode)
{
    switch(mode){
    case 0:
        return "人机模式";
    case 1:
        return "练习模式";
    case 2:
        return "联网模式";
    default:
        return "未知模式";
    }
}

QPixmap Config::getPixmapOfWhiteChess()
{
    return QPixmap(":/img/white");
}

QPixmap Config::getPixmapOfBlackChess()
{
    return QPixmap(":/img/black");
}

QString Config::getStatusIntToString(const int &status)
{
    switch (status) {
    case nomal:
        return "平常";
    case Advantage:
        return "优势";
    case Active:
        return "活跃";
    case Attack3:
        return "攻击3";
    case Attack4:
        return "攻击4";
    case SAttact3:
        return "攻击33";
    case SAttact4:
        return "攻击44";
    case SSucceed4:
        return "胜利4";
    case Succeed:
        return "胜利";
    }
    return "非法";
}

int Config::getEnemyColor(const int &color)
{
    if(color==CHESS_black) return CHESS_white;
    return CHESS_black;
}

QString Config::getChessToString(const QPoint &chess, const int &round)
{
    return getColorStringByRound(round)+"("+getLetterByNumber(chess.x()+1)+","+QString::number(15-chess.y())+")";
}



QString Config::getLetterByNumber(const int &n)
{
    switch (n) {
    case 1:
        return "A";
    case 2:
        return "B";
    case 3:
        return "C";
    case 4:
        return "D";
    case 5:
        return "E";
    case 6:
        return "F";
    case 7:
        return "G";
    case 8:
        return "H";
    case 9:
        return "I";
    case 10:
        return "J";
    case 11:
        return "K";
    case 12:
        return "L";
    case 13:
        return "M";
    case 14:
        return "N";
    case 15:
        return "O";
    }
    return "*";
}

QString Config::getColorStringByRound(const int &round)
{
    if(round%2==0)
    {
        return "B";
    }
    return "W";
}

QString Config::getWinnerToString(const int &winner)
{
    if(winner==CHESS_black)
    {
        return "[先手胜]";
    }
    if(winner==0)
    {
        return "[平局]";
    }
    return "[后手胜]";
}

QString Config::getTimeToString()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")+" 合肥";
}


