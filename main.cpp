#include "mainwindow.h"
#include "LoggingCategories/loggingcategories.h"
#include "DataBases/databases.h"
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSplashScreen>


// Умный указатель на файл логирования
static QScopedPointer<QFile>   m_logFile;

// Объявление обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Устанавливаем файл логирования
    m_logFile.reset(new QFile("MPosUtilites.log"));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);
    qInfo(logInfo()) << "Запуск программы. Версия:" << GIT_VERSION << "Дата сборки:" << BUILD_DATE;

#ifndef QT_NO_TRANSLATION
    //Определяем имя языкового файла в зависимости от текущей локали
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    //Создаем и пытаемся загрузить
    QTranslator *translator = new QTranslator(&a);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translator);
    else
        qWarning(logWarning()) << "Не удалось загрузить языковый файл.";
#endif

    QPixmap pix(":/Images/splash.png");
    QSplashScreen sp(pix);
    QString ver, message;
    ver = GIT_VERSION;
    message = "<html><head/><body><p><span style=' font-size:16pt; font-weight:600;'>Утилиты MPos.</span></p>"
              "<p><span style=' font-size:12pt;'>Идет загрузка параметров.</span></p>"
              "<p><span style=' font-size:12pt;'>Пожалуйста подождите....</span></p></body></html>";

    sp.setPixmap(pix);
    sp.showMessage(message,Qt::AlignHCenter | Qt::AlignBottom, Qt::blue);
    sp.show();




    DataBases *db = new DataBases();
    if(!db->connectDB()){
        qCritical(logCritical()) << "Аварийное завершение работы";
        return 1;
    }

    if(!db->connectCenralDB()){
        qInfo(logInfo()) << "Завершение работы. Не удалось подключится к центральной базе.";
        return 1;
    }

    MainWindow w;
    w.show();

    sp.finish(&w);
    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:
        out << "[INF] ";
        console << "Info:     " << "[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << msg << endl;
        break;
    case QtDebugMsg:
        out << "[DBG] " ;
        console << "Debug:    " << "[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << msg << endl;
        break;
    case QtWarningMsg:
        out << "[WRN] ";
        console << "Warning:  " << "[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << msg << endl;
        break;
    case QtCriticalMsg:
        out << "[CRT] ";
        console << "Critical: " << "[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << msg << endl;
        break;
    case QtFatalMsg:
        out << "[FTL] ";
        console << "Fatality: " << "[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << msg << endl;
        break;
#else
    case QtInfoMsg:     out << "[INF] "; break;
    case QtDebugMsg:    out << "[DBG] "; break;
    case QtWarningMsg:  out << "[WRN] "; break;
    case QtCriticalMsg: out << "[CRT] "; break;
    case QtFatalMsg:    out << "[FTL] "; break;
#endif

    }
    out << "*[" << context.function << " File: " << context.file << " Line: " << context.line << "]* ";
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg << endl;
    // Очищаем буферизированные данные
    out.flush();
    console.flush();
}
