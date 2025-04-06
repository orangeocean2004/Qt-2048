#include <QFile>
#include <QDateTime>
#include <QApplication>
#include <QSqlDatabase>     
#include <QCoreApplication> 
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // 设置错误日志
    QFile logFile("2048_error.log");
    logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    if (logFile.isOpen())
    {
        qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg)
                               {
            static QFile *logFilePtr = new QFile("2048_error.log");
            if(!logFilePtr->isOpen()) {
                logFilePtr->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
            }
            
            QTextStream out(logFilePtr);
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") 
                << msg << "\n";
            fprintf(stderr, "%s\n", msg.toLocal8Bit().constData()); });
    }

    QApplication app(argc, argv);

    try
    {
        MainWindow mainWindow;
        mainWindow.show();

        QObject::connect(&app, &QApplication::aboutToQuit, []()
                         {
    if (QSqlDatabase::contains("2048_game_connection")) {
        QSqlDatabase::removeDatabase("2048_game_connection");
    } });

        return app.exec();
    }
    catch (const std::exception &e)
    {
        QTextStream out(&logFile);
        out << "Exception: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        QTextStream out(&logFile);
        out << "Unknown exception occurred\n";
        return 1;
    }
}