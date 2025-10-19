#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    digitBTNs = {{Qt::Key_0, ui->btnNum_0},
               {Qt::Key_1, ui->btnNum_1},
               {Qt::Key_2, ui->btnNum_2},
               {Qt::Key_3, ui->btnNum_3},
               {Qt::Key_4, ui->btnNum_4},
               {Qt::Key_5, ui->btnNum_5},
               {Qt::Key_6, ui->btnNum_6},
               {Qt::Key_7, ui->btnNum_7},
               {Qt::Key_8, ui->btnNum_8},
               {Qt::Key_9, ui->btnNum_9},
               };
    opBTNs = {
        {Qt::Key_Plus, ui->btnPlus},
        {Qt::Key_Minus, ui->btnMinus},
        {Qt::Key_Asterisk, ui->btnMultiple},   // * 对应乘号
        {Qt::Key_Slash, ui->btnDivide},        // / 对应除号
    };

    foreach(auto btn, digitBTNs)
        connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    foreach(auto btn, opBTNs)
        connect(btn, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));


    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSign, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
    if(isUnaryOperator){
        isUnaryOperator = false;
        operand.clear();
    }

    if(isBinaryOperator){ //输入第二个操作数
        isBinaryOperator = false;
        ui->display->clear();
    }

    if (isEqual) { // 结束等号计算状态
        operand.clear();
        operands.clear();
        opcodes.clear();
        isEqual = false;
        ui->display->clear();
    }

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


void MainWindow::on_btnPeriod_clicked()
{
    if(isEqual || isBinaryOperator || isUnaryOperator){
        operand.clear();
        isEqual = false;
        isBinaryOperator = false;
        isUnaryOperator = false;
        return;
    }

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
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
    isEqual = false;
    isBinaryOperator = false;
    isUnaryOperator = false;
}

void MainWindow::on_btnClearAll_clicked()
{
    operand = "0";
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
    isEqual = false;
    isBinaryOperator = false;
    isUnaryOperator = false;
}


QString MainWindow::calculation(bool *ok)
{
    if (operands.isEmpty() && isEqual){ //操作数为空
        opcodes.clear();
        return operand;
    }

    if(operands.size() == 1){ //只有一个操作数的情况
        QString result = operands.front();
        operands.pop_front();
        return result;
    }

    double result = 0.0; //记录计算结果
    if (!opcodes.isEmpty() && operands.size() == 2) { //两个操作数
        double operand1 = operands.front().toDouble(); // 弹出一个操作数
        operands.pop_front();

        double operand2 = operands.front().toDouble(); // 弹出另一个操作数
        operands.pop_front();

        QString op = opcodes.front(); //弹出操作符
        opcodes.pop_front();

        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "×") {
            result = operand1 * operand2;
        } else if (op == "÷") {
            if (operand2 == 0) {
                if (ok) *ok = false;
                operands.clear();
                opcodes.clear();
                return "错误：除数为0";
            }
            result = operand1 / operand2;
        }
    }
    return QString::number(result);
}


void MainWindow::btnBinaryOperatorClicked()
{
    isBinaryOperator = true; //二元运算状态
    isUnaryOperator = false;
    isEqual = false;

    if(!operand.isEmpty()){ //当前操作数不为空
        if(operand.endsWith(".")) //最后一位是小数点，取小数点前数字做操作数
        {
            operand.chop(1);
        }
        operands.push_back(operand); //有效操作数，入栈存储
        operand.clear(); //清空当前操作数
    }

    if(operands.size() >= opcodes.size() && !operands.isEmpty()){ //避免连续按多个操作符
        opcode = qobject_cast<QPushButton*>(sender())->text();
        opcodes.push_back(opcode); //有效操作符，入栈存储
    }

    //计算结果
    bool ok = true;
    QString result = calculation(&ok);  //计算结果为新的操作数
    if (!ok) {
        ui->display->setText(result); // 错误信息
    } else {
        ui->display->setText(result);
        operands.push_back(result); // 将结果作为下一次计算的起点
    }
}

void MainWindow::btnUnaryOperatorClicked()
{
    isUnaryOperator = true;
    if(!operand.isEmpty()){
        if(operand.endsWith(".")) //最后一位是小数点，取小数点前数字做操作数
        {
            operand.chop(1);
        }

        double result = operand.toDouble();
        QString op = qobject_cast<QPushButton*>(sender())->text();

        if(op == "%"){
            result /= 100.0;
        }else if(op == "1/x"){
            if(result == 0){
                ui->display->setText("错误，除数不能为0");
                operand = "0";
                return;
            }
            result = 1 / result;
        }else if(op == "x²"){
            result *= result;
        }else if(op == "+/-"){
            result = -result;
        }
        else{
            result = sqrt(result);
        }
        ui->display->setText(QString::number(result));
        operand = QString::number(result);
    }
}

void MainWindow::on_btnEqual_clicked()
{
    isEqual = true; //进行等号运算
    isBinaryOperator = false;
    isUnaryOperator = false;
    if(!operand.isEmpty()){
        if(operand.endsWith(".")) //最后一位是小数点，取小数点前数字做操作数
        {
            operand.chop(1);
        }
        operands.push_back(operand); //有效操作数，入栈存储
        operand.clear(); //清空当前操作数
    }

    //计算结果
    bool ok = true;
    QString result = calculation(&ok);  //计算结果为新的操作数
    if (!ok) {
        ui->display->setText(result); // 错误信息
    } else {
        ui->display->setText(result);
        operand = result;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach(auto btnKey, digitBTNs.keys()){
        if(event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }
    foreach(auto btnKey, opBTNs.keys()){
        if(event->key() == btnKey)
            opBTNs[btnKey]->animateClick();
    }

    if (event->key() == Qt::Key_Backspace) {
        ui->btnDel->animateClick();
    }

    if (event->key() == Qt::Key_Enter ||
        event->key() == Qt::Key_Return ||
        event->key() == Qt::Key_Equal)
    {
        ui->btnEqual->animateClick();
    }

    if (event->key() == Qt::Key_Period || event->key() == Qt::Key_Comma) {
        ui->btnPeriod->animateClick();
    }

    if (event->key() == Qt::Key_Percent) {
        ui->btnPercentage->animateClick();
    }
}


