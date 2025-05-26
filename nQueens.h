#ifndef NQUEENS_H
#define NQUEENS_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QColor>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QVector>
#include <QRandomGenerator>
#include <random>
#include <QElapsedTimer>


namespace Ui {
class nQueens;
}

class nQueens : public QWidget
{
    Q_OBJECT

public:
    explicit nQueens(int Board_size, QWidget *parent = nullptr);
    ~nQueens();

private slots:
    void on_init_btn_clicked();
    void placeChess(int x, int y, QColor c);

    void on_start_btn_clicked();

    void on_next_btn_clicked();

private:
    Ui::nQueens *ui;
    int Board_size;
    int x, y;
    int rdm;
    int count;
    int idx;
    bool init_btn_flag;
    bool start_btn_flag;
    bool timeout_flag;
    QColor *black_c;
    QColor *gray_c;
    QVector<int> col;
    QVector<QVector<int>> sltn;
    QVector<bool> on;
    QVector<bool> diag1;
    QVector<bool> diag2;
};

#endif // NQUEENS_H
