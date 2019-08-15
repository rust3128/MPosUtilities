#ifndef DATABASES_H
#define DATABASES_H

#include <QObject>

class DataBases : public QObject
{
    Q_OBJECT
public:
    explicit DataBases(QObject *parent = nullptr);
    bool connectDB();                               //Подключение к базе данных приложения SQLite
signals:

public slots:
};

#endif // DATABASES_H
