#include "selectterminalsform.h"
#include "ui_selectterminalsform.h"
#include "LoggingCategories/loggingcategories.h"
//#include "FuelNameDialog/fuelnamedialog.h"

SelectTerminalsForm::SelectTerminalsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectTerminalsForm)

{
    ui->setupUi(this);
}

SelectTerminalsForm::~SelectTerminalsForm()
{
    delete ui;
}

void SelectTerminalsForm::slotGetModel(QSqlQueryModel *mod)
{
    modTerminals = mod;
}


void SelectTerminalsForm::on_toolButtonSelectTerminal_clicked()
{

}
