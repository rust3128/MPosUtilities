#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qDebug() << "APP Version" << GIT_VERSION << BUILD_DATE;
    w.show();

    return a.exec();
}
