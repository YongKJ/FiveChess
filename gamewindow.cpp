#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, int mode) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(GAME_name+QString(" ")+Config::getGameModeString(mode));
    this->setFixedSize(SCREEN_width,SCREEN_height);
    initWindow(mode);
    game_timer->start(1000);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::initWindow(int mode)
{
    game_mode = mode;
    initData();
    initLayout(mode);
    initConnect(mode);
    m_boardControl.init();
}

void GameWindow::setControlColor(int color)
{
    if(color==1){
        lb_ControlColor->setPixmap(Config::getPixmapOfBlackChess());
        lb_OtherColor->setPixmap(Config::getPixmapOfWhiteChess());
    }
    else{
        lb_ControlColor->setPixmap(Config::getPixmapOfWhiteChess());
        lb_OtherColor->setPixmap(Config::getPixmapOfBlackChess());
    }

}

void GameWindow::setNowPlayerColor(int color)
{
    if(color==1){
        lb_nowRoundColor->setPixmap(Config::getPixmapOfBlackChess());
    }
    else{
        lb_nowRoundColor->setPixmap(Config::getPixmapOfWhiteChess());
    }
}

bool GameWindow::GamePlayChess(const QPoint &chess)
{
    //返回将要下的点的检测信息
    Dot check = ai.dotControl.getDotByPoint(chess);
    //禁手标志
    bool ban = false;
    int nextPlayColor = m_boardControl.getNextChessColor();
    //检测下棋方将要下棋的点是不是它的禁手
    if(check.isDotBan(nextPlayColor))
    {
        //如果改点是禁手，设置标志为true
        ban = true;
    }
    if(!m_boardControl.playChess(chess))
    {
        return false;
    }
    updateWindow();
    ai.ComputerupdatePlayChess(m_boardControl,chess);
    //如果触犯了禁手点且触犯方是玩家（即对手)
    if(ban==true&&game_playerControlColor==nextPlayColor)
    {
        //设置胜利玩家为我方机器人
        m_boardControl.setWinner(Config::getEnemyColor(game_playerControlColor));
        //保存棋谱
        SaveGameOverChessBoard(m_boardControl);
        game_overTipFlag = true;
    }
    else if(m_boardControl.isGameClose()){
        SaveGameOverChessBoard(m_boardControl);
        game_overTipFlag = true;
    }
    game_currentPointOfChess = chess;
    //更新绘图窗口
    this->update();
    return true;
}

bool GameWindow::SaveGameOverChessBoard(const ChessBoard &board)
{
    ;
    QString dir = "D://fivechess//save"+QTime::currentTime().toString("hhmmss")+".txt";
    QFile save(dir);

    save.resize(0);
    if(!save.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QTextStream txtOutPut(&save);

    txtOutPut.setCodec("GB2312");

    txtOutPut<<"{[C5]";

    //先手
    txtOutPut<<QString("[先手队伍名字]");
    //后手
    txtOutPut<<QString("[后手队伍名字]");


    txtOutPut<<Config::getWinnerToString(board.getWinner());
    txtOutPut<<"["+Config::getTimeToString()+"]"<<"[2018 CCGC];";



    QList<QPoint>list = board.getChessQunue();
    int i = 0;
    foreach (QPoint chess,list) {

        txtOutPut<<Config::getChessToString(chess,i++);
        if(i!=list.length()) txtOutPut<<";";
    }

    txtOutPut<<"}";

    save.close();
    qDebug()<<"棋谱已保存:"<<dir;
    return true;

}

void GameWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint click = event->pos();
    QPoint t = getChessPointByMouseClick(click);
    game_currentPointOfChess = t;

    if(ChessBoard::isChessPointValid(t)&&control_flag){
        Dot dot;
        dot.setBoard(m_boardControl,t);
        control_monitor.setDot(dot);
    }


    if(event->button()==Qt::LeftButton){
        if(click.x()<589||click.y()<125||click.x()>=1121||click.y()>=657){
            qDebug()<<"不在棋盘区域"<<click;

            drag_flag = true;
            drag_flagPoint = event->globalPos();
        }
        else{

            if(!game_isStarting&&m_boardControl.getCurrentRound()<=0)
            {
                qDebug()<<"游戏尚未开始";
                return;
            }
            if(!game_isStarting&&m_boardControl.getCurrentRound()==4&&game_playerControlColor==CHESS_white)
            {
                if(ai.pointlist.indexOf(t)!=-1)
                {
                    if(GamePlayChess(t))
                    {
                        game_isStarting = true;
                    }

                }
                else
                {
                    qDebug()<<"五手n打:请选择其中一个绿色棋点！";
                    return;
                }

            }
            if(m_boardControl.getNextChessColor()!=game_playerControlColor&&m_boardControl.getCurrentRound()>=4){
                emit TimeToComputerPlayChess();
                qDebug()<<"轮到电脑下棋";
                return;
            }

            if(GamePlayChess(t))
            {
                if(m_boardControl.getCurrentRound()>=4)
                {
                    emit TimeToComputerPlayChess();
                }
            }
        }
    }

    this->update();
}

void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(drag_flag==true){
        QPoint temp = event->globalPos() - drag_flagPoint;
        this->move(this->pos()+temp);
        drag_flagPoint = event->globalPos();
    }
}

void GameWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        drag_flag = false;
    }
}

void GameWindow::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.drawPixmap(0,0,SCREEN_width,SCREEN_height,QPixmap(":/img/game_background"));
    painter.end();

    DrawChess();
    if(game_overTipFlag==true)
    DrawGameOverTip(m_boardControl.getWinner()==game_playerControlColor?1:0);

    if(ChessBoard::isChessPointValid(game_currentPointOfChess)){
        DrawChoosePoint(GameWindow::getDrawChessPoint(game_currentPointOfChess));
    }
    display_alreadySearch = false;
    if(!display_alreadySearch){
        DrawAlreadySearchDot(ai.dotControl);
    }

    if(control_flag)DrawRequestDisplayDot(ai.dotControl);

//    DrawSetPoint(ai.dotControl);

    if(m_boardControl.getCurrentRound()==4&&game_playerControlColor==CHESS_white) DrawFiveRoundNPlayChess(ai);

}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_P){
        control_flag = !control_flag;
        if(control_flag==true&&control_monitor.isHidden()){
            control_monitor.show();
        }
        else if(control_flag==false&&!control_monitor.isHidden()){
            control_monitor.hide();
        }
    }
}

void GameWindow::initData()
{
    drag_flag = false;
    drag_flagPoint = QPoint(0,0);

    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            boardMap[i][j] = QPoint(BOARD_x+i*BOARD_chessSize-22+i/2,BOARD_y+j*BOARD_chessSize-22+j/2);
        }
    }
    game_overTipFlag = false;
    control_flag = false;
    display_alreadySearch = false;

    game_currentPointOfChess = QPoint(-1,-1);
    ai.init();
    game_playerControlColor = CHESS_black;
    game_isStarting = false;
    game_stopflag = false;
}

void GameWindow::initLayout(int mode)
{
    game_timer = new QTimer(this);
    bt_setting = new GreatButton(this);
    bt_tip = new GreatButton(this);
    bt_undo = new GreatButton(this);
    bt_voice = new GreatButton(this);
    bt_start = new GreatButton(this);
    bt_threeSwap = new GreatButton(this);
    bt_nextRound = new GreatButton(this);

    bt_start->setText("开始");
    bt_start->setGeometry(449,280,80,46);

    bt_undo->setText("悔棋");
    bt_undo->setGeometry(449,357,80,46);

    bt_tip->setText("提示");
    bt_tip->setGeometry(449,429,80,46);

    bt_threeSwap->setText("三手交换");
    bt_threeSwap->setGeometry(200,200,80,46);

    bt_nextRound->setText("继续");
    bt_nextRound->setGeometry(449,200,80,46);
    bt_nextRound->setVisible(false);


    bt_setting->setText("设置");
    bt_setting->setGeometry(349,663,80,46);

    bt_voice->setText("声音");
    bt_voice->setGeometry(449,663,80,46);

    lb_ControlColor = new QLabel(this);
    lb_nowRoundColor = new QLabel(this);
    lb_OtherColor = new QLabel(this);
    lb_roundNumber = new QLabel(this);

    lb_ControlColor->setScaledContents(true);
    lb_nowRoundColor->setScaledContents(true);
    lb_OtherColor->setScaledContents(true);

    setControlColor(1);
    setNowPlayerColor(1);

    lb_ControlColor->setGeometry(-100,417,420,420);
    lb_OtherColor->setGeometry(1144,-105,256,273);
    lb_nowRoundColor->setGeometry(832,53,50,54);
    lb_roundNumber->setGeometry(890,53,100,54);
    lb_roundNumber->setText("第0回合");
}

void GameWindow::initConnect(int mode)
{
    connect(bt_start,SIGNAL(clicked(bool)),this,SLOT(ReStart()));
    connect(bt_undo,SIGNAL(clicked(bool)),this,SLOT(undoChess()));
    connect(this,SIGNAL(TimeToComputerPlayChess()),this,SLOT(ComputerPlayChess()));
    connect(game_timer,SIGNAL(timeout()),this,SLOT(GameTimerSlot()));
    connect(bt_threeSwap,SIGNAL(clicked(bool)),this,SLOT(ThreeSwap()));
    connect(bt_nextRound,SIGNAL(clicked(bool)),this,SLOT(GameGoOn()));
}

void GameWindow::DrawChess()
{
    QPoint point;

    QPainter painter(this);
    QList<QPoint> tempQunue = m_boardControl.getChessQunue();

    foreach (QPoint chess,tempQunue) {
        QPoint dc = GameWindow::getDrawChessPoint(chess);
        if(m_boardControl.getChessColorByPoint(chess)==1){
            painter.drawPixmap(dc.x(),dc.y(),44,44,Config::getPixmapOfBlackChess());
        }
        else{
            painter.drawPixmap(dc.x(),dc.y(),44,44,Config::getPixmapOfWhiteChess());
        }
    }

    painter.end();
}

void GameWindow::DrawGameOverTip(const int &type)
{
     QPainter painter(this);
     switch (type) {
     case 0:
         painter.drawPixmap(0,250,1280,220,QPixmap(":/img/gameover_fail"));
         break;
     case 1:
         painter.drawPixmap(0,250,1280,220,QPixmap(":/img/gameover_succeed"));
         break;
     default:
         break;
     }
     painter.end();
}

void GameWindow::DrawChoosePoint(const QPoint &point)
{
    QPainter painter(this);

    QPoint c = point+QPoint(22,22);

    painter.setPen(QPen(Qt::red,5));

    painter.drawLine(QPoint(c.x()-18,c.y()-18),QPoint(c.x()-9,c.y()-18));
    painter.drawLine(QPoint(c.x()-18,c.y()-18),QPoint(c.x()-18,c.y()-9));

    painter.drawLine(QPoint(c.x()+18,c.y()-18),QPoint(c.x()+9,c.y()-18));
    painter.drawLine(QPoint(c.x()+18,c.y()-18),QPoint(c.x()+18,c.y()-9));

    painter.drawLine(QPoint(c.x()-18,c.y()+18),QPoint(c.x()-9,c.y()+18));
    painter.drawLine(QPoint(c.x()-18,c.y()+18),QPoint(c.x()-18,c.y()+9));

    painter.drawLine(QPoint(c.x()+18,c.y()+18),QPoint(c.x()+9,c.y()+18));
    painter.drawLine(QPoint(c.x()+18,c.y()+18),QPoint(c.x()+18,c.y()+9));


    painter.end();
}

void GameWindow::DrawAlreadySearchDot(DotController controller)
{
    QPainter painter(this);

    QPoint c ;

    painter.setBrush(QBrush(Qt::red));

    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){

            if(controller.m_board.map[i][j]==1){
                if(controller.m_chessDot.map[i][j].isDotBan(CHESS_black))
                {
                    c = getDrawChessPoint(QPoint(i,j))+QPoint(11,11);
                     painter.drawEllipse(c.x(),c.y(),22,22);
                }
            }
//            if(controller.m_board.map[i][j]==1)
//            {
//                c = getDrawChessPoint(QPoint(i,j))+QPoint(11,11);
//                painter.drawEllipse(c.x(),c.y(),22,22);
//            }
        }
    }



    painter.end();
}

void GameWindow::DrawRequestDisplayDot(DotController controller)
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::blue));
    QPoint c;
    bool t = false;
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            if(!controller.isAlreadySearch(QPoint(i,j))) continue;


            t = false;

            for(int k=0;k<6;k++){

                if(control_monitor.display_type[k]&&controller.isDotHas(QPoint(i,j),control_monitor.m_controlChess,k)){
                    t = true;
                    break;
                }

            }

            if(t==true){

                c = getDrawChessPoint(QPoint(i,j))+QPoint(11,11);
                 painter.drawEllipse(c.x(),c.y(),22,22);
            }


        }


    }
    painter.end();
}

void GameWindow::DrawSetPoint(DotController controller)
{
}

void GameWindow::DrawFiveRoundNPlayChess(const Computer &cm)
{
    QPainter painter(this);
    QPoint c;
    painter.setBrush(QBrush(Qt::green));

    foreach (QPoint p, cm.pointlist) {
        c = getDrawChessPoint(p)+QPoint(11,11);
        painter.drawEllipse(c.x(),c.y(),22,22);
    }
    painter.end();
}

QPoint GameWindow::getDrawChessPoint(QPoint chess)
{
    if(!ChessBoard::isChessPointValid(chess)){
        return QPoint(0,0);

    }
    return boardMap[chess.x()][chess.y()];
}

QPoint GameWindow::getChessPointByMouseClick(QPoint click)
{
    for(int x = 0;x<15;x++){
        for(int y=0;y<15;y++){
            QPoint p = boardMap[x][y];

            if(p.x()+7<click.x()&&p.y()+7<click.y()&&p.x()+37>click.x()&&p.y()+37>click.y()){
                return QPoint(x,y);
            }
        }
    }

    return QPoint(-1,-1);
}

void GameWindow::undoChess()
{
    if(m_boardControl.undoChess()){
//        qDebug()<<"悔棋成功";
        game_currentPointOfChess = m_boardControl.getLastChess();
        ai.ComputerUndoUpdatePlayChess(m_boardControl,m_boardControl.getUndoChess());
        updateWindow();
        this->update();

    }
    else{
//        qDebug()<<"悔棋失败";
    }
}

void GameWindow::updateWindow()
{
    setNowPlayerColor(m_boardControl.getNextChessColor());
    if(control_flag){
        control_monitor.setChessBoard(m_boardControl);

    }
    game_overTipFlag = false;
    lb_roundNumber->setText("第 "+QString::number(m_boardControl.getCurrentRound()+1)+" 回合");
    if(m_boardControl.getCurrentRound()==3||(m_boardControl.getCurrentRound()==4&&m_boardControl.getNextChessColor()!=game_playerControlColor))
    {
        bt_nextRound->setVisible(true);
        game_isStarting = false;
        qDebug()<<"设置了暂停";
    }
    else
    {
        bt_nextRound->setVisible(false);
    }
}

void GameWindow::ComputerPlayChess()
{
    if(m_boardControl.isGameClose()){//游戏已经结束

        return ;
    }
    if(m_boardControl.getCurrentRound()<=2)
    {
        ai.playChess(m_boardControl,m_boardControl.getNextChessColor());

        QList<QPoint>list = ai.pointlist;

        foreach (QPoint p, list) {
            if(GamePlayChess(p))
            {
                ai.ComputerupdatePlayChess(m_boardControl,p);
            }
            else
            {
                qDebug()<<"下棋失败";
            }
        }

    }
    //五手n打
    else if(m_boardControl.getCurrentRound()==4)
    {
        ai.playChess(m_boardControl,m_boardControl.getNextChessColor());
        updateWindow();
        this->update();
    }
    else
    {
        QPoint point = ai.playChess(m_boardControl,m_boardControl.getNextChessColor());
        if(GamePlayChess(point)){
            ai.ComputerupdatePlayChess(m_boardControl,point);
        }
        else{
            qDebug()<<"机器人下棋失败";
        }
    }
    return ;
}

void GameWindow::GameTimerSlot()
{
    //    emit ComputerPlayChess();
}

void GameWindow::ReStart()
{
    StartDialog *sdialog = new StartDialog(this);

    sdialog->exec();

    int choose = sdialog->getOrder();

    m_boardControl.init();
    ai.init();
    updateWindow();
    this->update();
    setControlColor(choose);
    if(choose==2)
    {
        game_playerControlColor = CHESS_white;
        emit ComputerPlayChess();

    }
    else
    {
        game_playerControlColor = CHESS_black;
        game_isStarting = true;
    }


}

void GameWindow::ThreeSwap()
{
   if(m_boardControl.isGameClose())return;
   if(m_boardControl.getCurrentRound()!=3) return;
   game_playerControlColor = Config::getEnemyColor(game_playerControlColor);
   setControlColor(game_playerControlColor);
   bt_nextRound->setVisible(false);
   game_isStarting = true;
}

void GameWindow::GameGoOn()
{

    if(m_boardControl.getCurrentRound()==3)
    {
        game_isStarting = true;
        if(m_boardControl.getNextChessColor()!=game_playerControlColor)
        {
            emit ComputerPlayChess();
        }
        bt_nextRound->setVisible(false);

    }
    if(m_boardControl.getCurrentRound()==4&&CHESS_white==game_playerControlColor)
    {
        QMessageBox::information(this,"五手n打提示","点击棋盘上的绿色点选择相应的黑色棋子");
    }
}
