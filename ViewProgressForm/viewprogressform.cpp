#include "viewprogressform.h"
#include "ui_viewprogressform.h"
#include "FuelNameDialog/viewfuelnamedialog.h"



static QString passConv(QString str)
{
    if(str == "twqsnrhlv")
        return "sunoiladm";
    if(str == "juoesj8=")
        return "island15";
    if(str == "hc}xwuu9;=")
        return "gaztron123";
    if(str == "lguwmkyimw?AC")
        return "kersheradm456";
    return "masterkey";
};

static bool compare(const AzsFuelName& first, const AzsFuelName& second)
{
    if (first.terminalID() < second.terminalID())
    {
        return true;
    }
    else
    {
        return false;
    }
}

ViewProgressForm::ViewProgressForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewProgressForm)

{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "АЗС" << "Статус");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget->verticalHeader()->setDefaultSectionSize(36);
}

ViewProgressForm::~ViewProgressForm()
{
    delete ui;
}

void ViewProgressForm::slotGetTerminalsList(QList<int> *list)
{
    m_listTerm = list;
}

void ViewProgressForm::slotGetListSQL(QStringList sql)
{
    m_listSQL =sql;
}

void ViewProgressForm::slotRunSQL(int type)
{
    m_typeSQL = type;
    getConnectionsList();

    //Количетсво обрабатываемх АЗС
    int _azsCount = m_connectionsList.size();
    m_colError=0;
    ui->progressBarGetFuel->setRange(0, _azsCount);
    ui->progressBarGetFuel->setValue(0);
    ui->progressBarGetFuel->setFormat("Обработано %v из %m. Ошибок "+QString::number(m_colError));

    for(int i=0; i<_azsCount; i++){
        //Создаем объект класса получения наиметований и потока
        ExecuteSqlClass *execSQL = new ExecuteSqlClass(m_connectionsList.at(i),m_listSQL, m_typeSQL);
        QThread *thread = new QThread();
        //помещаем класс в поток.
        execSQL->moveToThread(thread);
        //Связываем сигналы и слоты
        connect(thread,&QThread::started, execSQL, &ExecuteSqlClass::executeSQL);
        connect(execSQL,&ExecuteSqlClass::signalSendStatus,this,&ViewProgressForm::slotGetStatusThread,Qt::UniqueConnection);
        connect(execSQL,&ExecuteSqlClass::signalSendAzsFuelName,this,&ViewProgressForm::slotGetAzsFuelName,Qt::DirectConnection);
        connect(execSQL,&ExecuteSqlClass::finisExecute, execSQL, &ExecuteSqlClass::deleteLater);
        connect(execSQL,&ExecuteSqlClass::finisExecute, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }
}

void ViewProgressForm::slotGetStatusThread(statusThread status)
{
    if(status.currentStatus == CONNECT_TO_DATABASE){
        //Статус соединения с базой
        //Добавляем строки в TableWidget
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0, new QTableWidgetItem(QString::number(status.terminalId)));
        ui->tableWidget->setItem(row,1, new QTableWidgetItem(status.statusText));
        ui->tableWidget->item(row,0)->setIcon(QIcon(":/Images/azs.png"));
        ui->tableWidget->item(row,1)->setIcon(QIcon(":/Images/database.png"));
        ui->tableWidget->item(row,1)->setBackground(QBrush("#F4FA58"));
        ui->tableWidget->sortByColumn(0,Qt::AscendingOrder);
        return;
    }

    int rowCount = ui->tableWidget->rowCount();
    for (int i=0;i<rowCount;++i) {
        //Находим строку в TableWidget и изменяем ее в соответсвии со статусом
        if(ui->tableWidget->item(i,0)->text().toInt() == status.terminalId){
            ui->tableWidget->item(i,1)->setText(status.statusText);
            switch (status.currentStatus) {
            case EXECUTE_SQL:
                 ui->tableWidget->item(i,1)->setBackground(QBrush("#D7DF01"));
                 ui->tableWidget->item(i,1)->setIcon(QIcon(":/Images/selectfuel.png"));
                break;
            case ERROR_OPEN_DATABASE:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#FE2E2E"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Images/error.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
                m_colError++;
               break;
            case ERROR_EXECUTE_SQL:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#FE2E2E"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Images/error.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
                m_colError++;
               break;
            case FINISHED:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#BFFF00"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Images/Accept.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
               break;
            default:
                break;
            }
            break;
        }
    }
    ui->tableWidget->resizeRowsToContents();

    ui->progressBarGetFuel->setFormat("Обработано %v из %m. Ошибок "+QString::number(m_colError));
    if(ui->progressBarGetFuel->value() == m_connectionsList.size()){
        switch (m_typeSQL) {
        case SIMPLE_SQL:
            break;
        case SHOW_FUEL_NAME:
            showFuelName();
            break;
        default:
            break;
        }
    }
}

void ViewProgressForm::slotGetAzsFuelName(AzsFuelName azsFuelname)
{
    //Добавляем полученный список наименований
    m_listFuelName.append(azsFuelname);
}

void ViewProgressForm::getConnectionsList()
{
    QSqlQuery q;
    QString strIN="";
    for(int i=0;i<m_listTerm->size();++i){
        strIN += QString::number(m_listTerm->at(i))+",";
    }
    strIN.resize(strIN.size()-1);
    QString strSQL = QString("select c.TERMINAL_ID, c.SERVER_NAME, c.DB_NAME, c.CON_PASSWORD from CONNECTIONS c "
                             "where c.TERMINAL_ID IN(%1) and c.CONNECT_ID=2").arg(strIN);
    QStringList list;
    if(!q.exec(strSQL)) qCritical(logCritical()) << "Не удалось получить список терминалов" << q.lastError().text();
    while(q.next()){
        list.clear();
        list << q.value(0).toString() << q.value(1).toString() << q.value(2).toString()  << passConv(q.value(3).toString());
        m_connectionsList.append(list);
    }
}

void ViewProgressForm::showFuelName()
{
    std::sort(m_listFuelName.begin(), m_listFuelName.end(),compare);
    ViewFuelNameDialog *viewFuelName = new ViewFuelNameDialog(&m_listFuelName,this);
    viewFuelName->exec();
}

void ViewProgressForm::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    ui->tableWidget->resizeRowsToContents();
}
