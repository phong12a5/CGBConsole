#include "Utility.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QJsonArray>
#include <QProcess>

Utility::Utility(QObject *parent) : QObject(parent)
{

}

void Utility::initImage()
{
    //login
//    IMGloadingfacebook = convertQImage2Mat(QImage(":/images/TEMPLATES/loadingfacebook.PNG"));
//    IMGlogin = convertQImage2Mat(QImage(getExecutedPath()+"/images/DataClick/nut/login/nutDangNhap/11.PNG"));
//    IMGlogin4 = convertQImage2Mat(QImage(":/images/TEMPLATES/login4.PNG"));
//    IMGusernameinput = convertQImage2Mat(QImage(":/images/TEMPLATES/usernameinput.PNG"));
//    IMGpasswordinput = convertQImage2Mat(QImage(":/images/TEMPLATES/passwordinput.PNG"));

//    IMGlogincoderequired = convertQImage2Mat(QImage(":/images/TEMPLATES/logincoderequired.PNG"));
//    IMGbtnOk = convertQImage2Mat(QImage(":/images/TEMPLATES/btnOk.PNG"));

//    IMGcontinue = convertQImage2Mat(QImage(":/images/TEMPLATES/continue.PNG"));
//    IMGlogincodeinput = convertQImage2Mat(QImage(":/images/TEMPLATES/logincodeinput.PNG"));
//    IMGsavelogininfo = convertQImage2Mat(QImage(":/images/TEMPLATES/saveyourlogin.PNG"));
//    IMGmaininterfaceheader = convertQImage2Mat(QImage(":/images/TEMPLATES/maininterfaceheader.PNG"));
//    //interaction
//    IMGlikebtn = convertQImage2Mat(QImage(":/images/TEMPLATES/likebtn.PNG"));
//    IMGcommentbtn = convertQImage2Mat(QImage(":/images/TEMPLATES/commentbtn.PNG"));

//    //error
//    IMGnointernet = convertQImage2Mat(QImage(":/images/TEMPLATES/nointernet.PNG"));
//    IMGaccountdisable = convertQImage2Mat(QImage(":/images/TEMPLATES/accountdisable.PNG"));
//    IMGaccountdisable2 = convertQImage2Mat(QImage(":/images/TEMPLATES/accountdisable2.PNG"));
//    IMGincorrectpass = convertQImage2Mat(QImage(":/images/TEMPLATES/incorrectpass.PNG"));
//    IMGLockedAccount = convertQImage2Mat(QImage(":/images/TEMPLATES/accountlocked.PNG"));
}

void Utility::waitForMiliseconds(long milisecs)
{
    QElapsedTimer timer;
    timer.start();
    long starting = timer.elapsed();
    while (timer.elapsed() - starting <= milisecs) {
        //do nothing
    }
}

void Utility::writeTo(QString filePath, QString newLines, bool isUTF8)
{
    QFileInfo inf(filePath);
    if(!inf.dir().exists()){
        QDir().mkpath(inf.dir().path());
    }

    if(inf.size() > 1024*1024*1024){
        QFile::remove(filePath);
    }

    QFile file( filePath );
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
    {
        QTextStream stream( &file );
        if(isUTF8){
            stream.setCodec("UTF-8");
        }
        stream << newLines;
    }
}

void Utility::write(QString filePath, QString content, bool isUTF8)
{
    //qDebug()<<content;
    QFileInfo inf(filePath);
    if(!inf.dir().exists()){
        QDir().mkpath(inf.dir().path());
    }
    QFile file( filePath );
    if ( file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
    {
        QTextStream stream( &file );
        if(isUTF8){
            stream.setCodec("UTF-8");
        }
        stream << content;
//        file.close();
    }
}

QString Utility::read(QString filePath, bool isUTF8)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        if(isUTF8){
            stream.setCodec("UTF-8");
        }
        return stream.readAll();
    }
    return "";
}

QJsonObject Utility::ObjectFromString(const QString &in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << in << endl;
    }

    return obj;
}

QString Utility::generateCurrentTime()
{
    QDateTime time = QDateTime::currentDateTime();
    uint year = time.date().year()%2000;
    uint month = time.date().month();
    uint date = time.date().day();
    uint hour = time.time().hour();
    uint minute = time.time().minute();
    uint second = time.time().second();
    return QString("%1%2%3%4%5%6")
            .arg(year,2,10,QChar('0'))
            .arg(month,2,10,QChar('0'))
            .arg(date,2,10,QChar('0'))
            .arg(hour,2,10,QChar('0'))
            .arg(minute,2,10,QChar('0'))
            .arg(second,2,10,QChar('0'));
}

QString Utility::generateCurrentTime2()
{
    QDateTime time = QDateTime::currentDateTime();
    uint year = time.date().year()%2000;
    uint month = time.date().month();
    uint date = time.date().day();
    uint hour = time.time().hour();
    uint minute = time.time().minute();
    uint second = time.time().second();
    return QString("20%1-%2-%3 %4:%5:%6")
            .arg(year,2,10,QChar('0'))
            .arg(month,2,10,QChar('0'))
            .arg(date,2,10,QChar('0'))
            .arg(hour,2,10,QChar('0'))
            .arg(minute,2,10,QChar('0'))
            .arg(second,2,10,QChar('0'));
}

QString Utility::generateDateString()
{
    QDateTime time = QDateTime::currentDateTime();
    uint year = time.date().year()%2000;
    uint month = time.date().month();
    uint date = time.date().day();
    return QString("%1%2%3")
            .arg(year,2,10,QChar('0'))
            .arg(month,2,10,QChar('0'))
            .arg(date,2,10,QChar('0'));
}

QString Utility::getExecutedPath()
{
    return QCoreApplication::applicationDirPath();
}

int Utility::random()
{
    int rd = 0;
    do{
        rd = QRandomGenerator::global()->generate();
    }while (rd < 0);

    return rd;
}

QString Utility::getRandomNumberArray(int length)
{
    QString rt;
    while(length-->0){
        rt += QString::number(random()%10);
    }
    return rt;
}

QString Utility::getRandomString(int length)
{
    QString module = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString rt;
    while(length-->0){
        rt += module.mid(random()%module.size(),1);
    }
    return rt;
}

bool Utility::command(QStringList agruments, bool waitForFinish)
{
//    qDebug()<<agruments;
    QProcess *proc = new QProcess();
    proc->setProgram("cmd.exe");
    proc->setArguments(QStringList()<<"/c"<<agruments);

    proc->start();
    if(waitForFinish){
        bool finished = proc->waitForFinished(60000);
        if(!finished){
            qDebug()<<QString::number(proc->processId())+" hasn't finished!";
            return false;
        }
        qDebug()<<proc->readAllStandardOutput();
        delete proc;
    }else {
        connect(proc, &QProcess::stateChanged,[proc](QProcess::ProcessState state){

            if(state == QProcess::NotRunning){
                qDebug()<<proc->readAllStandardOutput();
                proc->close();
            }
        });
    }
    return true;
}

QString Utility::command(QStringList agruments)
{
    QProcess *proc = new QProcess();
    proc->setProgram("cmd.exe");
    //proc->setProgram("cmd.exe");
//    qDebug()<<"command:"<<agruments;
    proc->setArguments(QStringList()<<"/c"<<agruments);

    proc->start();
    bool finished = proc->waitForFinished();
    if(!finished){
        qDebug()<<QString::number(proc->processId())+" hasn't finished!";
    }
    QString out = proc->readAll();
    //not understand why out is null if not have qDebug()<<out
//    qDebug()<<out;
    delete proc;
    return out;
}

QJsonArray Utility::getFakeLink()
{
    QJsonDocument doc = QJsonDocument::fromJson(read(Utility::getExecutedPath()+"/data/proxy/fake.ifo").toUtf8());
    if(!doc.isNull()
            && !doc.isEmpty()){
        return doc.array();
    }
    return QJsonArray();
}
