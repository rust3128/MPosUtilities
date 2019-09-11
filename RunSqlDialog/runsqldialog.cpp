#include "runsqldialog.h"
#include "ui_runsqldialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "SQLHighlighter/SQLHighlighter.h"
#include <QMessageBox>


RunSqlDialog::RunSqlDialog(QSqlQueryModel *mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunSqlDialog),
    m_model(mod)
{
    ui->setupUi(this);
    connect(this,&RunSqlDialog::signalSendModel,ui->widget,&SelectTerminalsForm::slotGetModel);
    connect(this,&RunSqlDialog::signalSendTerminals,ui->widgetProgress,&ViewProgressForm::slotGetTerminalsList);
    connect(this,&RunSqlDialog::signalSendSQL,ui->widgetProgress,&ViewProgressForm::slotGetListSQL);
    connect(this,&RunSqlDialog::signalRunSQL,ui->widgetProgress,&ViewProgressForm::slotRunSQL);
    emit signalSendModel(m_model);
    ui->pushButtonRun->setEnabled(false);
    new SQLHighlighter(ui->plainTextEditSql->document());
    ui->widgetProgress->hide();
}

RunSqlDialog::~RunSqlDialog()
{
    delete ui;
}

void RunSqlDialog::on_pushButtonRun_clicked()
{
    m_terminals.clear();
    m_terminals = ui->widget->selectedTerminals();
    if(m_terminals.size() == 0) {
        QMessageBox::warning(this, "Ошибка","Не выбран ни один терминал!");
        return;
    }
    QStringList listSQL;
    QString str=ui->plainTextEditSql->toPlainText();
    listSQL = str.split(";");
    for(auto& str : listSQL){
            str = str.trimmed();
            str = str.simplified();
    }
    listSQL.removeAll({});
    emit signalSendSQL(listSQL);
    emit signalSendTerminals(&m_terminals);
    emit signalRunSQL();
    ui->widget->hide();
    ui->widgetProgress->show();
    ui->groupBox->setEnabled(false);
}

void RunSqlDialog::on_plainTextEditSql_textChanged()
{
        ui->pushButtonRun->setDisabled(ui->plainTextEditSql->toPlainText().isEmpty());
}
