#include "fuelnamedialog.h"
#include "ui_fuelnamedialog.h"
#include "LoggingCategories/loggingcategories.h"

FuelNameDialog::FuelNameDialog(QSqlQueryModel *mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuelNameDialog),
    modelTerms(mod)

{
    ui->setupUi(this);
    qInfo(logInfo()) << "Model Row count" << modelTerms->rowCount();
    connect(this,&FuelNameDialog::signalSendModel,ui->widget,&SelectTerminalsForm::slotGetModel);
    emit signalSendModel(modelTerms);

}

FuelNameDialog::~FuelNameDialog()
{
    delete ui;
}


