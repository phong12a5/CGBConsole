#ifndef PERFORMANCEREADER_H
#define PERFORMANCEREADER_H

#include <QObject>
#include <QList>
#include <CPdhQuery.h>
#include <QTimer>

class PerformanceReader : public QObject
{
    Q_OBJECT

private:
    explicit PerformanceReader(QObject *parent = nullptr);

public:
    static PerformanceReader* instance();
    double currentDiskUsage();
    double currentCPUUsage();
    double avgDiskUsage();

private slots:
    void onCollectDiskUsage();

private:
    QList<double> m_listDiskUsage;
    static PerformanceReader* m_instance;
    QTimer m_collectDiskUsage;
};

#endif // PERFORMANCEREADER_H
