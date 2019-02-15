#include <gmock/gmock.h>

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    testing::FLAGS_gtest_filter = "-*._*";

    int nRet = RUN_ALL_TESTS();

#ifdef _DEBUG
    system( "pause" );
#endif
    return nRet;
}