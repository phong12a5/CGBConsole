#include "LDRunner.h"
#include <QThread>
#include "LDCommand.h"
#include "AppModel.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#define APP_MODEL AppModel::instance()

LDRunner::LDRunner(QString instanceName):
    m_instanceName(instanceName),
    m_setIsLDFile(false)
{

}

LDRunner::~LDRunner()
{
    LOG << m_instanceName;
    LDCommand::quitInstance(m_instanceName);
}

void LDRunner::run()
{
    LOG << "Thread ID: " << QThread::currentThreadId();

    m_checkConnectionTimer = new QTimer(this);
    m_checkConnectionTimer->setInterval(1000);
    m_checkConnectionTimer->setSingleShot(false);
    connect(m_checkConnectionTimer,SIGNAL(timeout()),this,SLOT(onCheckConnection()));

    m_checkEndScriptTimer = new QTimer(this);
    m_checkEndScriptTimer->setInterval(1000);
    m_checkEndScriptTimer->setSingleShot(false);
    connect(m_checkEndScriptTimer,SIGNAL(timeout()),this,SLOT(onCheckEnscript()));

    m_checkRunAppTimer = new QTimer(this);
    m_checkRunAppTimer->setInterval(15000);
    m_checkRunAppTimer->setSingleShot(false);
    connect(m_checkRunAppTimer,SIGNAL(timeout()),this,SLOT(onCheckRunApp()));

    LDCommand::lunchInstance(m_instanceName);

    m_checkConnectionTimer->start();
}

void LDRunner::quitRunner()
{
    LOG;
    m_checkConnectionTimer->stop();
    m_checkRunAppTimer->stop();
    m_checkEndScriptTimer->stop();
}

void LDRunner::onCheckConnection()
{
    if (LDCommand::checkConnection(m_instanceName) && !m_checkEndScriptTimer->isActive()){
        LOG << m_instanceName << " is connected";

        // Set token
        LOG << "Passing token id .." << APP_MODEL->token();
        QString value, error;

        /* Create startup.config and pass to LD */
        QJsonObject configObj;
        configObj["token"] = APP_MODEL->token();
        configObj["auto_startup"] = true;
        configObj["timeout"] = 30;
        configObj["country"] = "Vietnam";
        configObj["appname"] = "FBLite";

        QFile jsonFile("startup.config");
        jsonFile.open(QFile::WriteOnly);
        jsonFile.write(QJsonDocument(configObj).toJson());
        jsonFile.close();

        LDCommand::ld_adb_command(m_instanceName,QString("shell push startup.config %1").arg(APP_DATA_FOLDER));
        /* Created startup.config and passed to Nox*/

        // Run app
        LDCommand::runApp(m_instanceName, FARM_PACKAGE_NAME);
        m_checkRunAppTimer->start();

        QString endScptNamePath = QString(APP_DATA_FOLDER) + QString(ENDSCRIPT_FILENAME);
        LDCommand::ld_adb_command(m_instanceName,QString("shell rm %1").arg(endScptNamePath));
        m_checkEndScriptTimer->start();
        m_checkConnectionTimer->stop();
        LDCommand::sortWindow();
    }
}

void LDRunner::onCheckEnscript()
{
    QString endScptNamePath = QString(APP_DATA_FOLDER) + QString(ENDSCRIPT_FILENAME);
    QString output = LDCommand::ld_adb_command_str(m_instanceName,QString("shell [ -f %1 ] && echo true || echo false").arg(endScptNamePath)).simplified();
    if(output == "true"){
        LOG << "Output: " << output;
        emit finished();
    }
}

void LDRunner::onCheckRunApp()
{
    if (!LDCommand::isAppRunning(m_instanceName)) {
        LDCommand::runApp(m_instanceName, FARM_PACKAGE_NAME);
    }
}
