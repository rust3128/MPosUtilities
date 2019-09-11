#ifndef VIEWPROGRESSFORM_H
#define VIEWPROGRESSFORM_H

#include "ExecuteSqlClass/statusthread.h"
#include "LoggingCategories/loggingcategories.h"
#include "ExecuteSqlClass/executesqlclass.h"
#include <QThread>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class ViewProgressForm;
}

class ViewProgressForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewProgressForm(QWidget *parent = nullptr);
    ~ViewProgressForm();
public slots:
    void slotGetTerminalsList(QList<int> *list);
    void slotGetListSQL(QStringList sql);
    void slotRunSQL();
    void slotGetStatusThread(statusThread status);      //Обработка статуса выполнения запроса
private:
    void getConnectionsList();
private:
    Ui::ViewProgressForm *ui;
    QList<int> *m_listTerm;
    QStringList m_listSQL;
    QList<QStringList> m_connectionsList;     //Cписок данных для подключения к базе данных АЗС
    int colError;                             //Количество неудачных попыток
//    QStringList statusText;                   //Список описанияй статуса подключений
};

#endif // VIEWPROGRESSFORM_H
