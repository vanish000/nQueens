#include "nQueens.h"
#include "ui_nQueens.h"


nQueens::nQueens(int Board_size, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::nQueens)
    ,col(Board_size)
    ,on(Board_size,false)
    ,diag1(2*Board_size-1,false)
    ,diag2(2*Board_size-1,false)
{
    ui->setupUi(this);
    this->Board_size = Board_size;

    //变量初始化
    idx = 0;
    init_btn_flag = false;
    start_btn_flag = false;
    black_c = new QColor(0, 0, 0);
    gray_c = new QColor(220, 220, 220);


    //初始化棋盘
    ui->tableWidget->setColumnCount(Board_size);
    ui->tableWidget->setRowCount(Board_size);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格自动拉伸填满空间
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(50);//默认宽度为50像素
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止用户直接编辑表格内容
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);//禁止用户选中任意单元格

    //初始化布局
    ui->next_btn->setAutoRepeat(true);//设置重复触发模式，无需反复点击
    ui->next_btn->setAutoRepeatDelay(400);//设置开始重复之前的延迟时间为400ms
    ui->next_btn->setAutoRepeatInterval(200);//设置重复之间的间隔时间为200ms
    ui->status_label->setText("等待初始化……\n");


}

nQueens::~nQueens()
{
    delete ui;
}

//填入棋盘
void nQueens::placeChess(int input_x, int input_y, QColor c)
{
    QTableWidgetItem* chess = new QTableWidgetItem();
    chess->setBackground(c);
    ui->tableWidget->setItem(input_x, input_y, chess);
}

//初始化
void nQueens::on_init_btn_clicked()
{
    init_btn_flag = true;
    start_btn_flag = false;//对每个残局需要重置求解标志
    timeout_flag = false;

    QElapsedTimer timer;
    timer.start();

    ui->solution_label->setText("解的数量：\n");


    int n = Board_size;
    //残局中皇后数量
    if(n==1){
        rdm = 0;
    }
    else{
        rdm = QRandomGenerator::global()->bounded(1, n);
    }


    //生成残局
    ui->tableWidget->clearContents();

    //记录皇后位置，下标表示行号，值表示列号
    on.fill(true);//判断对应列是否可用，可用时为true
    diag1.fill(true);//判断左上斜线是否已有皇后，可用时为true
    diag2.fill(true);//判断右上斜线是否已有皇后，可用时为true


    auto dfs = [&](int len, auto&& dfs_ref)->bool{
        if(len == rdm){
            return true;
        }

        //生成一个元素为1到n的随机数组
        QVector<int> choices(n);
        std::iota(choices.begin(), choices.end(), 0);//递增地填入元素
        std::random_device rd;//创建一个random_device对象，用于生成随机的种子值
        std::mt19937 g(rd());//mt19937为伪随机数生成器，使用rd生成随机数引擎g
        std::shuffle(choices.begin(), choices.end(), g);//随机打乱排序

        //遍历该随机数组，实现残局生成的随机性
        for(int i : choices){
            if(on[i] && diag1[i-len+n-1] && diag2[i+len]){
                col[len] = i;
                on[i] = false;
                diag1[i-len+n-1] = false;
                diag2[i+len] = false;
                if(dfs_ref(len+1, dfs_ref)){
                    return true;
                }
                on[i] = true;
                diag1[i-len+n-1] = true;
                diag2[i+len] = true;
            }
        }
        return false;
    };

    dfs(0, dfs);

    for(int i = 0; i < rdm; i++){
        nQueens::placeChess(i, col[i], *black_c);
    }

    ui->status_label->setText("残局已生成\n");

    qint64 dur_ns = timer.nsecsElapsed();
    double dur_ms = dur_ns/1000000.0;
    QString output = QString("残局已生成\n耗时：%1ms").arg(dur_ms, 0, 'f', 3);
    QMessageBox::information(this,"初始化成功",output);
}

//求解并保存
void nQueens::on_start_btn_clicked()
{
    if(!init_btn_flag){
        QMessageBox::warning(this, "错误", "请先进行初始化！");
        return;
    }

    if(timeout_flag){
        QMessageBox::warning(this, "错误", "该残局已求解超时！");
        return;
    }

    start_btn_flag = true;
    QElapsedTimer timer;
    timer.start();

    struct TimeoutException{};//定义异常
    const int MAX_TIMEOUT_MS = 5000;


    int n = Board_size;
    count = 0;
    sltn.clear();
    auto dfs = [&](int len, auto&& dfs_ref)->void{
        if(timer.elapsed() > MAX_TIMEOUT_MS){//超时判断
            throw TimeoutException();
        }

        if(len == n){
            sltn.emplace_back(col);
            count++;
            return;
        }

        for(int i = 0; i < n; i++){
            if(on[i] && diag1[i-len+n-1] && diag2[i+len]){
                col[len] = i;
                on[i] = false;
                diag1[i-len+n-1] = false;
                diag2[i+len] = false;
                dfs_ref(len+1, dfs_ref);
                on[i] = true;
                diag1[i-len+n-1] = true;
                diag2[i+len] = true;
            }
        }
    };
    try{
        dfs(rdm, dfs);
    }catch(const TimeoutException&){
        timeout_flag = true;
    }

    if (timeout_flag) {
        ui->status_label->setText("超时终止！\n");
        QMessageBox::warning(this, "超时", "求解超时，已自动终止！");
        ui->solution_label->setText("解的数量：\n"+QString(">%1").arg(count));
        return;
    }

    ui->solution_label->setText("解的数量：\n"+QString("%1").arg(count));
    if (count==0) {
        qint64 dur_ns = timer.nsecsElapsed();
        double dur_ms = dur_ns/1000000.0;
        QString output = QString("共找到%1组解\n耗时：%2ms").arg(count).arg(dur_ms, 0, 'f', 3);
        ui->status_label->setText("未找到解！\n");
        QMessageBox::warning(this, "错误", output);
        return;
    }


    for(int i = rdm; i < n; i++){
        nQueens::placeChess(i, sltn[0][i], *gray_c);
    }
    idx = 1;

    qint64 dur_ns = timer.nsecsElapsed();
    double dur_ms = dur_ns/1000000.0;
    QString output = QString("共找到%1组解\n耗时：%2ms").arg(count).arg(dur_ms, 0, 'f', 3);
    ui->status_label->setText("求解完成！\n当前为第1组解");
    QMessageBox::information(this,"求解完成",output);
}


void nQueens::on_next_btn_clicked()
{
    if(!init_btn_flag){
        QMessageBox::warning(this, "错误", "请先进行初始化！");
        return;
    }

    if(!start_btn_flag){
        QMessageBox::warning(this, "错误", "请先进行求解！");
        return;
    }

    if(count == 0){
        QMessageBox::warning(this, "错误", "该残局无解！");
        return;
    }

    if(timeout_flag){
        QMessageBox::warning(this, "错误", "该残局求解超时！");
        return;
    }

    for(int i = rdm; i < Board_size; i++){
        ui->tableWidget->setItem(i, sltn[(idx-1)%count][i], nullptr);
        nQueens::placeChess(i, sltn[idx%count][i], *gray_c);
    }
    idx++;

    if(idx%count == 0){
        ui->status_label->setText(QString("当前为第%1组解！\n已遍历所有解!").arg(count));
        QMessageBox::warning(this, "提示", "已遍历所有解！");
        return;
    }

    ui->status_label->setText(QString("当前为第%1组解！\n").arg(idx%count));
}

