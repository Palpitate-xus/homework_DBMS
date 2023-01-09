#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include <QDialog>

namespace Ui {
class Dialog_login;
}

class Dialog_login : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_login(QWidget *parent = 0);
    ~Dialog_login();

private slots:
    void on_exitBtn_clicked();

    void on_loginBtn_clicked();

private:
    Ui::Dialog_login *ui;
};

#endif // DIALOG_LOGIN_H
