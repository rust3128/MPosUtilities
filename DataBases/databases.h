#ifndef DATABASES_H
#define DATABASES_H

#include <QObject>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>

class DataBases : public QObject
{
    Q_OBJECT
public:
    explicit DataBases(QObject *parent = nullptr);
    bool connectDB();               //Подключение к базе данных приложения SQLite
    bool connectCenralDB();
signals:

public slots:
private:
    void executeQueriesFromFile(QFile *file, QSqlQuery *query);
};

#endif // DATABASES_H
