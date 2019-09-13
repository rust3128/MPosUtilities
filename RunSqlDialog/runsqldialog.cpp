#include "runsqldialog.h"
#include "ui_runsqldialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "SQLHighlighter/SQLHighlighter.h"
#include <QMessageBox>
#include <QFileDialog>


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
    ui->pushButtonClear->setDisabled(ui->plainTextEditSql->toPlainText().isEmpty());

}

void RunSqlDialog::on_pushButtonOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Открыть файл скрипта",".", "SQL файлы (*.sql);; Все файлы (*.*)");
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Ошибка",
                                 QString("Не могу прочитать файл скрипта %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }
        QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        ui->plainTextEditSql->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
    }
}

void RunSqlDialog::on_pushButtonClear_clicked()
{
    ui->plainTextEditSql->clear();
}

void RunSqlDialog::on_pushButtonClose_clicked()
{
    this->reject();
}
