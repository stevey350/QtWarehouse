#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); //, "connection1"
    db.setHostName("localhost");
    db.setDatabaseName("warehouse");
    db.setUserName("root");
    db.setPassword("root");
    if (!db.open()) {
        QMessageBox::critical(0, "Cannot open database1", "Unable to establish a database connection.", QMessageBox::Cancel);
        return 1;
    }
    else {
        qDebug()<< "连接数据库ok";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
