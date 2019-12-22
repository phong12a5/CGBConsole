#include "PerformanceReader.h"
#include <QDir>
#include <AppDefines.h>

PerformanceReader::PerformanceReader(QObject *parent) : QObject(parent)
{

}

double PerformanceReader::currentDiskUsage()
{
    QString currentDisk =  QDir::currentPath().mid(0,2);
    double retVal = -1.00;
    try
    {
        static CPdhQuery m_pdhQuery(std::tstring(_T("\\PhysicalDisk(*)\\% Disk Time")));
        std::map<std::tstring, double> m = m_pdhQuery.CollectQueryData();
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
        static CPdhQuery m_pdhQuery(std::tstring(_T("\\Processor(*)\\% Processor Time")));
        std::map<std::tstring, double> m = m_pdhQuery.CollectQueryData();
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
