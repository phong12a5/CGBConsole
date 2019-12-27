#include "PerformanceReader.h"
#include <QDir>
#include <AppDefines.h>

PerformanceReader* PerformanceReader::m_instance = nullptr;

CPdhQuery PerformanceReader::m_pdhCpuQuery = CPdhQuery(std::tstring(_T("\\Processor(*)\\% Processor Time")));
CPdhQuery PerformanceReader::m_pdhDiskQuery = CPdhQuery(std::tstring(_T("\\PhysicalDisk(*)\\% Disk Time")));

PerformanceReader::PerformanceReader(QObject *parent) : QObject(parent)
{
    m_listDiskUsage.clear();
    m_collectDiskUsage.setInterval(10000);
    m_collectDiskUsage.setSingleShot(false);
    connect(&m_collectDiskUsage, &QTimer::timeout, this, &PerformanceReader::onCollectDiskUsage);
    m_collectDiskUsage.start();
}

PerformanceReader *PerformanceReader::instance()
{
    if(m_instance == nullptr){
        m_instance = new PerformanceReader();
    }
    return m_instance;
}

double PerformanceReader::currentDiskUsage()
{
    QString currentDisk =  QDir::currentPath().mid(0,2);
    double retVal = -1.00;
    try
    {
        std::map<std::tstring, double> m = m_pdhDiskQuery.CollectQueryData();
        std::map<std::tstring, double>::const_iterator itr = m.begin();
        while(m.end() != itr)
        {
            QString cc = QString::fromStdWString(itr->first);
            //            tcout << itr->first << " " << itr->second << std::endl;
            if(cc.contains(currentDisk)){
//                LOGD(QString("Usage of cucurrentDisk: %1").arg(itr->second));
                retVal = itr->second;
                break;
            }
            ++itr;
        }
    }
    catch (CPdhQuery::CException const &e)
    {
        tcout << e.What() << std::endl;
    }
    return retVal;
}

double PerformanceReader::currentCPUUsage()
{
    double retVal = -1.00;
    try
    {
        std::map<std::tstring, double> m = m_pdhCpuQuery.CollectQueryData();
        std::map<std::tstring, double>::const_iterator itr = m.begin();
        while(m.end() != itr)
        {
            QString cc = QString::fromStdWString(itr->first);
            if(cc == "_Total"){
//                LOGD(QString("Usage of cucurrentCPU: %1").arg(itr->second));
                retVal = itr->second;
                break;
            }
            ++itr;
        }
    }
    catch (CPdhQuery::CException const &e)
    {
        tcout << e.What() << std::endl;
    }
    return retVal;
}

int PerformanceReader::cpuCoreCount()
{
    int count = 0;
    try
    {
        std::map<std::tstring, double> m = m_pdhCpuQuery.CollectQueryData();
        std::map<std::tstring, double>::const_iterator itr = m.begin();
        while(m.end() != itr)
        {
            QString cc = QString::fromStdWString(itr->first);
            if(cc == "_Total"){
                break;
            }
            ++count;
            ++itr;
        }
    }
    catch (CPdhQuery::CException const &e)
    {
        tcout << e.What() << std::endl;
    }
    return count;
}

double PerformanceReader::avgDiskUsage()
{
    double sum = 0;
    foreach(double diskusage, m_listDiskUsage){
        sum += diskusage;
    }
    if(m_listDiskUsage.length() == 0)
        return this->currentDiskUsage();
    else
        return sum/static_cast<double>(m_listDiskUsage.length());
}

void PerformanceReader::onCollectDiskUsage()
{
    if(m_listDiskUsage.length() > 6)
        m_listDiskUsage.removeFirst();
    m_listDiskUsage.append(this->currentDiskUsage());
    qDebug() << m_listDiskUsage;
    qDebug() << this->avgDiskUsage();
}
