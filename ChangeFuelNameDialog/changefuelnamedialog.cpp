#include "changefuelnamedialog.h"
#include "ui_changefuelnamedialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QRadioButton>
#include <QMessageBox>
#include <QDate>

ChangeFuelNameDialog::ChangeFuelNameDialog(QSqlQueryModel *mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeFuelNameDialog),
    m_model(mod)
{
    ui->setupUi(this);
    createConnections();
    emit signalSendModel(m_model);

    this->setStyleSheet("QRadioButton::checked {border: 1px solid darkgray; border-radius: 6px; background-color: '#81F781'; width: 10px; height: 10px; margin-left: 5px;}");
    this->setStyleSheet("QRadioButton::unchecked {border: 1px solid darkgray; border-radius: 6px; background-color: white; width: 10px; height: 10px; margin-left: 5px;}");

    createUI();
}

ChangeFuelNameDialog::~ChangeFuelNameDialog()
{
    delete ui;
}


void ChangeFuelNameDialog::createConnections()
{
    connect(this,&ChangeFuelNameDialog::signalSendModel,ui->widgetTerminals,&SelectTerminalsForm::slotGetModel);
    connect(this,&ChangeFuelNameDialog::signalSendTerminals,ui->widgetProgress,&ViewProgressForm::slotGetTerminalsList);
    connect(this,&ChangeFuelNameDialog::signalSendSQL,ui->widgetProgress,&ViewProgressForm::slotGetListSQL);
    connect(this,&ChangeFuelNameDialog::signalRunSQL,ui->widgetProgress,&ViewProgressForm::slotRunSQL);
}


void ChangeFuelNameDialog::createUI()
{
    ui->widgetProgress->hide();
    QDate currDate = QDate::currentDate();
    ui->dateEdit->setMinimumDate(currDate);
    ui->dateEdit->setDate(currDate);
    QSqlDatabase db = QSqlDatabase::database("options");
    QSqlQuery q = QSqlQuery(db);

    q.exec("select f.fuelidmpos, f.shotname, f.rroname from fuelname f");
    while(q.next()){
        QString fName = q.value(2).toString();
        QRadioButton *rBut = new QRadioButton(fName);
        rBut->setStyleSheet("QRadioButton::indicator::unchecked{	image: url(:/Images/radio_button_unchecked.png);}"
                            "QRadioButton::indicator::checked{image: url(:/Images/radio_button_checked.png);}");
//        rBut->setStyleSheet("font: 14pt;");

        rBut->setIconSize(QSize(25,25));
        QFont font = rBut->font();
        font.setPointSize(14);
        rBut->setFont(font);
        if(fName.indexOf("-Л-")>0)
            rBut->setIcon(QIcon(":/Images/summer.png"));
        else
            rBut->setIcon(QIcon(":/Images/winter.png"));

        switch (q.value(0).toInt()) {
        case 7:
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidgetDP);
            ui->listWidgetDP->setItemWidget(item, rBut);
        }
            break;
        case 8:
        {
            QListWidgetItem *itemV = new QListWidgetItem(ui->listWidgetVPD);
            ui->listWidgetVPD->setItemWidget(itemV, rBut);
        }
            break;
        default:
            break;
        }
    }

}



void ChangeFuelNameDialog::on_pushButton_clicked()
{
    m_terminals.clear();
    m_terminals = ui->widgetTerminals->selectedTerminals();
    if(m_terminals.size() == 0) {
        QMessageBox::warning(this, "Ошибка","Не выбран ни один терминал!");
        return;
    }

    listSQL.clear();
    QString script;
    QString VPName="";
    QString dtName="";
    if(ui->groupBoxDP->isChecked()){
        int rowCount = ui->listWidgetDP->count();
        for(int i=0; i<rowCount; i++){
            QRadioButton *radioBt = qobject_cast<QRadioButton*>(ui->listWidgetDP->itemWidget(ui->listWidgetDP->item(i)));
            if(radioBt->isChecked()){
               dtName = radioBt->text();
            }
        }
    }
    if(ui->groupBoxVPD->isChecked()){
        int rowCount = ui->listWidgetVPD->count();
        for(int i=0; i<rowCount; i++){
            QRadioButton *radioBt = qobject_cast<QRadioButton*>(ui->listWidgetVPD->itemWidget(ui->listWidgetVPD->item(i)));
            if(radioBt->isChecked()){
               VPName = radioBt->text();
            }
        }
    }
    if(dtName.size() == 0 && VPName.size() == 0){
        QMessageBox::warning(this, "Ошибка","Не выбрано ни одного наименования!");
        return;
    }
    script.clear();
    if(dtName.size()>0)
        script += "UPDATE FUELS SET NAME = '"+dtName+"' WHERE FUEL_ID = 7;\n";
    if(VPName.size()>0)
        script += "UPDATE FUELS SET NAME = '"+VPName+"' WHERE FUEL_ID = 8;\n";
    script += "UPDATE OR INSERT INTO MIGRATEOPTIONS (MIGRATEOPTION_ID, SVALUE, VTYPE) VALUES (3400, '"+ui->dateEdit->date().toString("yyyyMMdd")+"', 'D') MATCHING (MIGRATEOPTION_ID);\n";
    script += "UPDATE OR INSERT INTO MIGRATEOPTIONS (MIGRATEOPTION_ID, SVALUE, VTYPE) VALUES (3410, '6', 'I') MATCHING (MIGRATEOPTION_ID);\n";
    script += "commit;\n";

    int result = QMessageBox::question(this,"Подтверждение операци",
                                       QString("На выбранных станциях %1 будут установлены следующие наименования:\n"
                                               "%2\n"
                                               "%3\n\n"
                                               "Подтверждаете операцию?").arg(ui->dateEdit->date().toString("dd.MM.yyyy")).arg(dtName).arg(VPName));
    if(result == QMessageBox::Yes){
        qDebug(logDebug()) << endl << script;
        QMessageBox::information(this,"script",script);
        insertMigrateOptions(script);
//        ui->widgetProgress->show();
//        ui->widgetTerminals->hide();
//        ui->frame->setEnabled(false);
//        emit signalSendSQL(listSQL);
//        emit signalSendTerminals(&m_terminals);
//        emit signalRunSQL(SIMPLE_SQL);
    } else {
        ui->groupBoxDP->setChecked(false);
        ui->groupBoxVPD->setChecked(false);
    }

}

void ChangeFuelNameDialog::insertMigrateOptions(QString strSQL)
{
    int runScriptCroupID;
    QSqlQuery q;
    q.exec("SELECT GEN_ID(GEN_RUNSCRIPTSGROUPS,1) FROM RDB$DATABASE");
    q.next();
    runScriptCroupID = q.value(0).toInt();
    q.finish();
    foreach (int terminalID, m_terminals) {
        q.prepare("INSERT INTO RUNSCRIPTS (TERMINAL_ID, RUNSCRIPT_ID, RUNSCRIPTSGROUP_ID, CONNECT_ID, RUNDAT, SQLDATA, NEED_RESTART, APPLY, ISERROR, RESTARTMSG, RUNSRIPT_TYPE, DPROCESSED) "
                       "VALUES (:terminalID, GEN_ID(GEN_RUNSCRIPTS,1), :runScriptGroupID, 2, :scriptDate, :script, 'F', 'F', 'F', '', 0, NULL)");
        q.bindValue(":terminalID", terminalID);
        q.bindValue(":runScriptGroupID",runScriptCroupID);
        q.bindValue(":scriptDate", ui->dateEdit->date().toString("yyyy-MM-dd ")+"23:15:00");
        q.bindValue(":script" , strSQL);
        if(!q.exec()) qCritical(logCritical()) << "Не возможно добавить скрипт для АЗС" << terminalID << q.lastError().text();
    }
}

void ChangeFuelNameDialog::on_buttonBox_rejected()
{
    this->reject();
}
