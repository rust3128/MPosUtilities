BEGIN TRANSACTION;
--Таблица пользователей
CREATE TABLE IF NOT EXISTS `users` (
        `user_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        `fio`	TEXT NOT NULL,
        `password`	TEXT,
        `isactive`	TEXT NOT NULL DEFAULT 'true'
);
--Таблица опций приложения
CREATE TABLE IF NOT EXISTS `options` (
        `option_id`	INTEGER NOT NULL,
        `value`	TEXT NOT NULL,
        `comment`	TEXT,
        PRIMARY KEY(`option_id`)
);
INSERT INTO `options` (option_id,value,comment) VALUES (1000,'false','Использовать аутентификацию'), (1020,'1','Текущий пользователь системы');
--Типы логов
CREATE TABLE IF NOT EXISTS `logtype` (
        `logtipeID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        `logtext`	TEXT NOT NULL
);
INSERT INTO `logtype` (logtipeID,logtext) VALUES (1,'Вход в программу'), (2,'Изменил наименование топлива');
--Таблица логирования
CREATE TABLE IF NOT EXISTS `logs` (
        `logID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        `date`	TEXT NOT NULL,
        `userID`	INTEGER NOT NULL,
        `logtypeID`	INTEGER NOT NULL,
        `info`	TEXT
);
--Тригер автоматической утановки пользователя с ID = 1 при изменении настройки Аутентификации
CREATE TRIGGER IF NOT EXISTS use_login AFTER UPDATE ON options WHEN (SELECT value FROM options WHERE option_id =1000) = 'false' OR (SELECT value FROM options WHERE option_id =1000) = 0  BEGIN UPDATE options SET value = 1 WHERE option_id= 1020; END;
COMMIT;
