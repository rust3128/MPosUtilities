#include "addterminalsdialog.h"
#include "ui_addterminalsdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "selectedlist.h"
#include <QKeyEvent>
#include <QCheckBox>

AddTerminalsDialog::AddTerminalsDialog(QSqlQueryModel *model, int selectType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTerminalsDialog),
    m_modelTerminals(model),
    m_selectType(selectType)
{
    ui->setupUi(this);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_modelTerminals);

    switch (m_selectType) {
    case TERMINAL_SELECTED:
        createUITerminal();
        break;
    case TERMINALS_REGION_SELECTED:
        createUITerminalsRegion();
        break;
    case REGIONS_SELECTED:
        createUIRegion();
        break;
    default:
        break;
    }
}

AddTerminalsDialog::~AddTerminalsDialog()
{
    delete ui;
}

void AddTerminalsDialog::createUITerminal()
{
    this->setWindowTitle("Выбор терминала");
    ui->pushButtonSelectAll->hide();
    ui->pushButtonDeSelectAll->hide();
    ui->tableWidgetTerminals->hide();

    int ownerRow=0;
    for (int row =0;row<m_proxyModel->rowCount();++row) {
        if(m_proxyModel->data(m_proxyModel->index(row, 2)).toInt() == 0 ){
            ui->comboBoxRegions->insertItem(row, m_proxyModel->data(m_proxyModel->index(row, 1)).toString());
            ownerRow++;
        } else {
            break;
        }
    }
    ui->comboBoxRegions->setCurrentIndex(-1);
    ui->tableViewTerminals->setModel(m_proxyModel);
    ui->tableViewTerminals->hideColumn(2);
    ui->tableViewTerminals->verticalHeader()->hide();
    ui->tableViewTerminals->resizeColumnsToContents();
    ui->tableViewTerminals->verticalHeader()->setDefaultSectionSize(ui->tableViewTerminals->verticalHeader()->minimumSectionSize());
    for(int i=0;i<ownerRow; ++i){
        ui->tableViewTerminals->hideRow(i);
    }
    ui->tableViewTerminals->setFocus();
    ui->tableViewTerminals->selectRow(ownerRow);
    // Имитируем нажатие кнопки Tab, чтобы  подсветить выбранную строку
    QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,Qt::NoModifier, "Tab");
    QApplication::sendEvent(this, pe) ;
}

void AddTerminalsDialog::createUITerminalsRegion()
{
    this->setWindowTitle("Выбор терминалов региона");
    ui->tableViewTerminals->hide();
    ui->tableWidgetTerminals->setColumnCount(3);
    ui->tableWidgetTerminals->setHorizontalHeaderLabels(QStringList() << "" << "АЗС" << "Наименование");
    ui->tableWidgetTerminals->verticalHeader()->hide();
    ui->tableWidgetTerminals->resizeColumnsToContents();
    ui->tableWidgetTerminals->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTerminals->setFocus();


    int ownerRow=0;
    for (int row =0;row<m_proxyModel->rowCount();++row) {
        if(m_proxyModel->data(m_proxyModel->index(row, 2)).toInt() == 0 ){
            ui->comboBoxRegions->insertItem(row, m_proxyModel->data(m_proxyModel->index(row, 1)).toString());
            ownerRow++;
        } else {
            break;
        }
    }
    ui->comboBoxRegions->activated(0);
}

void AddTerminalsDialog::createUIRegion()
{
    this->setWindowTitle("Выбор терминалов региона");
    ui->tableViewTerminals->hide();
    ui->frame->hide();
    ui->groupBox->setTitle("Выберите регион");
    ui->tableWidgetTerminals->setColumnCount(3);
    ui->tableWidgetTerminals->setHorizontalHeaderLabels(QStringList() << "" << "Код" << "Регион");
    ui->tableWidgetTerminals->verticalHeader()->hide();
    ui->tableWidgetTerminals->resizeColumnsToContents();
    ui->tableWidgetTerminals->horizontalHeader()->setStretchLastSection(true);

    QString ownerid = "0";

    m_proxyModel->setFilterRegExp(QRegExp("^"+ownerid+"$"));
    m_proxyModel->setFilterKeyColumn(2);
    int rowCount = m_proxyModel->rowCount();
    for(int row=0; row<rowCount; row++){
        ui->tableWidgetTerminals->insertRow(row);
        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
        layoutCheckBox->addWidget(checkBox);
        layoutCheckBox->setAlignment(Qt::AlignCenter);
        layoutCheckBox->setContentsMargins(0,0,0,0);
        checkBox->setChecked(true);
        ui->tableWidgetTerminals->setCellWidget(row,0,checkBoxWidget);
        ui->tableWidgetTerminals->setItem(row,1, new QTableWidgetItem(m_proxyModel->data(m_proxyModel->index(row, 0)).toString()));
        ui->tableWidgetTerminals->setItem(row,2, new QTableWidgetItem(m_proxyModel->data(m_proxyModel->index(row, 1)).toString()));
    }
    ui->tableWidgetTerminals->resizeColumnsToContents();
    ui->tableWidgetTerminals->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTerminals->verticalHeader()->setDefaultSectionSize(ui->tableWidgetTerminals->verticalHeader()->minimumSectionSize());


    ui->tableWidgetTerminals->setFocus();
}



void AddTerminalsDialog::on_comboBoxRegions_activated(int idx)
{
    switch (m_selectType) {
    case TERMINAL_SELECTED:
        comboBoxActivatedTerminal(idx);
        break;
    case TERMINALS_REGION_SELECTED:
        comboBoxActivatedTerminalRegions(idx);
        break;
    default:
        break;
    }

}

void AddTerminalsDialog::comboBoxActivatedTerminal(int idx)
{
    QString ownerid = QString::number(idx+1);

    m_proxyModel->setFilterRegExp(QRegExp("^"+ownerid+"$"));
    m_proxyModel->setFilterKeyColumn(2);

    ui->tableViewTerminals->setFocus();
    ui->tableViewTerminals->selectRow(0);
    QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,Qt::NoModifier, "Tab");
    QApplication::sendEvent(ui->tableViewTerminals, pe);
}

void AddTerminalsDialog::comboBoxActivatedTerminalRegions(int idx)
{
      QString ownerid = QString::number(idx+1);
      //Очищаем TableWidget
      ui->tableWidgetTerminals->clear();
      ui->tableWidgetTerminals->setRowCount(0);
      ui->tableWidgetTerminals->setHorizontalHeaderLabels(QStringList() << "" << "АЗС" << "Наименование");
      m_proxyModel->setFilterRegExp(QRegExp("^"+ownerid+"$"));
      m_proxyModel->setFilterKeyColumn(2);
      int rowCount = m_proxyModel->rowCount();
      for(int row=0; row<rowCount; row++){
          ui->tableWidgetTerminals->insertRow(row);
          QWidget *checkBoxWidget = new QWidget();
          QCheckBox *checkBox = new QCheckBox();
          QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
          layoutCheckBox->addWidget(checkBox);
          layoutCheckBox->setAlignment(Qt::AlignCenter);
          layoutCheckBox->setContentsMargins(0,0,0,0);
          checkBox->setChecked(true);
          ui->tableWidgetTerminals->setCellWidget(row,0,checkBoxWidget);
          ui->tableWidgetTerminals->setItem(row,1, new QTableWidgetItem(m_proxyModel->data(m_proxyModel->index(row, 0)).toString()));
          ui->tableWidgetTerminals->setItem(row,2, new QTableWidgetItem(m_proxyModel->data(m_proxyModel->index(row, 1)).toString()));
      }
      ui->tableWidgetTerminals->resizeColumnsToContents();
      ui->tableWidgetTerminals->horizontalHeader()->setStretchLastSection(true);
      ui->tableWidgetTerminals->verticalHeader()->setDefaultSectionSize(ui->tableWidgetTerminals->verticalHeader()->minimumSectionSize());

}


void AddTerminalsDialog::on_tableViewTerminals_doubleClicked(const QModelIndex &idx)
{
    m_selectedTerminals.append(m_proxyModel->mapToSource(idx));
    this->accept();
}

void AddTerminalsDialog::on_buttonBox_accepted()
{
    m_selectedTerminals.clear();
    if(ui->tableViewTerminals->isVisible()){
        QModelIndex sourseIdx = ui->tableViewTerminals->selectionModel()->selectedRows().first();
        m_selectedTerminals.append(m_proxyModel->mapToSource(sourseIdx));
    } else {
        int rowCount = ui->tableWidgetTerminals->rowCount();
        for(int i =0; i<rowCount; i++){
            QWidget *item = ui->tableWidgetTerminals->cellWidget(i,0);
            QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
            if(checkBox->isChecked()){
                if(m_selectType == TERMINALS_REGION_SELECTED)
                    m_selectedTerminals.append(m_proxyModel->mapToSource(m_proxyModel->index(i,0)));
                else
                    appendTerminals(i);
            }
        }
    }
    this->accept();

}

void AddTerminalsDialog::appendTerminals(int idx)
{
     QString ownerid = QString::number(idx+1);
     m_proxyModel->setFilterRegExp(QRegExp("^"+ownerid+"$"));
     m_proxyModel->setFilterKeyColumn(2);
     int rowCount = m_proxyModel->rowCount();
     for(int i=0; i<rowCount; ++i ){
         m_selectedTerminals.append(m_proxyModel->mapToSource(m_proxyModel->index(i,0)));
     }
}

QList<QModelIndex> AddTerminalsDialog::getSelectedTerminals()
{
    return m_selectedTerminals;
}
void AddTerminalsDialog::on_buttonBox_rejected()
{
    this->reject();
}

void AddTerminalsDialog::on_pushButtonDeSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerminals->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerminals->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(false);
    }
}

void AddTerminalsDialog::on_pushButtonSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerminals->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerminals->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(true);
    }
}
