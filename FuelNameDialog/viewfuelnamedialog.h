#ifndef VIEWFUELNAMEDIALOG_H
#define VIEWFUELNAMEDIALOG_H

#include <QDialog>
#include "AzsFuelName/azsfuelname.h"

namespace Ui {
class ViewFuelNameDialog;
}

class ViewFuelNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFuelNameDialog(QList<AzsFuelName> *lsFuels, QWidget *parent = nullptr);
    ~ViewFuelNameDialog();

private slots:
    void on_buttonBox_rejected();

private:
    Ui::ViewFuelNameDialog *ui;
    QList<AzsFuelName> *m_listFuelName;        //Список наименований топлива
};

#endif // VIEWFUELNAMEDIALOG_H
