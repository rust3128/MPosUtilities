#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"

#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createModels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    qInfo(logInfo()) << "Закрыте главного окна программы.";
}
void MainWindow::createModels()
{
    QApplication::processEvents();
    qDebug(logDebug()) << "Greate Model startted" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    modelTerminals = new QSqlQueryModel(this);
    modelTerminals->setQuery("SELECT DISTINCT t.TERMINAL_ID, TRIM(t.NAME), t.OWNER_ID from terminals t "
                             "LEFT JOIN shifts s ON s.TERMINAL_ID = t.TERMINAL_ID "
                             "WHERE  t.TERMINALTYPE=3 and s.SHIFT_ID >1"
                             "ORDER BY t.TERMINAL_ID");
    modelTerminals->setHeaderData(0, Qt::Horizontal,"АЗС");
    modelTerminals->setHeaderData(1, Qt::Horizontal,"Название терминала");
    qDebug(logDebug()) << "Greate Model finished" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");;
}

void MainWindow::on_actionTerminals_triggered()
{

}
