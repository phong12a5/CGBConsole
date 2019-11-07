#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "CPdhQuery.h"

void DumpMap(std::map<std::tstring, double> const &m)
{
  std::map<std::tstring, double>::const_iterator itr = m.begin();
  while(m.end() != itr)
  {
    tcout << itr->first << " " << itr->second << std::endl;
    ++itr;
  }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);
//    int * pointer = nullptr;
//    if(true) {
//        int val = 10;
//        qDebug() << "val addr" << &val;
//        pointer = &val;
//    }
//    qDebug() << "pointer: " << pointer;
//    qDebug() << "*pointer: " << *pointer;
//    *pointer = 12;
//    qDebug() << "*pointer: " << *pointer;

    try
    {
      // uncomment to try different counter paths
      CPdhQuery pdhQuery(
//        std::tstring(_T("\\Memory\\% Committed Bytes In Use"))
        std::tstring(_T("\Thread(firefox/0)\Context Switches/sec"))
        //tstring(L"\Processor(*)\% Processor Time")
        //tstring(_T("\Processor(*)\Interrupts/sec"))
        //tstring(L"\Processor(_Total)\Interrupts/sec")
        );
      while(1)
      {
        Sleep(1000);
        DumpMap(pdhQuery.CollectQueryData());
      }
    }
    catch (CPdhQuery::CException const &e)
    {
      tcout << e.What() << std::endl;
    }


    return app.exec();
}
