#include "LDIntance.h"

LDIntance::LDIntance(QObject *parent, QString instanceName) :
    QObject(parent),
    m_instanceName(instanceName)
{
    LOG << "Created new LD instance: " << instanceName;
}

LDIntance::~LDIntance()
{
    // Do nothing
}

QString LDIntance::instanceName() const
{
    return m_instanceName;
}

