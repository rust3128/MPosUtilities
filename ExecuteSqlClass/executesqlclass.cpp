#include "executesqlclass.h"
#include "LoggingCategories/loggingcategories.h"
#include <QSqlQuery>
#include <QSqlError>

ExecuteSqlClass::ExecuteSqlClass(QStringList connList, QStringList sqlList, int typeSQL, QObject *parent) :
    QObject(parent),
    m_connList(connList),
    m_listSQL(sqlList),
    m_sqlType(typeSQL)

{
    typedef statusThread st;
    qRegisterMetaType<st>("st");
}

ExecuteSqlClass::~ExecuteSqlClass()
{
    QSqlDatabase::removeDatabase(m_connList[0]);
}

void ExecuteSqlClass::executeSQL()
{
    //Устанавливаем текущий статус выполнения
    m_currStatus.terminalId=m_connList[0].toInt();
    m_currStatus.currentStatus=CONNECT_TO_DATABASE;
    m_currStatus.statusText="Подключение к базе данных АЗС...";
    emit signalSendStatus(m_currStatus);
    //Cоздаем подключение к базе данных АЗС
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE", m_connList[0]);

    db.setHostName(m_connList[1]);
    db.setDatabaseName(m_connList[2]);
    db.setUserName("SYSDBA");
    db.setPassword(m_connList[3]);
    db.setConnectOptions("ISC_DPB_SQL_ROLE_NAME=PUBLISH");

    //Подключаемся к базе данных АЗС
    if(!db.open()){
        //Не удалось подключится
        qCritical(logCritical()) << "Невозможно подключится к базе данных АСЗ" << m_connList[0] << db.lastError().text();
        //Меняем статус выполнения
        m_currStatus.currentStatus=ERROR_OPEN_DATABASE;
        m_currStatus.statusText="Невозможно подключится к базе данных АСЗ. "+ db.lastError().text().trimmed();
        //Отправляем статус главному потоку
        emit signalSendStatus(m_currStatus);
        //Завершаем выполнение по данной АЗС
        emit finisExecute();
        return;
    }
    //Меняем статус выполнения и отправляем его в главный поток
    m_currStatus.currentStatus=EXECUTE_SQL;
    m_currStatus.statusText = "Выполнение запроса...";
    emit signalSendStatus(m_currStatus);
    //Выполняем запросы
    QSqlQuery q = QSqlQuery(db);
    foreach(const QString &strSQL, m_listSQL ) {
            if(!q.exec(strSQL)){
                qCritical(logCritical()) << "Не удалось выполнить запрос." << strSQL << "На базе АЗС" << m_connList[0] << q.lastError().text();
                //Меняем статус выполнения и отправляем его в главный поток
                m_currStatus.currentStatus=ERROR_EXECUTE_SQL;
                m_currStatus.statusText = "Не удалось выполнить запрос. " + strSQL + " " + q.lastError().text();
                emit signalSendStatus(m_currStatus);
                emit finisExecute();
                return;
            } else {
                if(m_sqlType != SIMPLE_SQL){
                    AzsFuelName _fuelName;
                    _fuelName.setTerminalID(m_connList[0].toInt());
                    while(q.next()){
                        _fuelName.insertFuelName(q.value(0).toInt(),q.value(1).toInt(),q.value(2).toString(),q.value(3).toString());
                    }
                    q.finish();
                    q.prepare("select trim(t.NAME) from terminals t WHERE t.TERMINAL_ID = :terminalID");
                    q.bindValue(":terminalID", m_currStatus.terminalId);
                    q.exec();
                    q.next();
                    _fuelName.setAzsName(q.value(0).toString());
                    q.finish();
                    emit signalSendAzsFuelName(_fuelName);
                }
            }
    }
    m_currStatus.currentStatus=FINISHED;
    m_currStatus.statusText = "Успешное выполнение!";
    emit signalSendStatus(m_currStatus);
    emit finisExecute();
}
