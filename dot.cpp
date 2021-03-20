#include "dot.h"



Dot::Dot()
{
    init();
}

void Dot::init()
{
    dot_mes.init();
    chess = QPoint(-1,-1);
    changlian = false;
    b_tiaosan = 0;
    b_tiaosi = 0;

    w_tiaosan = 0;
    w_tiaosi = 0;
}

bool Dot::setBoard(const ChessBoard &board,const QPoint &point)
{
    init();
    chess = point;

    if(!ChessBoard::isChessPointValid(point)){
       return false;
    }

    if(board.getChessColorByPoint(point)!=CHESS_none){
        return false;
    }


    for(int i=0;i<4;i++){
        ChessLine line = board.getChessLine(i,point);
          LineScan(line);
    }


}

DotMessage Dot::getDotMessage() const
{
    return dot_mes;
}

bool Dot::isDotNoMessage(const int &color) const
{
    if(color==CHESS_black){

        if(dot_mes.black[0]==0&&dot_mes.black[1]==0&&dot_mes.black[2]==0&&dot_mes.black[3]==0&&dot_mes.black[4]==0&&dot_mes.black[5]==0){
            return true;
        }
    }
    else{
        if(dot_mes.white[0]==0&&dot_mes.white[1]==0&&dot_mes.white[2]==0&&dot_mes.white[3]==0&&dot_mes.white[4]==0&&dot_mes.white[5]==0){
            return true;
        }
    }
    return false;
}

int Dot::getDotStatusByColor(const int &color)const
{
    const int *con = CHESS_black==color?dot_mes.black:dot_mes.white;

    if(con[0]>0)
    {
        return Succeed;
    }

    if(con[1]>0)
    {
        return SSucceed4;
    }

    if(con[2]>=2||(con[2]>0&&con[3]>0))
    {
        return SAttact4;
    }

    if(con[3]>=2)
    {
        return SAttact3;
    }

    if(con[2]>0)
    {
        return Attack4;
    }
    if(con[3]>0)
    {
        return Attack3;
    }

    if(getDotScore()>=4)
    {
        return Active;
    }

    if(getEmmironmentScore()>=1)
    {
        return Advantage;
    }

    return nomal;
}

int Dot::getDotScore() const
{
    int score = 0;
    for(int i=0;i<=5;i++){
        if(i==5)
        {
            score+=dot_mes.black[i]*2;
            score+=dot_mes.white[i]*2;
        }
        else if(i==4)
        {
            score+=dot_mes.black[i]*3;
            score+=dot_mes.white[i]*3;
        }
        else if(i==3||i==2)
        {
            score+=dot_mes.black[i]*15-b_tiaosan*4-b_tiaosi*3;
            score+=dot_mes.white[i]*15-w_tiaosan*4-w_tiaosi*3;
        }
        else if(i==1)
        {
            score+=dot_mes.black[i]*500;
            score+=dot_mes.white[i]*500;
        }
        else
        {
            score+=dot_mes.black[i]*2000;
            score+=dot_mes.white[i]*2000;
        }

    }
    return score+getEmmironmentScore();
}

int Dot::getDotScore(const int &color) const
{
    int score = 0;
    const int *con = (CHESS_black==color?dot_mes.black:dot_mes.white);
    const int tiaosan = (CHESS_black==color?b_tiaosan:w_tiaosan);
    const int tiaosi = (CHESS_black==color?b_tiaosi:w_tiaosi);
    for(int i=0;i<=5;i++){
        if(i==5)
        {
            score+=con[i]*2;
        }
        else if(i==4)
        {
             score+=con[i]*3;
        }
        else if(i==3||i==2)
        {
             score+=con[i]*15-tiaosan*4-tiaosi*3;
        }
        else if(i==1)
        {
             score+=con[i]*500;
        }
        else
        {
             score+=con[i]*2000;
        }
    }
    return score+getEmmironmentScore();
}

bool Dot::operator <(const Dot &dot) const
{
    return dot.getDotScore()<getDotScore();
}

bool Dot::operator ==(const Dot &dot) const
{
    if(dot.chess.x()==chess.x()&&dot.chess.y()==chess.y()){
            return true;
    }
    return false;
}

int Dot::getEmmironmentScore() const
{
    int x =chess.x()-7;
    int y =chess.y()-7;
    if(x>=-1&&x<=1&&y>=-1&&y<=1)
    {
        return 3;
    }

    if(x>=-3&&x<=3&&y>=-3&&y<=3)
    {
        return 2;
    }

    return 0;
}

bool Dot::isDotBan(const int &color) const
{
    if(color==CHESS_black)
    {
        //活三>=2，则禁手
        if(dot_mes.black[DOT_ACTIVE3]>=2)
        {
            return true;
        }
        //活四和冲四加起来>=2，则禁手
        if(dot_mes.black[DOT_ACTIVE4]+dot_mes.black[DOT_DEATH4]>=2)
        {
                return true;
        }
        if(changlian==true)
        {
            return true;
        }
    }
    return false;
}

bool Dot::compareDotScore(const Dot &dot1, const Dot &dot2)
{
    return dot1.getDotScore()>=dot2.getDotScore();
}

void Dot::DotScan0(const int &judge, bool &f4)
{
    switch(judge)
    {
    case 122222:
        changlian = true;
        break;
    //胜利
    case 12222:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_black);
        break;
    case 13333:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_white);
        break;
    //活四
    //死四
    case 11222: 
        b_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 12122:
        b_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 12212:        
        b_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;

    case 11333:       
        w_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 13133:       
        w_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 13313:       
        w_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    //活三
    //死三
    //活二
    }
}

void Dot::DotScan1(const int &judge, bool &f4)
{

    switch(judge)
    {
    case 212222:
        changlian = true;
        break;
    //胜利
    case 21222:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_black);
        break;
    case 31333:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_white);
        break;
    //活四
    case 112221:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_black);
        break;
    case 113331:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_white);
        break;

    //死四
    case 21122: 
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 21212:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;

    case 31133:   
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 31313: 
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;

    case 112223:
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 112224:
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 312221:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 412221:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;

    case 113332:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 113334:      
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 213331:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 413331:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    //活三
    case 111221:
        b_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 111331:
        w_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;

    case 11221:
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 11331:
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;

    case 112121:
        b_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 113131:
        w_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;
    //死三
    case 312211:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 412211:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 311221:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 411221:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 312121:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 412121:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 111223:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 111224:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 112123:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 112124:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;

    case 213311:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 413311:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 211331:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 411331:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 213131:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 413131:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 111332:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 111334:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 113132:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 113134:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;

    //活二
    case 11211:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        break;
    case 11121:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        break;
    case 11311:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        break;
    case 11131:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        break;
    }
}

void Dot::DotScan2(const int &judge, bool &f4)
{
    switch(judge)
    {
    case 221222:
        changlian = true;
        break;
    //胜利
    case 22122:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_black);
        break;
    case 33133:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_white);
        break;

    //活四
    case 121221:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_black);
        break;
    case 131331:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_white);
        break;
    //死四
   case 22112:    
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 21122:      
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;

    case 33113:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 31133:     
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;


    case 321221:      
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 421221:       
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 121223:
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;
    case 121224:      
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;

        break;

    case 231331:        
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;

        break;
    case 431331:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    case 131332:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    case 131334:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    //活三
    case 12121:
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 13131:
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;


    case 121121:
        b_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 131131:
        w_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;

    //死三
    case 321211:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 421211:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 321121:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 421121:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 121123:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 121124:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 111223:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 111224:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;

    case 231311:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 431311:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 231131:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 431131:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 131132:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 131134:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 111332:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 111334:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;


    //活二
    case 12111:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        break;
    case 11121:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        break;

    case 13111:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        break;
    case 11131:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        break;
    }
}

void Dot::DotScan3(const int &judge,bool &f4)
{
    switch(judge)
    {
    case 222122:
        changlian = true;
        break;
    //胜利
    case 22212:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_black);
        break;
    case 33313:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_white);
        break;
    //活四
    case 122121:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_black);
        break;
    case 133131:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_white);
        break;
    //死四
    case 22112:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
        break;
    case 21212:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
        break;

    case 33113:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    case 31313:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;

    case 322121:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
        break;
    case 422121:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
        break;
    case 122123:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
        break;
    case 122124:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
        break;

    case 233131:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    case 433131:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    case 133132:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    case 133134:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
        break;
    //活三
    case 12211:
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 13311:
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;

    case 121121:
        b_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
        break;
    case 131131:
        w_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
        break;
    //死三
    case 322111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 422111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 321121:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 421121:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 112123:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 112124:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 121123:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;
    case 121124:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
        break;

    case 233111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 433111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 231131:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 431131:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 131132:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 131134:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 113132:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    case 113134:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
        break;
    //活二

    case 12111:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        break;
    case 11211:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_black);
        break;
    case 13111:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        break;
    case 11311:
        AddTypeCountOfColor(DOT_ACTIVE2,CHESS_white);
        break;
    }
}

void Dot::DotScan4(const int &judge,bool &f4)
{
    switch(judge)
    {
    case 222212:
        changlian = true;
        break;
    //胜利
    case 22221:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_black);
        break;
    case 33331:
        AddTypeCountOfColor(DOT_SUCCEET,CHESS_white);
        break;
        //活四
    case 122211:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_black);
            break;
    case 133311:
        AddTypeCountOfColor(DOT_ACTIVE4,CHESS_white);
            break;
       //死四
    case 22211:

        b_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;
    case 22121:

        b_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;
    case 21221:

        b_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;

    case 33311:

        w_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;
    case 33131:

        w_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;
    case 31331:

        w_tiaosi++;
        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;

    case 322211:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;
        case 422211:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;
        case 122213:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;
        case 122214:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_black);f4=true;
            break;

        case 233311:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;
        case 433311:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;
        case 133312:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;
        case 133314:

        if(!f4)AddTypeCountOfColor(DOT_DEATH4,CHESS_white);f4=true;
            break;
        //活三
        case 122111:
        b_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
            break;
        case 121211:
        b_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_black);
            break;
        case 133111:
        w_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
            break;
        case 131311:
        w_tiaosan++;
        if(!f4)AddTypeCountOfColor(DOT_ACTIVE3,CHESS_white);
            break;
        //死三
        case 322111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 321211:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 422111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 421211:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;

        case 122113:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 122114:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 121213:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 121214:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 112213:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;
        case 112214:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_black);
            break;

        case 233111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 433111:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 231311:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 431311:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 133112:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 133114:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 113312:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 113314:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 131312:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;
        case 131314:
        AddTypeCountOfColor(DOT_DEATH3,CHESS_white);
            break;

        //活二
    }
}

void Dot::DotScan5(const int &judge, bool &f4)
{
    switch(judge)
    {
        //胜利
        //活四
        //死四
        //活三
        //死三
        //活二
    }
}

void Dot::LineScan(ChessLine line)
{
    bool f4 = false;
    for(int i=0;i+5<11;i++){

        int judge = getLineToJudge(line,i,i+5);
        switch (5-i) {
        case 5:
            DotScan5(judge,f4);
            break;
        case 4:
            DotScan4(judge,f4);
            break;
        case 3:
            DotScan3(judge,f4);
            break;
        case 2:
            DotScan2(judge,f4);
            break;
        case 1:
            DotScan1(judge,f4);
            break;
        case 0:
            DotScan0(judge,f4);
            break;
        }
        if(i>=1&&i<=5)
        {
            int judge = getLineToJudge(line,i,i+4);
            switch (5-i) {
            case 4:
                DotScan4(judge,f4);
                break;
            case 3:
                DotScan3(judge,f4);
                break;
            case 2:
                DotScan2(judge,f4);
                break;
            case 1:
                DotScan1(judge,f4);
                break;
            case 0:
                DotScan0(judge,f4);
                break;
            }
        }

    }
}

int Dot::getLineToJudge(const ChessLine &line, const int &left, const int &right)
{
    int judge = 0;
    for(int i=left;i<=right;i++){
        judge+=line.shape[i]*getNumber(right-i);
    }
    return judge;
}

int Dot::getNumber(const int &number)
{
    switch (number) {
    case 6:
        return 1000000;
    case 5:
        return 100000;
        break;
    case 4:
        return 10000;
    case 3:
        return 1000;
    case 2:
        return 100;
    case 1:
        return 10;
    case 0:
        return 1;
    }
    return 0;
}

void Dot::AddTypeCountOfColor(const int &type, const int &color)
{
    if(color==CHESS_black){
        dot_mes.black[type]++;
    }
    else{
        dot_mes.white[type]++;
    }
}
