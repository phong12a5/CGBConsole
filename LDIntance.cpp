#include "LDIntance.h"

LDIntance::LDIntance(QObject *parent, QString instanceName, int index) :
    QObject(parent),
    m_instanceName(instanceName),
    m_index(index)
{
    LOGD << "Created new LD instance: " << instanceName;
}

LDIntance::~LDIntance()
{
    // Do nothing
}

QString LDIntance::instanceName() const
{
    return m_instanceName;
}

int LDIntance::index() const
{
    return m_index;
}

