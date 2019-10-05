#ifndef LDINTANCE_H
#define LDINTANCE_H

#include <QObject>
#include "LDCommand.h"

class LDIntance : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString instanceName READ instanceName CONSTANT)

public:
    explicit LDIntance(QObject *parent, QString instanceName);
    ~LDIntance();

    QString instanceName() const;

private:
    QString m_instanceName;

private slots:
};

#endif // LDINTANCE_H
