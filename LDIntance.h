#ifndef LDINTANCE_H
#define LDINTANCE_H

#include <QObject>
#include "LDCommand.h"

class LDIntance : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString instanceName READ instanceName CONSTANT)

public:
    explicit LDIntance(QObject *parent, QString instanceName, int index);
    ~LDIntance();

    QString instanceName() const;
    int index() const;

private:
    QString m_instanceName;
    int m_index;

private slots:
};

#endif // LDINTANCE_H
