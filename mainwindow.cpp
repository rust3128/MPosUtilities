#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include "SelectTerminalsForm/selectterminalsform.h"
#include "FuelNameDialog/fuelnamedialog.h"
#include "RunSqlDialog/runsqldialog.h"
#include "ChangeFuelNameDialog/changefuelnamedialog.h"

#include <QDateTime>
#include <QTimer>
#include <QMdiSubWindow>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);  // инициализируем QMdiArea
    // Настраиваем скроллбары
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Устанавливаем Mdi Area в качестве центрального виджета
    setCentralWidget(mdiArea);
    createModels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    qInfo(logInfo()) << "Закрыте главного окна программы.";
}
void MainWindow::createModels()
{
    QApplication::processEvents();
    qDebug(logDebug()) << "Greate Model startted" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    modelTerminals = new QSqlQueryModel(this);

    modelTerminals->setQuery("SELECT TERMINAL_ID, adress, OWNER_ID FROM ( "
                             "SELECT t.TERMINAL_ID, TRIM(t.NAME) AS adress, t.OWNER_ID from terminals t "
                             "WHERE t.TERMINALTYPE = 2 "
                             "UNION ALL "
                             "SELECT DISTINCT t.TERMINAL_ID, TRIM(t.NAME) AS adress, t.OWNER_ID from terminals t "
                             "LEFT JOIN shifts s ON s.TERMINAL_ID = t.TERMINAL_ID "
                             "WHERE t.TERMINAL_ID<30000 and t.TERMINALTYPE = 3 and s.SHIFT_ID > 0) "
                             "ORDER BY 1");

//    modelTerminals->setQuery("SELECT DISTINCT t.TERMINAL_ID, TRIM(t.NAME), t.OWNER_ID from terminals t "
//                             "LEFT JOIN shifts s ON s.TERMINAL_ID = t.TERMINAL_ID "
//                             "WHERE  t.TERMINAL_ID < 30000 and t.TERMINALTYPE=3 and s.SHIFT_ID >1"
//                             "ORDER BY t.TERMINAL_ID");
    modelTerminals->setHeaderData(0, Qt::Horizontal,"АЗС");
    modelTerminals->setHeaderData(1, Qt::Horizontal,"Название терминала");
    qDebug(logDebug()) << "Greate Model finished" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
}

void MainWindow::on_actionFuelName_triggered()
{
    FuelNameDialog *fnDiag = new FuelNameDialog(modelTerminals,this);
    fnDiag->setWindowTitle("Просмотр наименований топлива");
    if(mdiArea->subWindowList().size()>0){
        for(int i=0; i<mdiArea->subWindowList().size();++i) {
            if(mdiArea->subWindowList().at(i)->windowTitle() == fnDiag->windowTitle()) {
                mdiArea->setActiveSubWindow(mdiArea->subWindowList().at(i));
                return;
            }
        }
    }
    mdiArea->addSubWindow(fnDiag);
    mdiArea->cascadeSubWindows();
    fnDiag->show();
}

void MainWindow::on_actionRunSql_triggered()
{
    RunSqlDialog *runSql = new RunSqlDialog(modelTerminals,this);
    runSql->setWindowTitle("Выполнить скрипт на АЗС");

    if(mdiArea->subWindowList().size()>0){
        for(int i=0; i<mdiArea->subWindowList().size();++i) {
            if(mdiArea->subWindowList().at(i)->windowTitle() == runSql->windowTitle()) {
                mdiArea->setActiveSubWindow(mdiArea->subWindowList().at(i));
                return;
            }
        }
    }
    mdiArea->addSubWindow(runSql);
    mdiArea->cascadeSubWindows();
    runSql->show();
}

void MainWindow::on_actionChangeFuel_triggered()
{
    ChangeFuelNameDialog *chDiag = new ChangeFuelNameDialog(modelTerminals,this);
    if(mdiArea->subWindowList().size()>0){
        for(int i=0; i<mdiArea->subWindowList().size();++i) {
            if(mdiArea->subWindowList().at(i)->windowTitle() == chDiag->windowTitle()) {
                mdiArea->setActiveSubWindow(mdiArea->subWindowList().at(i));
                return;
            }
        }
    }
    mdiArea->addSubWindow(chDiag);
    mdiArea->cascadeSubWindows();
    chDiag->show();
}
