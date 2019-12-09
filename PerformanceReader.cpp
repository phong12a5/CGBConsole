#include "PerformanceReader.h"
#include <QDir>
#include <AppDefines.h>

PerformanceReader::PerformanceReader(QObject *parent) : QObject(parent)
{

}

double PerformanceReader::currentDiskUsage()
{
    QString currentDisk =  QDir::currentPath().mid(0,2);
    LOG << "currentDisk: " << currentDisk;
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
                LOG << "Usage of cucurrentDisk: " << itr->second;
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
                LOG << "Usage of cucurrentCPU: " << itr->second;
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
