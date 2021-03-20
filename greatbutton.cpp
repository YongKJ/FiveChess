#include "greatbutton.h"

GreatButton::GreatButton(QWidget *parent):
    QPushButton(parent)
{

}

void GreatButton::enterEvent(QEvent *event)
{
  emit enter();
    event->accept();
}
