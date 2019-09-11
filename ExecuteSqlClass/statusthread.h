#ifndef STATUSTHREAD_H
#define STATUSTHREAD_H
#include <QObject>

struct statusThread             //Структура для хранения текущего статуса
{
    int terminalId;             //Номер терминала
    int currentStatus;          //Текущий статус операции получения наименования
    QString statusText;         //Описание
};

enum statusList {               //Список статусов
    CONNECT_TO_DATABASE,        //Соединение с базой данных
    EXECUTE_SQL,                //Выполнение запроса
    ERROR_OPEN_DATABASE,        //Ошибка отрытия базы данных
    ERROR_EXECUTE_SQL,          //Ошибка выполнения запроса
    FINISHED                    //Удачное завершение запроса
};

//Разрешаем использовать данный тип данных во взаимодействии сигнал-слот
Q_DECLARE_METATYPE(statusThread);

#endif // STATUSTHREAD_H
