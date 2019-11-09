#ifndef PERFORMANCEREADER_H
#define PERFORMANCEREADER_H

#include <QObject>
#include <CPdhQuery.h>

class PerformanceReader : public QObject
{
    Q_OBJECT
public:
    explicit PerformanceReader(QObject *parent = nullptr);

    static double currentDiskUsage();
};

#endif // PERFORMANCEREADER_H
