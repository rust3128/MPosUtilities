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
    void signalSendTerminals(QList<int>*);
    void signalSendSQL(QStringList);
    void signalRunSQL();
private slots:
    void on_pushButtonRun_clicked();
    void on_plainTextEditSql_textChanged();

private:
    void getConnectionsList();                 //Получения дагнных о подключении к базам данных АЗС
private:
    Ui::RunSqlDialog *ui;
    QSqlQueryModel *m_model;
    QList<int> m_terminals;
};

#endif // RUNSQLDIALOG_H
