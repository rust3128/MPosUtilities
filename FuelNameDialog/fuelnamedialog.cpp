#include "fuelnamedialog.h"
#include "ui_fuelnamedialog.h"
#include "LoggingCategories/loggingcategories.h"

FuelNameDialog::FuelNameDialog(QSqlQueryModel *mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuelNameDialog),
    modelTerms(mod)

{
    ui->setupUi(this);
    connect(this,&FuelNameDialog::signalSendModel,ui->widgetSelectTerminals,&SelectTerminalsForm::slotGetModel);
    emit signalSendModel(modelTerms);
    ui->widgetProgress->hide();

}

FuelNameDialog::~FuelNameDialog()
{
    delete ui;
}


