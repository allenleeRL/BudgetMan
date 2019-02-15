// Module test code

#include <gmock/gmock.h>
#include <Windows.h>
#include "Module/Module.h"

using namespace testing;

// Classic Assertion:
//   Fatal assertion          Nonfatal assertion      Verifies
// ASSERT_EQ(val1, val2);   EXPECT_EQ(val1, val2);   val1 == val2
// ASSERT_NE(val1, val2);   EXPECT_NE(val1, val2);   val1 != val2
// ASSERT_LT(val1, val2);   EXPECT_LT(val1, val2);   val1 < val2
// ASSERT_LE(val1, val2);   EXPECT_LE(val1, val2);   val1 <= val2
// ASSERT_GT(val1, val2);   EXPECT_GT(val1, val2);   val1 > val2
// ASSERT_GE(val1, val2);   EXPECT_GE(val1, val2);   val1 >= val2
// 
// Test Case:
//   TEST( TestSuiteName, TestName ) {...}
//
// or using test fixture:
//   class TestSuiteName : public Test
//   {
//   protected:
//        virtual void SetUp() {}
// 	      CModule	m_kModule;    // your test module
//   };
//   TEST_F( TestSuiteName, TestName ) {...}


TEST( DatabaseClient, Construct )
{
    CDatabaseClient kClient;
}

TEST( DatabaseClient, Initialize )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";

    ASSERT_TRUE( kClient.Initialize( strPath ) );
}

TEST( DatabaseClient, InitializeWithEmptyPath )
{
    CDatabaseClient kClient;
    std::wstring strPath = L"";

    ASSERT_FALSE( kClient.Initialize( strPath ) );
}

TEST( DatabaseClient, InitializeWithWrongPath )
{
    CDatabaseClient kClient;
    std::wstring strPath = L"database";

    ASSERT_FALSE( kClient.Initialize( strPath ) );
}

TEST( DatabaseClient, InitializeWithNotExistFile )
{
    CDatabaseClient kClient;
    std::wstring strPath = L"TEST/database.json";

    ASSERT_FALSE( kClient.Initialize( strPath ) );
}

TEST( DatabaseClient, InitializeWithNotJsonFile )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\NotJson.json";

    ASSERT_FALSE( kClient.Initialize( strPath ) );
}

TEST( DatabaseClient, QueryNonRegularDateFormatWithEmptyString )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "", "" ), 0 );
}

TEST( DatabaseClient, QueryNonRegularDateFormatWithNumber )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2018/02/14", "2018/02/144" ), 0 );
}

TEST( DatabaseClient, QueryNonRegularDateFormatWithArbitraryValue )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "T.T", "QAQ/RR/77" ), 0 );
}

TEST( DatabaseClient, QueryNonNormalDateWithMonthOverflow )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/01/01", "2019/14/20" ), 0 );
}

TEST( DatabaseClient, QueryNonNormalDateWithDaysOverflow )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/03/60", "2019/05/90" ), 0 );
}

TEST( DatabaseClient, QueryNonNormalDateWithLeapYear )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/02/29", "2019/02/29" ), 0 );
}

TEST( DatabaseClient, Query0214_0214 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2018/02/14", "2018/02/14" ), 0 );
}

TEST( DatabaseClient, Query0301_0301 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/03/15", "2019/03/15" ), 1 );
}

TEST( DatabaseClient, Query0101_0430 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/01/01", "2019/04/30" ), 120 );
}

TEST( DatabaseClient, Query0301_0331 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/03/01", "2019/03/31" ), 31 );
}

TEST( DatabaseClient, Query0201_0228 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/02/01", "2019/02/28" ), 28 );
}

TEST( DatabaseClient, Query0215_0315 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/02/15", "2019/03/15" ), 29 );
}

TEST( DatabaseClient, Query20190101_20200101 )
{
    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    kClient.Initialize( strPath );

    ASSERT_EQ( kClient.Query( "2019/01/01", "2020/01/01" ), 241 );
}