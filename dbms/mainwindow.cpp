#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DateType.h"
#include "sql_parser.h"
#include "TableManage.h"
#include "logs.h"
#include <QMessageBox>
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
    QMessageBox::warning(this, tr("gmy"),
                                ui->input_text->toPlainText().trimmed(),
                                QMessageBox::Yes);
    ui->textBrowser->setText(ui->input_text->toPlainText().trimmed());
}
