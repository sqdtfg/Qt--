#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegularExpression>

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

    connect(ui->btn_Multiple, SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
    operand = ui->display->text();
    QString btnText = qobject_cast<QPushButton*>(sender())->text();

    // 如果显示内容不是纯数字或小数，说明是运算符/错误提示等，重新开始输入
    bool isNumber = QRegularExpression("^[0-9]*\\.?[0-9]*$").match(operand).hasMatch();
    if (!isNumber) {
        operand = "";
    }

    if(operand == "0")
        operand = btnText;
    else
        operand += btnText;

    ui->display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()
{
    if(!operand.isEmpty()){
        if(operand[operand.length() - 1] == ".")
        {
            ui->display->setText("操作数无效");
            return;
        }
        operands.push_back(operand);
        operand.clear();
    }
    ui->display->setText(operand);
}

void MainWindow::on_btnPeriod_clicked()
{
    if(operand.isEmpty())
        operand = "0";
    if(!operand.contains("."))
        operand += qobject_cast<QPushButton*>(sender())->text();

    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    if (!operand.isEmpty()) {
        operand.chop(1);  // 更安全的删除最后一个字符
    }

    // 删除后若为空，可显示 0
    if (operand.isEmpty()) operand = "0";

    ui->display->setText(operand);
}


void MainWindow::on_btnClear_clicked()
{
    operand = "0";

    ui->display->setText(operand);
}

