#include "selectterminalsform.h"
#include "ui_selectterminalsform.h"
#include "LoggingCategories/loggingcategories.h"
#include "selectedlist.h"
#include "addterminalsdialog.h"
#include <QCheckBox>



SelectTerminalsForm::SelectTerminalsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectTerminalsForm)
{
    ui->setupUi(this);
    createUI();
}

SelectTerminalsForm::~SelectTerminalsForm()
{
    delete ui;
}

void SelectTerminalsForm::createUI()
{
    //Настраиваем внешний вид TableWidget
    ui->tableWidgetTerm->setColumnCount(3);
    ui->tableWidgetTerm->setHorizontalHeaderLabels(QStringList() << "" << "АЗС" << "Наименование");
    ui->tableWidgetTerm->verticalHeader()->hide();
    ui->tableWidgetTerm->resizeColumnsToContents();
    ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
}

void SelectTerminalsForm::slotGetModel(QSqlQueryModel *mod)
{
    modTerminals = mod;
}

void SelectTerminalsForm::on_toolButtonSelectTerminal_clicked()
{
    AddTerminalsDialog *addTerm = new AddTerminalsDialog(modTerminals, TERMINAL_SELECTED,this);
    addTerm->move(this->parentWidget()->geometry().center().x() - addTerm->geometry().center().x(),
                  this->parentWidget()->geometry().center().y() - addTerm->geometry().center().y());
    if(addTerm->exec() == QDialog::Accepted){
        fillingTerminals(addTerm->getSelectedTerminals());
    }

    addTerm->deleteLater();
}


void SelectTerminalsForm::fillingTerminals(QList<QModelIndex> listIdx)
{
    int row = ui->tableWidgetTerm->rowCount();
    foreach(QModelIndex idx, listIdx) {
        bool coincides = false;
        for (int i = 0; i<row; ++i) {
            if( modTerminals->data(modTerminals->index(idx.row(),0)).toInt() == ui->tableWidgetTerm->item(i,1)->data(Qt::DisplayRole).toInt()){
                coincides = true;
                break;
            }
        }
        if(coincides)
            continue;
        ui->tableWidgetTerm->insertRow(row);
        //В столбце 0 добавляем CheckBox
        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
        layoutCheckBox->addWidget(checkBox);
        layoutCheckBox->setAlignment(Qt::AlignCenter);
        layoutCheckBox->setContentsMargins(0,0,0,0);
        checkBox->setChecked(true);
        ui->tableWidgetTerm->setCellWidget(row,0,checkBoxWidget);
        ui->tableWidgetTerm->setItem(row,1,new QTableWidgetItem(modTerminals->data(modTerminals->index(idx.row(),0)).toString()));
        ui->tableWidgetTerm->setItem(row,2,new QTableWidgetItem(modTerminals->data(modTerminals->index(idx.row(),1)).toString()));
        ui->tableWidgetTerm->sortByColumn(1,Qt::AscendingOrder);
        ui->tableWidgetTerm->resizeColumnsToContents();
        ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
        ui->tableWidgetTerm->verticalHeader()->setDefaultSectionSize(ui->tableWidgetTerm->verticalHeader()->minimumSectionSize());
        row++;
    }
}
