#include "fuelnamedialog.h"
#include "ui_fuelnamedialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QMessageBox>

FuelNameDialog::FuelNameDialog(QSqlQueryModel *mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuelNameDialog),
    modelTerms(mod)

{
    ui->setupUi(this);
    connect(this,&FuelNameDialog::signalSendModel,ui->widgetSelectTerminals,&SelectTerminalsForm::slotGetModel);
    connect(this,&FuelNameDialog::signalSendTerminals,ui->widgetProgress,&ViewProgressForm::slotGetTerminalsList);
    connect(this,&FuelNameDialog::signalSendSQL,ui->widgetProgress,&ViewProgressForm::slotGetListSQL);
    connect(this,&FuelNameDialog::signalRunSQL,ui->widgetProgress,&ViewProgressForm::slotRunSQL);
    emit signalSendModel(modelTerms);
    ui->widgetProgress->hide();

}

FuelNameDialog::~FuelNameDialog()
{
    delete ui;
}

void FuelNameDialog::showFuelName(int typeView)
{
    m_terminals.clear();
    m_terminals = ui->widgetSelectTerminals->selectedTerminals();
    if(m_terminals.size() == 0) {
        QMessageBox::warning(this, "Ошибка","Не выбран ни один терминал!");
        return;
    }
    QStringList listSQL;
    listSQL << "select t.TANK_ID, f.FUEL_ID, f.SHORTNAME, f.NAME from FUELS f LEFT JOIN tanks t ON t.FUEL_ID = f.FUEL_ID where f.ISACTIVE='T' order by t.TANK_ID";

    emit signalSendSQL(listSQL);
    emit signalSendTerminals(&m_terminals);
    emit signalRunSQL(typeView);
    ui->widget->hide();
    ui->widgetProgress->show();

}

void FuelNameDialog::on_pushButtonView_clicked()
{
    showFuelName(SHOW_FUEL_NAME);
}

void FuelNameDialog::on_pushButtonExcel_clicked()
{
    showFuelName(EXPORT_FUEL_NAME);
}
