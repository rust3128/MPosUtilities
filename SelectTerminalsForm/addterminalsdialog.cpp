#include "addterminalsdialog.h"
#include "ui_addterminalsdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "selectedlist.h"
#include <QKeyEvent>

AddTerminalsDialog::AddTerminalsDialog(QSqlQueryModel *model, int selectType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTerminalsDialog),
    m_modelTerminals(model),
    m_selectType(selectType)
{
    ui->setupUi(this);
    switch (m_selectType) {
    case TERMINAL_SELECTED:
        this->setWindowTitle("Выбор терминала");
        break;
    default:
        break;
    }

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_modelTerminals);
    createUI();

}

AddTerminalsDialog::~AddTerminalsDialog()
{
    delete ui;
}

void AddTerminalsDialog::createUI()
{
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
    ui->tableViewTerminals->selectRow(ownerRow);
    // Имитируем нажатие кнопки Tab, чтобы  подсветить выбранную строку
    QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,Qt::NoModifier, "Tab");
    QApplication::sendEvent(this, pe) ;
}

void AddTerminalsDialog::on_comboBoxRegions_activated(int idx)
{
    QString ownerid = QString::number(idx+1);

    m_proxyModel->setFilterRegExp(QRegExp("^"+ownerid+"$"));
    m_proxyModel->setFilterKeyColumn(2);

    ui->tableViewTerminals->setFocus();
    ui->tableViewTerminals->selectRow(0);
    QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,Qt::NoModifier, "Tab");
    QApplication::sendEvent(ui->tableViewTerminals, pe);

}

void AddTerminalsDialog::on_tableViewTerminals_doubleClicked(const QModelIndex &idx)
{
    m_selectedTerminals.append(m_proxyModel->mapToSource(idx));
    this->accept();
}

void AddTerminalsDialog::on_buttonBox_accepted()
{
    QModelIndex sourseIdx = ui->tableViewTerminals->selectionModel()->selectedRows().first();
    m_selectedTerminals.append(m_proxyModel->mapToSource(sourseIdx));
    this->accept();
}
QList<QModelIndex> AddTerminalsDialog::getSelectedTerminals()
{
    return m_selectedTerminals;
}
void AddTerminalsDialog::on_buttonBox_rejected()
{
    this->reject();
}
