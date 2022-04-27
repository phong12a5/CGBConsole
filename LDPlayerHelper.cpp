#include "LDPlayerHelper.h"

#include <QDirIterator>

const QString LDPlayerHelper::dnconsole = "dnconsole.exe";
const QString LDPlayerHelper::ldconsole = "ldconsole.exe";
const QString LDPlayerHelper::adbshell = "adb.exe";

QString LDPlayerHelper::workspace = "";

LDPlayerHelper::LDPlayerHelper(QObject *parent) : QObject(parent)
{
    ldPlayerPath = ConfigHelper::GetLDPlayerLibsPath();
}

LDPlayerHelper::~LDPlayerHelper()
{

}

void LDPlayerHelper::refreshPath()
{
    ldPlayerPath = ConfigHelper::GetLDPlayerLibsPath();
}

void LDPlayerHelper::refreshServer()
{
    QStringList checkDeviceAgr;
    checkDeviceAgr<<"devices";
    QStringList killServerAgr;
    killServerAgr<<"kill-server";
    qDebug()<<__FUNCTION__<<Runcmd2(adbshell,killServerAgr);
    Utility::waitForMiliseconds(500);
    qDebug()<<__FUNCTION__<<Runcmd2(adbshell,checkDeviceAgr);
}

double LDPlayerHelper::dataSizeOf(int dncsId)
{
    QString dataPath = ldPlayerPath+"/vms/leidian"+QString::number(dncsId);
    QDir dir(dataPath);
    const auto &list = dir.entryInfoList();
    quint64 size = 0;
    for(const QFileInfo &dir : list) {
      size += dir.size();
    }
     qDebug()<<"Size:"<<(size*1.f/1073741824);
    return size*1.f/1073741824;
}

void LDPlayerHelper::cleanData(int dncsId)
{
    QString dataPath = ldPlayerPath+"/vms/leidian"+QString::number(dncsId);
    QString rootDataPath = ldPlayerPath+"/vms/leidian"+QString::number(0);

    QDirIterator it(rootDataPath, QDirIterator::Subdirectories);
    QDir dir(rootDataPath);
    const int absSourcePathLength = dir.absoluteFilePath(rootDataPath).length();

    while (it.hasNext()){
        it.next();
        const auto fileInfo = it.fileInfo();
        if(!fileInfo.isHidden()) { //filters dot and dotdot
            const QString subPathStructure = fileInfo.absoluteFilePath().mid(absSourcePathLength);
            const QString constructedAbsolutePath = dataPath + subPathStructure;

            if(fileInfo.isDir()){
                //Create directory in target folder
                dir.mkpath(constructedAbsolutePath);
            } else if(fileInfo.isFile()) {
                //Copy File to target directory

                //Remove file at target location, if it exists, or QFile::copy will fail
                QFile::remove(constructedAbsolutePath);
                QFile::copy(fileInfo.absoluteFilePath(), constructedAbsolutePath);
            }
        }
    }
}

//publics
void LDPlayerHelper::initWorkspace()
{
    workspace = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"\\FBAutoQt";
    if(!QDir(workspace).exists()){
        QDir().mkdir(workspace);
    }

}

void LDPlayerHelper::start(int index)
{
    Runcmd(ldconsole,QStringList()<<"launch"<<"--index"<<QString::number(index));
}

void LDPlayerHelper::killAllReferencesProcess(bool isInStated)
{
    if(isInStated){
        Utility::command(QStringList()<<"TASKKILL"<<"/F"<<"/IM"<<"adb*",true);
        Utility::waitForMiliseconds(500);
    }
    Utility::command(QStringList()<<"TASKKILL"<<"/F"<<"/IM"<<"ldconsole*",true);
    Utility::waitForMiliseconds(500);
    if(isInStated){
        Utility::command({"taskkill","/f","/im","Ld*"});
        Utility::waitForMiliseconds(500);
    }
    Utility::command({ConfigHelper::GetLDPlayerLibsPath()+"/adb.exe","devices"});
    Utility::waitForMiliseconds(500);


}

void LDPlayerHelper::closeAllLDPLayer()
{
    ldConsoleCommand(QStringList()<<"quitall",true);
}

bool LDPlayerHelper::cloneLDPlayer(int numClone)
{
    while (numClone-->0) {
        if(!Runcmd(ldconsole,QStringList()<<"copy"<<"--from"<<"0",true)){
            QMessageBox *box = new QMessageBox();
            box->setText("Đường dẫn LDPlayer không đúng, hoặc không tồn tại!");
            box->show();
            return false;
        }
    }
    return true;
}

bool LDPlayerHelper::copyFrom(QString name, int fromIndex)
{
    if(name != ""){
        return Runcmd(ldconsole,{"copy","--name",name,"--from",QString::number(fromIndex)});
    }

    return Runcmd(ldconsole,{"copy","--from",QString::number(fromIndex)});
}


bool LDPlayerHelper::createLDPlayer(int numLd)
{
    if(!validLdPath()){
        return false;
    }
    QList<LDPlayerProfile> listProfile = getListProfile();
    int next_id = listProfile.size();
    while (numLd-->0) {
        if(!Runcmd(ldconsole,QStringList()<<"add"<<"--name"<<"LDPlayer-"+QString::number(next_id++),true)){
            QMessageBox *box = new QMessageBox();
            box->setText("Lỗi! Không tạo được LDPlayer-"+QString::number(next_id-1));
            box->show();
            return false;
        }else{
            qDebug()<<"Created LDPlayer-"+QString::number(next_id-1);
        }
    }
    return true;
}

bool LDPlayerHelper::deleteLDPlayer(int dnsId)
{
    return ldConsoleCommand(QStringList()<<"remove"<<"--index"<<QString::number(dnsId),true);
}

QList<LDPlayerProfile> LDPlayerHelper::getRunningProfile()
{
    QList<LDPlayerProfile> profiles = getListProfile();
    QStringList listName = ldConsoleCommand2(QStringList()<<"runninglist").split("\r\n");
    for(int i=0; i<profiles.size(); i++){
        if(!listName.contains(profiles[i].name)){
            profiles.removeAt(i);
            i--;
        }
    }
    return profiles;
}

QList<LDPlayerProfile> LDPlayerHelper::getNotRunningProfile()
{
    QList<LDPlayerProfile> profiles = getListProfile();
    QString listName = ldConsoleCommand2(QStringList()<<"runninglist");
    for(int i=0; i<profiles.size(); i++){
        if(listName.contains(profiles[i].name)
                || profiles[i].id == 0){
            profiles.removeAt(i);
            i--;
        }
    }
    return profiles;
}



int LDPlayerHelper::getRunningCount()
{
    QString listName = ldConsoleCommand2(QStringList()<<"runninglist");
    QStringList names = listName.split("\n");
    for(int i=0;i<names.size();i++){
        if(names[i].size() < 2){
            names.removeAt(i);
        }
    }
    return names.size();
}

LDPlayerProfile LDPlayerHelper::getProfileByDnsId(int dnsId)
{
    QList<LDPlayerProfile> list = getListProfile();
    LDPlayerProfile rt = LDPlayerProfile();
    for(int i=0; i<list.size();i++){
        if(dnsId == list[i].id){
            rt = list[i];
        }
    }

    return rt;
}

LDPlayerProfile LDPlayerHelper::getProfileByName(QString name)
{
    QList<LDPlayerProfile> list = getListProfile();
    LDPlayerProfile rt = LDPlayerProfile();
    for(int i=0; i<list.size();i++){
        if(name == list[i].name){
            rt = list[i];
        }
    }

    return rt;
}

bool LDPlayerHelper::isRunning(int dncsId)
{
    LDPlayerProfile ldName = getProfileByDnsId(dncsId);
    qDebug()<<__FUNCTION__<<__LINE__<<ldName.name;
    QStringList listName = ldConsoleCommand2(QStringList()<<"runninglist").split("\r\n");
    qDebug()<<__FUNCTION__<<__LINE__<<listName;
    if(listName.contains(ldName.name)){
        return true;
    }
    return false;
}

QList<LDPlayerProfile> LDPlayerHelper::getListProfile()
{
    QString out = ldConsoleCommand2(QStringList()<<"list2");
    QStringList agrs = out.split("\r\n");
    QList<LDPlayerProfile> list = QList<LDPlayerProfile>();
    for(int i=0;i<agrs.size();i++){
        QString str = agrs.at(i);
        QStringList profilagrs = str.split(",");
        if(profilagrs.size()>6){
            LDPlayerProfile newProfile;
            newProfile.id = profilagrs.at(0).toInt();
            newProfile.name = profilagrs.at(1);
            newProfile.topHWND = profilagrs.at(2).toUInt();
            newProfile.bindHWND = profilagrs.at(3).toUInt();
            newProfile.isStarted = profilagrs.at(4).toInt()==1;
            newProfile.pid = profilagrs.at(5).toInt();
            newProfile.vboxpid = profilagrs.at(6).toInt();
            newProfile.address = getAdbAdress(newProfile.id);
            list.append(newProfile);
        }
    }
    return list;
}


QList<int> LDPlayerHelper::getListDnsId()
{
    QString out = ldConsoleCommand2(QStringList()<<"list2");
    QStringList agrs = out.split("\r\n");
    QList<int> list = QList<int>();
    for(int i=0;i<agrs.size();i++){
        QString str = agrs.at(i);
        QStringList profilagrs = str.split(",");
        if(profilagrs.size()>6){
            int dncsId = profilagrs.at(0).toInt();
            if(dncsId != 0){
                list.append(dncsId);
            }
        }
    }
    return list;
}

QString LDPlayerHelper::getAdbAdress(int dncsId)
{
    return ldConsoleCommand2(QStringList()<<"adb"<<"--index"<<QString::number(dncsId)<<"--command"<<"get-serialno").remove('\n').remove('\r');
}

bool LDPlayerHelper::ldConsoleCommand(QStringList agruments, bool waitForFinish)
{
    return Runcmd(ldconsole,agruments,waitForFinish);
}

void LDPlayerHelper::adbShellCommand(QString address,QStringList agruments, bool waitForFinish)
{
//    qDebug()<<__FUNCTION__<<agruments;
    Runcmd(adbshell,QStringList()<<"-s"<<address<<agruments,waitForFinish);
}

QString LDPlayerHelper::adbShellCommand2(QString address, QStringList agruments, int timeOut)
{
    return Runcmd2(adbshell,QStringList()<<"-s"<<address<<agruments,timeOut);
}

QString LDPlayerHelper::ldConsoleCommand2(QStringList agruments)
{
    return Runcmd2(ldconsole,agruments);
}

QByteArray LDPlayerHelper::adbShellCommand3(QString address, QStringList agruments)
{
    return Runcmd3(adbshell,QStringList()<<"-s"<<address<<agruments);
}

//private

bool LDPlayerHelper::Runcmd(QString file,QStringList agruments,bool waitForFinish)
{
    if(!validLdPath()){
        return false;
    }
//    qDebug()<<agruments;
    QProcess *proc = new QProcess();
    proc->setProgram("cmd.exe");
    proc->setArguments(QStringList()<<"/c"<<ldPlayerPath+"/"+file<<agruments);

    proc->start();
    if(waitForFinish){
        bool finished = proc->waitForFinished(90000);

        if(!finished){
            emit addLog(QString::number(proc->processId())+" hasn't finished!");
            proc->kill();
            delete proc;
            return false;
        }
//        qDebug()<<proc->readAllStandardOutput();

    }else {
        connect(proc, &QProcess::stateChanged,[proc](QProcess::ProcessState state){

            if(state == QProcess::NotRunning){
                //qDebug()<<proc->readAllStandardOutput();
                proc->close();
            }
        });
    }
    delete proc;
    return true;
}

QString LDPlayerHelper::Runcmd2(QString file, QStringList agruments,int timeOut)
{
    if(!validLdPath()){
        return "";
    }
    QProcess *proc = new QProcess();
    proc->setProgram("cmd.exe");
    //proc->setProgram("cmd.exe");
    //qDebug()<<"command:"<<ldPlayerPath+"/"+file+" "<<agruments;
    proc->setArguments(QStringList()<<"/c"<<ldPlayerPath+"/"+file<<agruments);

    proc->start();
    bool finished = proc->waitForFinished(timeOut);
    if(!finished){
        emit addLog(QString::number(proc->processId())+" hasn't finished!");
        proc->kill();
        delete proc;
        return "timeOuts";
    }
    QString out = proc->readAll();
    //not understand why out is null if not have qDebug()<<out
    //qDebug()<<out;
    delete proc;
    return out;
}

QByteArray LDPlayerHelper::Runcmd3(QString file, QStringList agruments)
{
    if(!validLdPath()){
        return "";
    }
    QProcess *proc = new QProcess();
    proc->setProgram("cmd.exe");
    //proc->setProgram("cmd.exe");
    //qDebug()<<"command:"<<ldPlayerPath+"/"+file+" "<<agruments;
    proc->setArguments(QStringList()<<"/c"<<ldPlayerPath+"/"+file<<agruments);

    proc->start();

    bool finished = proc->waitForFinished(90000);
    if(!finished){
        emit addLog(QString::number(proc->processId())+" hasn't finished!");
        proc->kill();
    }
    //not understand why out is null if not have qDebug()<<out
    //qDebug()<<out;
    //qDebug()<<proc->readAllStandardError();
    QByteArray out = proc->readAllStandardOutput();
    delete proc;
    return out;
}

bool LDPlayerHelper::validLdPath()
{
    if(!QFile(ldPlayerPath+"/"+adbshell).exists()){
        return false;
    }

    if(!QFile(ldPlayerPath+"/"+ldconsole).exists()){
        return false;
    }

    if(!QFile(ldPlayerPath+"/"+dnconsole).exists()){
        return false;
    }

    return true;
}
