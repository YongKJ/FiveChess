#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QPixmap>
#include <QDateTime>

#define SCREEN_width 1280
#define SCREEN_height 720
#define GAME_name "智能五子棋v1.2"
#define GAME_version " 2018-7-15 @ v0.1 "
#define GAME_practice 1
#define GAME_machine 0
#define GAME_network 2

#define CHESS_none 0
#define CHESS_black 1
#define CHESS_white 2
#define CHESS_illegal 3
#define BOARD_chessSize 35
#define BOARD_x 606
#define BOARD_y 143

enum{
    nomal,
    Advantage,
    Active,
    Attack3,
    Attack4,
    SAttact3,
    SAttact4,
    SSucceed4,
    Succeed,
};

class Config
{
public:
    Config();
    static QString getGameModeString(int mode = 0);
    static QPixmap getPixmapOfWhiteChess();
    static QPixmap getPixmapOfBlackChess();
    static QString getStatusIntToString(const int &status);
    static int getEnemyColor(const int &color);
    static QString getChessToString(const QPoint &chess,const int &round);
    static QString getLetterByNumber(const int &n);
    static QString getColorStringByRound(const int &round);
    static QString getWinnerToString(const int &winner);
    static QString getTimeToString();
};

#endif // CONFIG_H
