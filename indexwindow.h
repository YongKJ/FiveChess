#ifndef INDEXWINDOW_H
#define INDEXWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include "greatbutton.h"

#include "config.h"

namespace Ui {
class IndexWindow;
}

class IndexWindow : public QWidget
{
    Q_OBJECT

public:
    explicit IndexWindow(QWidget *parent = 0);
    ~IndexWindow();

    void initWindow();

    QLabel *lb_windowTitle;

    QPushButton *bt_setting;
    QPushButton *bt_about;
    QPushButton *bt_voice;
    QPushButton *bt_practice;
    QPushButton *bt_machine;
    QPushButton *bt_network;
    QLabel *lb_choose;
    QLabel *lb_version;



protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void setChooseMode(int mode = 0);
    void Button_Practice_Slot();
    void Button_Network_Slot();
    void Button_Machine_SLot();

private:
    Ui::IndexWindow *ui;

    bool drag_flag;
    QPoint drag_flagPoint;

    void initData();
    void initLayout();
    void initConnect();


};

#endif // INDEXWINDOW_H
