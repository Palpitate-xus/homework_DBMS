#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sql_parser.h"
#include <QMessageBox>
#include <QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString message;
            message = QString::fromStdString(execute(sql_processor(ui->input_text->toPlainText().trimmed().toStdString())));
    QMessageBox::warning(this, tr("info"),
                                message,
                                QMessageBox::Yes);
    ui->textBrowser->setText(message);
}
