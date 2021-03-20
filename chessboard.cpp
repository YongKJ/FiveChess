#include "chessboard.h"

ChessBoard::ChessBoard()
{
    init();
}

void ChessBoard::init()
{
    setUnWinner();

    boardMessage.board.init();
    boardMessage.qunue.clear();

    m_undoChess = QPoint(-1,-1);

}

void ChessBoard::setWinner(const int &color)
{
   game_winner = color;
   game_closeFlag = true;
}

void ChessBoard::setUnWinner()
{
    game_winner = CHESS_illegal;
    game_closeFlag = false;
}

bool ChessBoard::playChess(const QPoint &chess)
{
    if(!isPlayChessValid(chess)){
        return false;
    }

    if(isGameClose()){
        return false;
    }

    boardMessage.board.map[chess.x()][chess.y()] = getNextChessColor();
    boardMessage.qunue.append(chess);

    WinEvent(chess);

    return true;
}

bool ChessBoard::undoChess()
{
    if(getCurrentRound()==0){
        return false;
    }
    m_undoChess = boardMessage.qunue.last();
    boardMessage.board.map[m_undoChess.x()][m_undoChess.y()] = 0;
    boardMessage.qunue.pop_back();

    setUnWinner();

    return true;
}

bool ChessBoard::isPlayChessValid(const QPoint &chess)const
{
    if(!ChessBoard::isChessPointValid(chess)){
        return false;
    }

    if(boardMessage.board.map[chess.x()][chess.y()]!=0){
        return false;
    }
    return true;
}

bool ChessBoard::isGameClose()const
{
    if(getCurrentRound()==255){
        return true;
    }
    if(game_closeFlag==true){
        return true;
    }
    return false;
}

int ChessBoard::getWinner()const
{
    if(game_closeFlag==false){
        return CHESS_illegal;
    }
    return game_winner;
}

BoardMessage ChessBoard::getBoardMessage()const
{
    return boardMessage;
}

Board ChessBoard::getChessBoard() const
{
    return boardMessage.board;
}

QList<QPoint> ChessBoard::getChessQunue() const
{
    return boardMessage.qunue;
}

QPoint ChessBoard::getLastChess()const
{
    if(getCurrentRound()==0){
        return QPoint(-1,-1);
    }
    return boardMessage.qunue.last();
}

QPoint ChessBoard::getUndoChess() const
{
    return m_undoChess;
}

int ChessBoard::getCurrentRound()const
{
    return boardMessage.qunue.length();
}

int ChessBoard::getNextChessColor()const
{
    return ChessBoard::getChessColorByRound(getCurrentRound());
}

int ChessBoard::getChessColorByPoint(QPoint point) const
{
    return boardMessage.board.map[point.x()][point.y()];
}

ChessLine ChessBoard::getChessLine(const int &type,const QPoint &chess)const
{
    ChessLine line;
    line.shape[5] = getChessColorByPoint(chess)+1;
    QPoint t;
    for(int i=1;i<=5;i++){

        t = ChessBoard::FindNextQPoint(chess,type,i);
        if(ChessBoard::isChessPointValid(t)){
            line.shape[5+i] = getChessColorByPoint(t)+1;
        }
        else
        {
            line.shape[5+i] = 4;
        }
        t = ChessBoard::FindNextQPoint(chess,type,-i);
        if(ChessBoard::isChessPointValid(t)){
            line.shape[5-i] = getChessColorByPoint(t)+1;
        }
        else
        {
            line.shape[5-i] = 4;
        }
    }
    return line;
}

bool ChessBoard::isChessPointValid(const QPoint &chess)
{
    if(chess.x()<0||chess.y()<0||chess.x()>=15||chess.y()>=15){
        return false;
    }
    return true;
}

bool ChessBoard::isChessPointValid(const int &x, const int &y)
{
    if(x<0||y<0||x>=15||y>=15){
        return false;
    }
    return true;
}

int ChessBoard::getChessColorByRound(const int &round)
{
    return round%2+1;
}

QPoint ChessBoard::FindNextQPoint(QPoint chess, const int &type, const int &direct)
{
    switch (type) {
    case HLINE:
        return QPoint(chess.x()+direct,chess.y());
        break;
    case VLINE:
        return QPoint(chess.x(),chess.y()+direct);
        break;
    case LLINE:
        return QPoint(chess.x()+direct,chess.y()+direct);
        break;
    case RLINE:
        return QPoint(chess.x()-direct,chess.y()+direct);
        break;
    }
}

QList<QPoint> ChessBoard::getAreaOfChessPoint(const QPoint &point)
{
    QList<QPoint> list;
    if(!ChessBoard::isChessPointValid(point)){
        return list;
    }
    QPoint t;

    for(int i=1;i<=5;i++){
        t.setX(point.x()+i);t.setY(point.y());
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x()-i);t.setY(point.y());
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x());t.setY(point.y()+i);
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x());t.setY(point.y()-i);
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x()+i);t.setY(point.y()+i);
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x()-i);t.setY(point.y()-i);
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x()+i);t.setY(point.y()-i);
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
        t.setX(point.x()-i);t.setY(point.y()+i);
        if(ChessBoard::isChessPointValid(t)){
            list.append(t);
        }
    }
    return list;
}

bool ChessBoard::WinEvent(const QPoint &point)
{
    for(int i=0;i<4;i++){
        int t = isColorWinInLine(getChessLine(i,point));
        if(t!=CHESS_none){
            setWinner(t);
            return true;
        }
    }
    return false;
}

int ChessBoard::isColorWinInLine(const ChessLine &line)
{
    int blackCount = 0;
    int whiteCount = 0;
    for(int i=0;i<11;i++){
        if(line.shape[i]==CHESS_black+1){
            blackCount++;
            whiteCount = 0;
        }
        else if(line.shape[i]==CHESS_white+1){
            blackCount = 0;
            whiteCount++;
        }
        else{
            blackCount = 0;
            whiteCount = 0;
        }

        if(blackCount>=5){
            return CHESS_black;
        }
        if(whiteCount>=5){
            return CHESS_white;
        }
    }
    return CHESS_none;
}
