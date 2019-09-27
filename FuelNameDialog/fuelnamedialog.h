#ifndef FUELNAMEDIALOG_H
#define FUELNAMEDIALOG_H

#include "ExecuteSqlClass/statusthread.h"
#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class FuelNameDialog;
}

class FuelNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FuelNameDialog(QSqlQueryModel *mod, QWidget *parent = nullptr);
    ~FuelNameDialog();
signals:
    void signalSendModel(QSqlQueryModel*);
    void signalSendTerminals(QList<int>*);
    void signalSendSQL(QStringList);
    void signalRunSQL(int);

private slots:
    void on_pushButtonView_clicked();
private:
    void showFuelName(int typeView);
private:
    Ui::FuelNameDialog *ui;
    QSqlQueryModel *modelTerms;
    QList<int> m_terminals;

};

#endif // FUELNAMEDIALOG_H
