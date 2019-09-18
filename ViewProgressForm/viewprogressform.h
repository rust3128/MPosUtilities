#ifndef VIEWPROGRESSFORM_H
#define VIEWPROGRESSFORM_H


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
    void slotRunSQL(int type);
    void slotGetStatusThread(statusThread status);      //Обработка статуса выполнения запроса
    void slotGetAzsFuelName(AzsFuelName azsFuelname);   //Получение списка наименований по терминалу
private:
    void getConnectionsList();
private:
    Ui::ViewProgressForm *ui;
    QList<int> *m_listTerm;
    QStringList m_listSQL;
    QList<QStringList> m_connectionsList;     //Cписок данных для подключения к базе данных АЗС
    QList<AzsFuelName> m_listFuelName;        //Список наименований топлива
    int m_colError;                             //Количество неудачных попыток
    int m_typeSQL;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // VIEWPROGRESSFORM_H
