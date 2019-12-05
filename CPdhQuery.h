#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <tchar.h>
#include <iostream>

namespace std
{
  typedef std::basic_string<TCHAR> tstring;
  typedef std::basic_ostream<TCHAR> tostream;
  typedef std::basic_istream<TCHAR> tistream;
  typedef std::basic_ostringstream<TCHAR> tostringstream;
  typedef std::basic_istringstream<TCHAR> tistringstream;
  typedef std::basic_stringstream<TCHAR> tstringstream;
} // end namespace

#ifdef UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif

class CPdhQuery
{
public:

  // Inner exception class to report error.
  class CException
  {
  public:
    CException(std::tstring const & errorMsg) : m_errorMsg(errorMsg)  {}
    std::tstring What() const { return m_errorMsg; }
  private:
    std::tstring m_errorMsg;
  };

  //! Constructor
  explicit CPdhQuery(std::tstring const &counterPath)
    : m_pdhQuery(nullptr)
    , m_pdhStatus(ERROR_SUCCESS)
    , m_pdhCounter(nullptr)
    , m_counterPath(counterPath)
  {
      m_pdhStatus = PdhOpenQuery(nullptr, 0, &m_pdhQuery);
    if (m_pdhStatus)
    {
      throw CException(GetErrorString(m_pdhStatus));
    }

    // Specify a counter object with a wildcard for the instance.
    m_pdhStatus = PdhAddCounter(
          m_pdhQuery,
          m_counterPath.c_str(),
          0,
          &m_pdhCounter);
    if (m_pdhStatus)
    {
      GetErrorString(m_pdhStatus);
      throw CException(GetErrorString(m_pdhStatus));
    }
  }

  //! Destructor. The counter and query handle will be closed.
  ~CPdhQuery()
  {
    m_pdhCounter = nullptr;
    if (m_pdhQuery)
      PdhCloseQuery(m_pdhQuery);
  }

  //! Collect all the data since the last sampling period.
  std::map<std::tstring, double> CollectQueryData()
  {
    std::map<std::tstring, double> collectedData;

    while(true)
    {
      // Collect the sampling data. This might cause
      // PdhGetFormattedCounterArray to fail because some query type
      // requires two collections (or more?). If such scenario is
      // detected, the while loop will retry.
        m_pdhStatus = PdhCollectQueryData(m_pdhQuery);
      if (m_pdhStatus)
      {
        throw CException(GetErrorString(m_pdhStatus));
      }

      // Size of the pItems buffer
      DWORD bufferSize= 0;

      // Number of items in the pItems buffer
      DWORD itemCount = 0;

      PDH_FMT_COUNTERVALUE_ITEM *pdhItems = nullptr;

      // Call PdhGetFormattedCounterArray once to retrieve the buffer
      // size and item count. As long as the buffer size is zero, this
      // function should return PDH_MORE_DATA with the appropriate
      // buffer size.
      m_pdhStatus = PdhGetFormattedCounterArray(
        m_pdhCounter,
        PDH_FMT_DOUBLE,
        &bufferSize,
        &itemCount,
        pdhItems);

      // If the returned value is nto PDH_MORE_DATA, the function
      // has failed.
      if (PDH_MORE_DATA != static_cast<DWORD>(m_pdhStatus))
      {
        throw CException(GetErrorString(m_pdhStatus));
      }

      std::vector<unsigned char> buffer(bufferSize);
      pdhItems = reinterpret_cast<PDH_FMT_COUNTERVALUE_ITEM *>(&buffer[0]);

      m_pdhStatus = PdhGetFormattedCounterArray(
        m_pdhCounter,
        PDH_FMT_DOUBLE,
        &bufferSize,
        &itemCount,
        pdhItems);

      if (ERROR_SUCCESS != m_pdhStatus)
      {
        continue;
      }

      // Everything is good, mine the data.
      for (DWORD i = 0; i < itemCount; i++)
      {
        collectedData.insert(
          std::make_pair(
          std::tstring(pdhItems[i].szName),
          pdhItems[i].FmtValue.doubleValue)
          );
      }

      pdhItems = nullptr;
      bufferSize = itemCount = 0;
      break;
    }
    return collectedData;
  }

private:
  //! Helper function that translate the PDH error code into
  //! an useful message.
  std::tstring GetErrorString(PDH_STATUS errorCode)
  {
    HANDLE hPdhLibrary = nullptr;
    LPTSTR pMessage = nullptr;
    DWORD_PTR pArgs[] = { reinterpret_cast<DWORD_PTR>(m_searchInstance.c_str()) };
    std::tstring errorString;

    hPdhLibrary = LoadLibrary(_T("pdh.dll"));
    if (nullptr == hPdhLibrary)
    {
      std::tstringstream ss;
      ss
        << _T("Format message failed with ")
        << std::hex << GetLastError();
      return ss.str();
    }

    if (!FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      /*FORMAT_MESSAGE_IGNORE_INSERTS |*/
      FORMAT_MESSAGE_ARGUMENT_ARRAY,
      hPdhLibrary,
      static_cast<DWORD>(errorCode),
      0,
      reinterpret_cast<LPTSTR>(&pMessage),
      0,
      reinterpret_cast<va_list*>(pArgs)))
    {
      std::tstringstream ss;
      ss
        << m_counterPath
        << _T(" ")
        << _T("Format message failed with ")
        << std::hex
        << GetLastError()
        << std::endl;
      errorString = ss.str();
    }
    else
    {
      errorString += m_counterPath;
      errorString += _T(" ");
      errorString += pMessage;
      LocalFree(pMessage);
    }

    return errorString;
  }

private:
  PDH_HQUERY m_pdhQuery;
  PDH_STATUS m_pdhStatus;
  PDH_HCOUNTER m_pdhCounter;
  std::tstring m_searchInstance;
  std::tstring m_counterPath;
};
