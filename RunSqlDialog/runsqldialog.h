#ifndef RUNSQLDIALOG_H
#define RUNSQLDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class RunSqlDialog;
}

class RunSqlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunSqlDialog(QSqlQueryModel *mod, QWidget *parent = nullptr);
    ~RunSqlDialog();

signals:
    void signalSendModel(QSqlQueryModel*);
private slots:
    void on_pushButtonRun_clicked();
    void on_textEditSql_textChanged();

private:
    void getConnectionsList();                 //Получения дагнных о подключении к базам данных АЗС
private:
    Ui::RunSqlDialog *ui;
    QSqlQueryModel *m_model;
    QList<int> m_terminals;
    QList<QStringList> m_connectionsList;     //Cписок данных для подключения к базе данных АЗС
};

#endif // RUNSQLDIALOG_H
