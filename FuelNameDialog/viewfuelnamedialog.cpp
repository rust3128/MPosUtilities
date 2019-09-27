#include "viewfuelnamedialog.h"
#include "ui_viewfuelnamedialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QPushButton>

ViewFuelNameDialog::ViewFuelNameDialog(QList<AzsFuelName> *lsFuels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFuelNameDialog),
    m_listFuelName(lsFuels)

{
    ui->setupUi(this);
    QPushButton * excelButton = new QPushButton(QIcon(":/Images/Excel.png")," Экспорт\n в Excel");
    excelButton->setIconSize(QSize(32,32));
    ui->buttonBox->addButton(excelButton, QDialogButtonBox::AcceptRole);
    ui->buttonBox->button(QDialogButtonBox::Close)->setIconSize(QSize(36,36));

    QStringList headers;
    headers << "Резервуар"<<"Код"<<"Краткое"<<"Полное";
    ui->tableWidgetView->setColumnCount(4);
    ui->tableWidgetView->setHorizontalHeaderLabels(headers);
    ui->tableWidgetView->verticalHeader()->hide();
    int colAzs = m_listFuelName->size();
    for(int i = 0; i<colAzs; ++i ){
        //Добавляем строку с номером и адресом АЗС
        int row = ui->tableWidgetView->rowCount();
        ui->tableWidgetView->insertRow(row);
        QTableWidgetItem *itemAZS = new QTableWidgetItem(QString::number(m_listFuelName->at(i).terminalID())+" "+m_listFuelName->at(i).azsName());
        itemAZS->setTextAlignment(Qt::AlignHCenter);
        itemAZS->setBackground(QColor("#aaff7f"));
        //Объединяем ячейки
        ui->tableWidgetView->setSpan(row,0,1,4);
        ui->tableWidgetView->setItem(row,0,itemAZS);
        for(int j = 0; j<m_listFuelName->at(i).listFuels().size();++j){
            //Заполняем строки наименованиями
            int rowName = ui->tableWidgetView->rowCount();
            ui->tableWidgetView->insertRow(rowName);
            ui->tableWidgetView->setItem(rowName,0, new QTableWidgetItem(QString::number(m_listFuelName->at(i).listFuels().at(j).tankID())));
            ui->tableWidgetView->item(rowName,0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetView->setItem(rowName,1, new QTableWidgetItem(QString::number(m_listFuelName->at(i).listFuels().at(j).fuelID())));
            ui->tableWidgetView->item(rowName,1)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetView->setItem(rowName,2, new QTableWidgetItem(m_listFuelName->at(i).listFuels().at(j).shortName()));
            ui->tableWidgetView->setItem(rowName,3, new QTableWidgetItem(m_listFuelName->at(i).listFuels().at(j).name()));
            ui->tableWidgetView->resizeColumnToContents(3);
        }
    }
    ui->tableWidgetView->verticalHeader()->setDefaultSectionSize(ui->tableWidgetView->verticalHeader()->minimumSectionSize());

}

ViewFuelNameDialog::~ViewFuelNameDialog()
{

    delete ui;
}

void ViewFuelNameDialog::on_buttonBox_rejected()
{
    this->reject();
}
