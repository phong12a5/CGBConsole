#include "LDIntance.h"

LDIntance::LDIntance(QObject *parent, QString instanceName) :
    QObject(parent),
    m_instanceName(instanceName)
{
    LOG << "Created new LD instance: " << instanceName;
    LDCommand::runLDCommand(QString("modify --name %1 --cpu 1 --memory 1024 --resolution 720,1280,320").arg(instanceName));
}

LDIntance::~LDIntance()
{
    // Do nothing
}

QString LDIntance::instanceName() const
{
    return m_instanceName;
}

