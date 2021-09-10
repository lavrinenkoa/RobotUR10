#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>

#include "common.h"


int main(int argc, char *argv[])
{
    qRegisterMetaType<FloatVector>("FloatVector");

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("UR10 Cobot 3D debug window");
    w.show();

    return a.exec();
}
