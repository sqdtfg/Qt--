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

    connect(ui->btnMultiple, SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnNumClicked()
{
    if (justCalculated) { // 如果刚计算完一次结果，则清空状态，开始新输入
        operand.clear();
        operands.clear();
        opcodes.clear();
        justCalculated = false;
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


QString MainWindow::calculation(bool *ok){
    if (operands.isEmpty())
        return "0";

    // 用第一个操作数作为初始值，不弹出
    double result = operands.at(0).toDouble();

    int opCount = opcodes.size();
    int operandCount = operands.size();

    // i 对应第 i 个运算符，操作数为 operands[i+1]
    for (int i = 0; i < opCount ; ++i) {
        if(i + 1 >= operandCount) break;

        QString op = opcodes.at(i);
        double operand2 = operands.at(i + 1).toDouble();

        if (op == "+") {
            result += operand2;
        } else if (op == "-") {
            result -= operand2;
        } else if (op == "×") {
            result *= operand2;
        } else if (op == "÷") {
            if (operand2 == 0) {
                if (ok) *ok = false;
                return "错误：除数为0";
            }
            result /= operand2;
        }
    }

    // 清空缓存
    operands.clear();
    opcodes.clear();

    if (ok) *ok = true;
    return QString::number(result);
}

void MainWindow::btnBinaryOperatorClicked()
{
    if (justCalculated) { // 进行连算，清除 justCalculated 状态
        justCalculated = false;
    }
    opcode = qobject_cast<QPushButton*>(sender())->text();
    if(!operand.isEmpty()){ //当前操作数不为空
        if(operand.endsWith(".")) //最后一位是小数点，取小数点前数字做操作数
        {
            operand.chop(1);
        }
        operands.push_back(operand); //有效操作数，入栈存储
        operand.clear(); //清空当前操作数
    }

    opcodes.push_back(opcode); //有效操作符，入栈存储
    ui->display->setText(opcode); // 显示当前操作符，提示用户当前状态
}

void MainWindow::on_btnEqual_clicked()
{
    if(!operand.isEmpty()){
        if(operand.endsWith(".")) //最后一位是小数点，取小数点前数字做操作数
        {
            operand.chop(1);
        }
        operands.push_back(operand); //有效操作数，入栈存储
        operand.clear(); //清空当前操作数
    }

    bool ok = true;
    QString result = calculation(&ok);

    if (!ok) {
        ui->display->setText(result); // 错误信息
    } else {
        ui->display->setText(result);
        // 将结果作为下一次计算的起点
        operands.push_back(result);
    }

    justCalculated = true; //标记为刚计算完
}

