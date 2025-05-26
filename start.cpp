#include "start.h"
#include "ui_start.h"

int BOARD_SIZE_MAX = 20;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_clicked);
    ui->lineEdit->clear();
    ui->lineEdit->setEchoMode(QLineEdit::Normal);
    ui->lineEdit->setPlaceholderText(QString("0<N≤%1").arg(BOARD_SIZE_MAX));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int Board_size = ui->lineEdit->text().toInt();
    //输入检测
    if(Board_size > BOARD_SIZE_MAX || Board_size<=0){
        QMessageBox::information(this, "错误", "输入数字不合法，请重新输入！\n");
        ui->lineEdit->clear();
    }
    else{
        nQueens *b1 = new nQueens(Board_size);
        QPixmap img(":/new/prefix1/src/Queen.jpg");
        b1->setWindowIcon(img);
        b1->setWindowTitle("N皇后");
        b1->show();
        this->close();
    }
}

