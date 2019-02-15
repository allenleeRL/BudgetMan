#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "Module/Module.h"

int main( int argc, char* argv[] )
{
    std::cout << "***************************************************************" << std::endl;
    std::cout << "*                                                             *" << std::endl;
    std::cout << "*                    Budget Query Platform                    *" << std::endl;
    std::cout << "*                                                             *" << std::endl;
    std::cout << "*  Author:  Jeff, Sean, Allen                                 *" << std::endl;
    std::cout << "*  Version: 1.0                                               *" << std::endl;
    std::cout << "*  Date:    2019/02/14                                        *" << std::endl;
    std::cout << "*  Copyright (c) 2019 Reallusion, Inc. All rights reserved.   *" << std::endl;
    std::cout << "***************************************************************" << std::endl;

    CDatabaseClient kClient;
    wchar_t kBuffer[ MAX_PATH ];
    GetModuleFileName( nullptr, kBuffer, MAX_PATH );
    std::wstring::size_type nPos = std::wstring( kBuffer ).find_last_of( L"\\/" );
    std::wstring strPath = std::wstring( kBuffer ).substr( 0, nPos ) + L"\\database.json";
    if ( !kClient.Initialize( strPath ) )
    {
        std::cout << "Initialize database failed..." << std::endl;
        std::cout << "Check list:" << std::endl;
        std::cout << "1. Is Database existed." << std::endl;
        std::cout << "2. Is Database save as json file." << std::endl;
        std::cout << "3. Is Database follow json file format." << std::endl;
        system( "pause" );
        return 0;
    }
    std::cout << std::endl;
    std::cout << "Initialize database success." << std::endl;

    std::string strBeginDate = "";
    std::string strEndDate   = "";
    int nTotalBudget = 0;
    while ( true )
    {
        std::cout << std::endl;
        std::cout << "Starting to query budget from database...(Type c or C to exit)" << std::endl;
        std::cout << std::endl;
        std::cout << "Input begin date: (ex.2019/02/14)" << std::endl;
        std::getline( std::cin, strBeginDate );
        if ( strBeginDate == "c" || strBeginDate == "C" )
        {
            break;
        }

        std::cout << std::endl;
        std::cout << "Input end date: (ex.2019/02/14)" << std::endl;
        std::getline( std::cin, strEndDate );
        if ( strEndDate == "c" || strEndDate == "C" )
        {
            break;
        }

        nTotalBudget = kClient.Query( strBeginDate, strEndDate );
        std::cout << std::endl;
        std::cout << "Total budget from " << strBeginDate << " to " << strEndDate << " : " << nTotalBudget << std::endl;
        std::cout << std::endl;
        std::cout << "***************************************************************"<< std::endl;
    }

    system( "pause" );

    return 0;
}