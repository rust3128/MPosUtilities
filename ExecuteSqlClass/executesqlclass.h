#ifndef EXECUTESQLCLASS_H
#define EXECUTESQLCLASS_H
#include "statusthread.h"
#include "AzsFuelName/azsfuelname.h"
#include <QObject>

class ExecuteSqlClass : public QObject
{
    Q_OBJECT
public:
    explicit ExecuteSqlClass(QStringList connList, QStringList sqlList, int typeSQL, QObject *parent = nullptr);
    ~ExecuteSqlClass();
signals:
    void finisExecute();                           //Выполнение завершено
    void signalSendStatus(statusThread);           //Текущий статус выполнения
    void signalSendAzsFuelName(AzsFuelName);    //Отправка наименований видов топлива в основной поток
public slots:
    void executeSQL();
private:
    QStringList m_connList;                     //Параменты подключения к базе данный АЗС
    QStringList m_listSQL;                      //Список SQL запросов
    statusThread m_currStatus;                  //Текущий статус выполнения
    int m_sqlType;                                //Тип SQl
};

#endif // EXECUTESQLCLASS_H
