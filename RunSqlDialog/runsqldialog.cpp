#include "runsqldialog.h"
#include "ui_runsqldialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "SQLHighlighter/SQLHighlighter.h"
#include <QMessageBox>

static QString passConv(QString str)
{
    if(str == "twqsnrhlv")
        return "sunoiladm";
    if(str == "juoesj8=")
        return "island15";
    if(str == "hc}xwuu9;=")
        return "gaztron123";
    if(str == "lguwmkyimw?AC")
        return "kersheradm456";
    return "masterkey";
};

RunSqlDialog::RunSqlDialog(QSqlQueryModel *mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunSqlDialog),
    m_model(mod)
{
    ui->setupUi(this);
    connect(this,&RunSqlDialog::signalSendModel,ui->widget,&SelectTerminalsForm::slotGetModel);
    emit signalSendModel(m_model);
    ui->pushButtonRun->setEnabled(false);
    new SQLHighlighter(ui->textEditSql->document());
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
    getConnectionsList();

}
void RunSqlDialog::on_textEditSql_textChanged()
{
    ui->pushButtonRun->setDisabled(ui->textEditSql->toPlainText().isEmpty());
}

void RunSqlDialog::getConnectionsList()
{
    QSqlQuery q;
    QString strIN="";
    for(int i=0;i<m_terminals.size();++i){
        strIN += QString::number(m_terminals.at(i))+",";
    }
    strIN.resize(strIN.size()-1);
    QString strSQL = QString("select c.TERMINAL_ID, c.SERVER_NAME, c.DB_NAME, c.CON_PASSWORD from CONNECTIONS c "
                             "where c.TERMINAL_ID IN(%1) and c.CONNECT_ID=2").arg(strIN);
    QStringList list;
    if(!q.exec(strSQL)) qCritical(logCritical()) << "Не удалось получить список терминалов" << q.lastError().text();
    while(q.next()){
        list.clear();
        list << q.value(0).toString() << q.value(1).toString() << q.value(2).toString()  << passConv(q.value(3).toString());
        m_connectionsList.append(list);
    }
}
