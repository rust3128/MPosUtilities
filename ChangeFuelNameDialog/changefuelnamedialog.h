#ifndef CHANGEFUELNAMEDIALOG_H
#define CHANGEFUELNAMEDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QListWidgetItem>


namespace Ui {
class ChangeFuelNameDialog;
}

class ChangeFuelNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChangeFuelNameDialog(QSqlQueryModel *mod, QWidget *parent = nullptr);
    ~ChangeFuelNameDialog();
signals:
    void signalSendModel(QSqlQueryModel*);
    void signalSendTerminals(QList<int>*);
    void signalSendSQL(QStringList);
    void signalRunSQL(int);
private slots:

    void on_pushButton_clicked();

    void on_buttonBox_rejected();

private:
    void createConnections();
    void createUI();
    void insertMigrateOptions(QString strSQL);
private:
    Ui::ChangeFuelNameDialog *ui;
    QSqlQueryModel *m_model;
    QList<int> m_terminals;
    QStringList listSQL;

};

#endif // CHANGEFUELNAMEDIALOG_H
