#include "indexwindow.h"
#include "ui_indexwindow.h"

IndexWindow::IndexWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(GAME_name);
    this->setFixedSize(SCREEN_width,SCREEN_height);

    initWindow();
}

IndexWindow::~IndexWindow()
{
    delete ui;
}

void IndexWindow::initWindow()
{
    initData();
    initLayout();
    initConnect();
}

void IndexWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        drag_flag = true;
        drag_flagPoint = event->globalPos();
    }
}

void IndexWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(drag_flag==true){
        QPoint temp = event->globalPos() - drag_flagPoint;
        this->move(this->pos()+temp);
        drag_flagPoint = event->globalPos();
    }
}

void IndexWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        drag_flag = false;
    }
}

void IndexWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,SCREEN_width,SCREEN_height,QPixmap(":/img/index_background.png"));

    painter.end();

}

void IndexWindow::setChooseMode(int mode)
{
    switch(mode){
    case 0:
        lb_choose->setGeometry(950,250,40,40);
        break;
    case 1:
        lb_choose->setGeometry(950,330,40,40);
        break;
    case 2:
        lb_choose->setGeometry(950,410,40,40);
        break;
    }
}

void IndexWindow::Button_Practice_Slot()
{
    setChooseMode(1);
}

void IndexWindow::Button_Network_Slot()
{
    setChooseMode(2);
}

void IndexWindow::Button_Machine_SLot()
{
    setChooseMode(0);
}

void IndexWindow::initData()
{
    drag_flag = false;
    drag_flagPoint = QPoint(0,0);
}

void IndexWindow::initLayout()
{
    lb_windowTitle = new QLabel(this);
    lb_windowTitle->setPixmap(QPixmap(":/img/fivechess.png"));
    lb_windowTitle->setGeometry(800,100,304,93);

    bt_about = new QPushButton(this);
    bt_machine = new GreatButton(this);
    bt_network = new GreatButton(this);
    bt_practice = new GreatButton(this);
    bt_setting = new QPushButton(this);
    bt_voice = new QPushButton(this);

    bt_setting->setText("设置");
    bt_setting->setGeometry(30,656,100,45);

    bt_voice->setText("声音");
    bt_voice->setGeometry(160,656,100,45);

    bt_about->setText("关于");
    bt_about->setGeometry(290,656,100,45);

    bt_machine->setText("人机模式");
    bt_machine->setGeometry(1000,250,120,40);

    bt_practice->setText("双人模式");
    bt_practice->setGeometry(1000,330,120,40);

    bt_network->setText("联网模式");
    bt_network->setGeometry(1000,410,120,40);

    lb_choose = new QLabel(this);
    lb_choose->setScaledContents(true);
    lb_choose->setPixmap(QPixmap(":/img/white.png"));
    lb_choose->setGeometry(950,330,40,40);

    lb_version = new QLabel(this);
    lb_version->setText(GAME_version);
    lb_version->setGeometry(1150,680,100,30);
}

void IndexWindow::initConnect()
{
    connect(bt_machine,SIGNAL(enter()),this,SLOT(Button_Machine_SLot()));
    connect(bt_practice,SIGNAL(enter()),this,SLOT(Button_Practice_Slot()));
    connect(bt_network,SIGNAL(enter()),this,SLOT(Button_Network_Slot()));

}
