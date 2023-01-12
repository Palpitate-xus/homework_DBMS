#include "dialog_login.h"
#include "ui_dialog_login.h"
#include "login.h"
#include <string>
#include <QMessageBox>
Dialog_login::Dialog_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_login)
{
    ui->setupUi(this);
    ui->pwdlineEdit->setEchoMode(QLineEdit::Password);
}

Dialog_login::~Dialog_login()
{
    delete ui;
}

void Dialog_login::on_exitBtn_clicked()
{

}

void Dialog_login::on_loginBtn_clicked()
{
    if(userLogin(ui->usrlineEdit->text().trimmed().toStdString(),ui->pwdlineEdit->text().trimmed().toStdString()))
    {
         accept();
    }
     else
     {
         QMessageBox::warning(this, tr("Warning"), tr("user name or password error!"),QMessageBox::Yes);
         ui->usrlineEdit->clear();
         ui->pwdlineEdit->clear();
         ui->usrlineEdit->setFocus();
      }
}
