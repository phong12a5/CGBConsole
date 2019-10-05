#include "LDRunner.h"
#include <QThread>
#include "LDCommand.h"
#include "AppModel.h"

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
        while (!value.contains(APP_MODEL->token())) {
//            LDCommand::setPropLD(m_instanceName,TOKEN_PROP_KEY,QString("@%1@").arg(APP_MODEL->token()));
//            LDCommand::getPropLD(m_instanceName,TOKEN_PROP_KEY,value,error);
//            LOG << "value: " << value;
            delay(3000);
        }

        // Run app
        LDCommand::runApp(m_instanceName, FARM_PACKAGE_NAME);
        m_checkRunAppTimer->start();

        QString endScptNameFile = ENDSCRIPT_FILENAME;
        QString endScptNamePath = ENDSCRIPT_PATH + endScptNameFile;
        LDCommand::ld_adb_command(m_instanceName,QString("shell rm %1").arg(endScptNamePath));
        m_checkEndScriptTimer->start();
        m_checkConnectionTimer->stop();
    }
}

void LDRunner::onCheckEnscript()
{
    QString endScptNameFile = ENDSCRIPT_FILENAME;
    QString endScptNamePath = ENDSCRIPT_PATH + endScptNameFile;
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
