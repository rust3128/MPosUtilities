#ifndef FUELNAMEDIALOG_H
#define FUELNAMEDIALOG_H

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
private slots:

private:
    Ui::FuelNameDialog *ui;
    QSqlQueryModel *modelTerms;

};

#endif // FUELNAMEDIALOG_H
