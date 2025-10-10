#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnNum_0, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_1, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_2, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_3, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_4, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_5, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_6, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_7, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_8, SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum_9, SIGNAL(clicked()),this,SLOT(btnNumClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
    QString str = ui->display->text();
    QString btnText = qobject_cast<QPushButton*>(sender())->text();

    if(str == "0")
        str = btnText;
    else
        str += btnText;

    ui->display->setText(str);
}

void MainWindow::on_btnPeriod_clicked()
{
    QString str = ui->display->text();
    if(!str.contains("."))
        str += qobject_cast<QPushButton*>(sender())->text();

    ui->display->setText(str);
}


void MainWindow::on_btnDel_clicked()
{
    QString str = ui->display->text();
    str = str.left(str.length() - 1);

    ui->display->setText(str);
}

