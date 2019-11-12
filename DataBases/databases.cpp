#include "databases.h"
#include "databasesettings.h"
#include "LoggingCategories/loggingcategories.h"
#include "ConnectionSettingDialog/connectionsettingdialog.h"
#include <QSettings>
#include <QMessageBox>

DataBases::DataBases(QObject *parent) : QObject(parent)
{

}

bool DataBases::connectDB()
{
    bool result = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "options");
    db.setHostName(DataBaseSettings::HOSTNAME);
    db.setDatabaseName(DataBaseSettings::NAME);
    QFile dbFile(DataBaseSettings::NAME);


    //Проверяем наличие файла базы данных
    if(dbFile.exists() && (dbFile.size() > 0) ){
        if(db.open()){
            qInfo(logInfo()) << "Файл базы данных настроек открыт успешно";
            result = true;
        } else {
            qCritical(logCritical()) << "He удалось открыть базу данных настроек";
            result = false;
        }
    } else {
        if(db.open()){
            QFile sqlfile(DataBaseSettings::MAIN_SQL_FILE_NAME);
            QSqlQuery q = QSqlQuery(db);
            if(sqlfile.open(QIODevice::ReadOnly | QIODevice::Text)){
                 executeQueriesFromFile(&sqlfile,&q);
                 qInfo(logInfo()) << "Создан файл настроек приложения.";
                 result = true;
            } else {
                qCritical(logCritical()) << "Не удалось открыть файл " << sqlfile.fileName();
                result = false;
            }
        } else {
            qCritical(logCritical()) << "Не удалось создать файл настроек приложения.";
            result = false;
        }
    }

        return result;
}

void DataBases::executeQueriesFromFile(QFile *file, QSqlQuery *query)
{
    while (!file->atEnd()){
            QByteArray readLine="";
            QString cleanedLine;
            QString line="";
            bool finished=false;
            while(!finished){
                readLine = file->readLine();
                cleanedLine=readLine.trimmed();
                // remove comments at end of line
                QStringList strings=cleanedLine.split("--");
                cleanedLine=strings.at(0);

                // remove lines with only comment, and DROP lines
                if(!cleanedLine.startsWith("--")
                        && !cleanedLine.startsWith("DROP")
                        && !cleanedLine.isEmpty()){
                    line+=cleanedLine;
                }
                if(cleanedLine.endsWith(";")){
                    break;
                }
                if(cleanedLine.startsWith("COMMIT")){
                    finished=true;
                }
            }

            if(!line.isEmpty()){
                query->exec(line);
            }
            if(!query->isActive()){
                qInfo(logInfo()) << QSqlDatabase::drivers();
                qCritical(logCritical()) <<  query->lastError();
                qDebug() << "test executed query:"<< query->executedQuery();
                qDebug() << "test last query:"<< query->lastQuery();
            }
        }
}

bool DataBases::connectCenralDB()
{
    bool result = true;
    //Указываем имя файла настроек
    QSettings settings(DataBaseSettings::CONFIG_FILE_NAME, QSettings::IniFormat);
    //Создаем подключение по умолчанию к базе данных FireBird
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");

    //Установка параметров подключения из файла настроек
    settings.beginGroup("DATABASE");
    db.setHostName(settings.value("HostName").toString());
    db.setDatabaseName(settings.value("DataBase").toString());
    db.setUserName(settings.value("User").toString());
    db.setPassword(settings.value("Password").toString());
    db.setConnectOptions("ISC_DPB_LC_CTYPE=WIN1251");
    settings.endGroup();
    //Открываем базу данных, если попытка не удачная вызываем диалог настроек подключения
    if(!db.open()) {
        qCritical(logCritical()) <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << db.lastError().text();
        int rez = QMessageBox::question(nullptr, QObject::tr("Ошибка подключения"),
                              "Не могу установить соединение с базой данных.\nПроверить настройки подключения?",
                              QMessageBox::Yes | QMessageBox::No);
        if(rez == QMessageBox::Yes) {
            ConnectionSettingDialog *connDlg = new ConnectionSettingDialog();
            int result = connDlg->exec();
            if(result == QDialog::Accepted) {
                qInfo(logInfo()) << "Сохранение настроек подключения.";
            }
        }
        //Завершаем работу
        result = false;
    } else {
        qInfo(logInfo()) << "Успешное подключение к центральной базе данных.";
        result = true;
    }

    return result;

}
