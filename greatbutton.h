#ifndef GREATBUTTON_H
#define GREATBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QEvent>

class GreatButton : public QPushButton
{
    Q_OBJECT
public:
    GreatButton(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *event);

signals:
    void enter();

};

#endif // GREATBUTTON_H
