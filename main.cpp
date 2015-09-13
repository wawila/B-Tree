#include "mainwindow.h"
#include <QApplication>
#include <bplus.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BPlus bp;
    bp.play();

//    MainWindow w;
//    w.show();


    return a.exec();
}
