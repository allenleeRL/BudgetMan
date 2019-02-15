#pragma once
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

class CDatabaseClient
{
public:
    CDatabaseClient()
    {
    }
    ~CDatabaseClient()
    {
    }

    bool Initialize( const std::wstring& strPath );
    int Query( std::string strBeginDate, std::string strEndDate );

private:
    bool IsLeapYear( int nYear );
    std::vector< std::string > Split( std::string strInput, std::string strDelimiter );

private:
    Document m_kData;
    // 0, 1 - 12
    std::vector< int > m_kDaysOfMonth = std::vector< int >{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
};