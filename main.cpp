#include "start.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("N皇后");
    QPixmap img(":/new/prefix1/src/Queen.jpg");
    w.setWindowIcon(img);
    w.show();
    return a.exec();
}
