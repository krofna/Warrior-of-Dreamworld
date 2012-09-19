#include <QtGui/QApplication>
#include <QDebug>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QSqlDatabase::drivers();
    MainWindow w;
    w.show();
    
    return a.exec();
}
