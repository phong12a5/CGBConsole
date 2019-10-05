#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "AppMain.h"
#include <QtWidgets>
#include <iostream>

//void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    QByteArray localMsg = msg.toLocal8Bit();
//    const char *file = context.file ? context.file : "";
//    const char *function = context.function ? context.function : "";
//    switch (type) {
//    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtInfoMsg:
//        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    }
//}


int main(int argc, char *argv[])
{
//    qInstallMessageHandler(myMessageOutput);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

//    while(true) {
//        LOG << "Connect: " << LDCommand::checkConnection("CGBDevice-0");
//    }

    AppMain appMain;
    appMain.initApplication(&engine);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()){
        qDebug() << "rootObject is NULL";
    }

    return app.exec();
}
