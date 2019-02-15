#include "Module.h"
#include <iostream>
#include <fstream>
#include <regex>

bool CDatabaseClient::Initialize( const std::wstring& strPath )
{
    if ( strPath.empty() || strPath.find( L".json" ) == std::wstring::npos )
    {
        return false;
    }

    std::ifstream kFile( strPath );
    if ( !kFile.good() )
    {
        return false;
    }

    FILE* pFile = _wfopen( strPath.c_str(), L"rb" );
    if ( !pFile )
    {
        return false;
    }
    char kReadBuffer[ 65536 ];
    FileReadStream kStream( pFile, kReadBuffer, sizeof( kReadBuffer ) );

    auto& kRet = m_kData.ParseStream( kStream );

    fclose( pFile );
    return ( kRet.GetParseError() == ParseErrorCode::kParseErrorNone ) ? true : false;
}

int CDatabaseClient::Query( std::string strBeginDate, std::string strEndDate )
{
    if ( !m_kData.IsObject() )
    {
        return 0;
    }

    std::regex kDatePattern( "^[0-9]{4}/[0-9]{2}/[0-9]{2}$" );
    if ( !std::regex_match( strBeginDate, kDatePattern ) || !std::regex_match( strEndDate, kDatePattern ) )
    {
        return 0;
    }

    std::vector< std::string > kBeginData = Split( strBeginDate, "/" );
    std::vector< std::string > kEndDate   = Split( strEndDate, "/" );
    if ( kBeginData.empty() || kEndDate.empty() )
    {
        return 0;
    }

    int nBeginYear  = std::stoi( kBeginData[ 0 ] );
    int nBeginMonth = std::stoi( kBeginData[ 1 ] );
    int nBeginDay   = std::stoi( kBeginData[ 2 ] );
    int nEndYear    = std::stoi( kEndDate[ 0 ] );
    int nEndMonth   = std::stoi( kEndDate[ 1 ] );
    int nEndDay     = std::stoi( kEndDate[ 2 ] );
    if ( nBeginMonth > m_kDaysOfMonth.size() - 1 || nEndMonth > m_kDaysOfMonth.size() - 1 )
    {
        return 0;
    }
    if ( IsLeapYear( nBeginYear ) )
    {
        if ( nBeginMonth == 2 && nBeginDay > m_kDaysOfMonth[ nBeginMonth ] + 1 || ( nEndMonth == 2 && nEndDay > m_kDaysOfMonth[ nEndMonth ] + 1 )
        || ( nBeginMonth != 2 && nBeginDay > m_kDaysOfMonth[ nBeginMonth ] || nEndMonth != 2 && nEndDay > m_kDaysOfMonth[ nEndMonth ] ) )
        {
            return 0;
        }
    }
    else if ( nBeginDay > m_kDaysOfMonth[ nBeginMonth ] || nEndDay > m_kDaysOfMonth[ nEndMonth ] )
    {
        return 0;
    }

    int nTotalBudget = 0;
    for ( int y = nBeginYear; y <= nEndYear; ++y )
    {
        std::string strYear = std::to_string( y ).c_str();
        if ( !m_kData.HasMember( strYear.c_str() ) )
        {
            continue;
        }
        auto kThisYearBudget = m_kData[ strYear.c_str() ].GetObject();

        // 處理不同年份的情況
        int nCurrentBeginMonth = ( y == nBeginYear ) ? nBeginMonth : 1;
        int nCurrentEndMonth   = ( y == nEndYear )   ? nEndMonth   : 12;

        for ( int m = nCurrentBeginMonth; m <= nCurrentEndMonth; ++m )
        {
            std::string strMonth = std::to_string( m ).c_str();
            if ( !kThisYearBudget.HasMember( strMonth.c_str() ) )
            {
                continue;
            }

            int nDays = m_kDaysOfMonth[ m ];
            int nThisMonthBudget = kThisYearBudget[ strMonth.c_str() ].GetInt();
            int nOneDayBudget    = nThisMonthBudget / nDays;

            // 處理2月份天數問題
            if ( m == 2 && ( nThisMonthBudget % m_kDaysOfMonth[ m ] != 0 ) )
            {
                nOneDayBudget = nThisMonthBudget / m_kDaysOfMonth[ m ] + 1;
            }

            // 處理起始結束天數問題
            int nCurrentBeginDay = ( y == nBeginYear ) && ( m == nCurrentBeginMonth ) ? nBeginDay : 1;
            int nCurrentEndDay   = ( y == nEndYear ) && ( m == nCurrentEndMonth ) ? nEndDay : nDays;
            for ( int d = nCurrentBeginDay; d <= nCurrentEndDay; ++d )
            {
                nTotalBudget += nOneDayBudget;
            }
        }
    }
    return nTotalBudget;
}

bool CDatabaseClient::IsLeapYear( int nYear )
{
    return ( nYear % 4 ) == 0 && ( nYear % 100 ) != 0 || ( nYear % 400 ) == 0;
}

std::vector< std::string > CDatabaseClient::Split( std::string strInput, std::string strDelimiter )
{
    size_t nPosStart = 0;
    size_t nPosEnd   = 0;
    size_t nDelimLen = strDelimiter.length();
    std::string strToken;
    std::vector< std::string > kOutput;

    while ( ( nPosEnd = strInput.find( strDelimiter, nPosStart ) ) != std::string::npos )
    {
        strToken = strInput.substr( nPosStart, nPosEnd - nPosStart );
        nPosStart = nPosEnd + nDelimLen;
        kOutput.push_back( strToken );
    }

    kOutput.push_back( strInput.substr( nPosStart ) );
    return kOutput;
}