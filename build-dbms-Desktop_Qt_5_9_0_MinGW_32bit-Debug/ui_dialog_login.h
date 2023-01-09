/********************************************************************************
** Form generated from reading UI file 'dialog_login.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_LOGIN_H
#define UI_DIALOG_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_login
{
public:
    QLabel *label_3;
    QPushButton *loginBtn;
    QPushButton *exitBtn;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *pwdlineEdit;
    QLabel *label_2;
    QLineEdit *usrlineEdit;

    void setupUi(QDialog *Dialog_login)
    {
        if (Dialog_login->objectName().isEmpty())
            Dialog_login->setObjectName(QStringLiteral("Dialog_login"));
        Dialog_login->resize(300, 150);
        label_3 = new QLabel(Dialog_login);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(100, 10, 181, 16));
        loginBtn = new QPushButton(Dialog_login);
        loginBtn->setObjectName(QStringLiteral("loginBtn"));
        loginBtn->setGeometry(QRect(120, 110, 75, 23));
        exitBtn = new QPushButton(Dialog_login);
        exitBtn->setObjectName(QStringLiteral("exitBtn"));
        exitBtn->setGeometry(QRect(210, 110, 75, 23));
        widget = new QWidget(Dialog_login);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(60, 30, 177, 66));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        pwdlineEdit = new QLineEdit(widget);
        pwdlineEdit->setObjectName(QStringLiteral("pwdlineEdit"));

        gridLayout->addWidget(pwdlineEdit, 1, 1, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        usrlineEdit = new QLineEdit(widget);
        usrlineEdit->setObjectName(QStringLiteral("usrlineEdit"));

        gridLayout->addWidget(usrlineEdit, 0, 1, 1, 1);


        retranslateUi(Dialog_login);

        QMetaObject::connectSlotsByName(Dialog_login);
    } // setupUi

    void retranslateUi(QDialog *Dialog_login)
    {
        Dialog_login->setWindowTitle(QApplication::translate("Dialog_login", "Dialog", Q_NULLPTR));
        label_3->setText(QApplication::translate("Dialog_login", "\345\237\272\344\272\216C++\347\232\204\347\256\200\345\215\225DBMS", Q_NULLPTR));
        loginBtn->setText(QApplication::translate("Dialog_login", "Login", Q_NULLPTR));
        exitBtn->setText(QApplication::translate("Dialog_login", "Exit", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog_login", "\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog_login", "\345\257\206\347\240\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_login: public Ui_Dialog_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_LOGIN_H
