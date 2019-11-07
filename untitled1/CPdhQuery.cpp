#include "CPdhQuery.h"

CPdhQuery::CPdhQuery()
{

}

double CPdhQuery::CollectSingleData()
{
    double data = 0;
    while(true)
    {
        bool status = PdhCollectQueryData(hQuery);

        if (ERROR_SUCCESS != status)
        {
            throw CException(GetErrorString(status));
        }

        PDH_FMT_COUNTERVALUE cv;
        // Format the performance data record.
        status = PdhGetFormattedCounterValue(hCounter,
            PDH_FMT_DOUBLE,
            (LPDWORD)NULL,
            &cv);

        if (ERROR_SUCCESS != status)
        {
            continue;
        }

        data = cv.doubleValue;

        break;

    }

    return data;
}
