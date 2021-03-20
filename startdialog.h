#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();
    int getOrder()const;


private slots:
    void on_bt_sure_clicked();


private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
