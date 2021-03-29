/*! \file
    \brief Decimal type test

 */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>

#include "invest_openapi/config_helpers.h"
#include "invest_openapi/api_config.h"
#include "invest_openapi/auth_config.h"
#include "invest_openapi/currencies_config.h"

#include "invest_openapi/invest_openapi.h"
#include "invest_openapi/factory.h"
#include "invest_openapi/openapi_completable_future.h"

#include "invest_openapi/marty_decimal.h"
#include "invest_openapi/marty_long_ints.h"
#include "invest_openapi/marty_bcd.h"



#define THROW_ERROR() do{ throw std::runtime_error("Test failed"); } while(0)
//#define THROW_ERROR() do{ } while(0)


template<typename IntType1, typename IntType2, typename IntTypeRes>
void martyLongIntMultiplyTestHelper( IntType1 u1, IntType2 u2, IntTypeRes &res )
{
    std::uint16_t ul1[ marty::calcNumberOfUint16ItemsInLong<IntType1>() ];
    marty::convertToLongUnsigned( u1, &ul1[0] );

    std::uint16_t ul2[ marty::calcNumberOfUint16ItemsInLong<IntType2>() ];
    marty::convertToLongUnsigned( u2, &ul2[0] );

    std::uint16_t ulRes[ marty::calcNumberOfUint16ItemsInLong<IntType1>() + marty::calcNumberOfUint16ItemsInLong<IntType2>() ];

    marty::multiplicationOfUnsignedLongInts( &ul1[0], marty::calcNumberOfUint16ItemsInLong<IntType1>()
                                           , &ul2[0], marty::calcNumberOfUint16ItemsInLong<IntType2>()
                                           , &ulRes[0], marty::calcNumberOfUint16ItemsInLong<IntType1>() + marty::calcNumberOfUint16ItemsInLong<IntType2>()
                                           );

    marty::convertFromLongUnsigned( res, &ulRes[0], marty::calcNumberOfUint16ItemsInLong<IntType1>() + marty::calcNumberOfUint16ItemsInLong<IntType2>() );

}

template<typename IntType>
void martyLongIntSummTestHelper( IntType u1, IntType u2, IntType &res )
{
    std::uint16_t ul1[ marty::calcNumberOfUint16ItemsInLong<IntType>() ];
    marty::convertToLongUnsigned( u1, &ul1[0] );

    std::uint16_t ul2[ marty::calcNumberOfUint16ItemsInLong<IntType>() ];
    marty::convertToLongUnsigned( u2, &ul2[0] );

    std::uint16_t ulRes[ marty::calcNumberOfUint16ItemsInLong<IntType>() ];

    marty::summationOfUnsignedLongInts( &ul1[0], marty::calcNumberOfUint16ItemsInLong<IntType>()
                                      , &ul2[0], marty::calcNumberOfUint16ItemsInLong<IntType>()
                                      , &ulRes[0], marty::calcNumberOfUint16ItemsInLong<IntType>()
                                      );

    marty::convertFromLongUnsigned( res, &ulRes[0], marty::calcNumberOfUint16ItemsInLong<IntType>() );

}




INVEST_OPENAPI_MAIN()
{
    using std::cout;
    using std::endl;

    using marty::Decimal;
    using marty::DecimalPrecision;
    //using marty::toString;
    //using marty::fromString;


    // simple pretest
    {

        std::uint32_t u1 = 213251u; // u1  = 0x00034103� ul1 = { 0x4103, 0x0003 }
        std::uint32_t u2 = 153754u; // u2  = 0x0002589a  ul2  = { 0x589a, 0x0002 }
        // 0x4103 * 0x589a = 377496526 // 0x168023CE

        std::uint64_t uRes; // 213251 * 153754 = 32788194254 (0x07A25423CE)
        std::uint32_t sumRes;

        martyLongIntMultiplyTestHelper( u1, u2, uRes );
        cout << u1 << " x " << u2 << " = " << uRes << endl;

        martyLongIntSummTestHelper( u1, u2, sumRes );
        cout << u1 << " + " << u2 << " = " << sumRes << endl; // 213251+153754 = 367005


        u1 = 54213251u;
        u2 = 91153754u;

        martyLongIntMultiplyTestHelper( u1, u2, uRes );
        cout << u1 << " x " << u2 << " = " << uRes << endl;
        // 54213251 * 91153754 = 4941741345194254

        martyLongIntSummTestHelper( u1, u2, sumRes );
        cout << u1 << " + " << u2 << " = " << sumRes << endl; // 54213251+91153754 = 145367005


        u1 = 4213251u;  // 0x0040 4A03
        u2 = 1153754u;  // 0x0011 9ADA
        // 0x4A03*0x9ADA = 0x2CC4 D48E
        // 0x4A03*0x0011 = 0x0004 EA33
        //                             0x2CC9 BEC1
        // 0x0040*0x9ADA = 0x0026 B680
        //                             0x2CF0 7541
        // 0x0040*0x0011 = 0x0000 0440
        //                             0x2CF07981
        
        martyLongIntMultiplyTestHelper( u1, u2, uRes );
        cout << u1 << " x " << u2 << " = " << uRes << endl;
        // 4213251 * 1153754 =    4861055194254

        martyLongIntSummTestHelper( u1, u2, sumRes );
        cout << u1 << " + " << u2 << " = " << sumRes << endl; // 4213251+1153754 = 5367005

     }

    cout << endl;
    cout << "------------------------------" << endl;
    cout << endl;


    #define RAW_BCD_FROM_STRING_TEST( strVal, strResForCompare )                            \
                do                                                                          \
                {                                                                           \
                    ++totalRawBcdFromStringConvertTests;                                    \
                    marty::bcd::raw_bcd_number_t bcdNumber;                                 \
                    int                          bcdNumberPrecision;                        \
                                                                                            \
                    bcdNumberPrecision = marty::bcd::makeRawBcdNumber( bcdNumber, strVal ); \
                                                                                            \
                    std::string bcdStrFormatted = marty::bcd::formatRawBcdNumber( bcdNumber, bcdNumberPrecision );\
                                                                                            \
                    bool bGood = bcdStrFormatted==strResForCompare;                         \
                                                                                            \
                    if (!bGood)                                                             \
                       ++totalRawBcdFromStringConvertTestsFailed;                           \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << strVal << " converted to BCD is '" << bcdStrFormatted << "'"; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected '" << strResForCompare << "')";                 \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)


    #define RAW_BCD_COMPARE_TEST( strVal1, strVal2, cmpResForCompare )                      \
                do                                                                          \
                {                                                                           \
                    ++totalRawBcdCompareTests;                                              \
                    marty::bcd::raw_bcd_number_t bcdNumber1;                                \
                    int                          bcdNumberPrecision1;                       \
                    marty::bcd::raw_bcd_number_t bcdNumber2;                                \
                    int                          bcdNumberPrecision2;                       \
                                                                                            \
                    bcdNumberPrecision1 = marty::bcd::makeRawBcdNumber( bcdNumber1, strVal1 ); \
                    bcdNumberPrecision2 = marty::bcd::makeRawBcdNumber( bcdNumber2, strVal2 ); \
                                                                                            \
                    int cmpRes = marty::bcd::compareRaws( bcdNumber1, bcdNumberPrecision1, bcdNumber2, bcdNumberPrecision2 );\
                                                                                            \
                    bool bGood = cmpRes==cmpResForCompare;                                  \
                                                                                            \
                    if (!bGood)                                                             \
                       ++totalRawBcdCompareTestsFailed;                                     \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << strVal1 << " compare with " << strVal2 << " is " << cmpRes << ""; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected " << cmpResForCompare << ")";                   \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)

    #define RAW_BCD_ZERO_TEST( strVal1, cmpResForCompare )                                  \
                do                                                                          \
                {                                                                           \
                    ++totalRawBcdZeroTests;                                                 \
                    marty::bcd::raw_bcd_number_t bcdNumber1;                                \
                    int                          bcdNumberPrecision1;                       \
                                                                                            \
                    bcdNumberPrecision1 = marty::bcd::makeRawBcdNumber( bcdNumber1, strVal1 ); \
                                                                                            \
                    bool zeroRes = marty::bcd::checkForZero( bcdNumber1 );                  \
                                                                                            \
                    bool bGood = zeroRes==cmpResForCompare;                                 \
                                                                                            \
                    if (!bGood)                                                             \
                       ++totalRawBcdZeroTestsFailed;                                        \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << strVal1 << " is zero: " << (zeroRes?"true":"false") << ""; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected " << cmpResForCompare << ")";                   \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)


    #define RAW_BCD_OP_TEST( strVal1, strVal2, op, strResForCompare )                       \
                do                                                                          \
                {                                                                           \
                    ++totalRawBcdOpTests;                                                   \
                    marty::bcd::raw_bcd_number_t bcdNumber1;                                \
                    int                          bcdNumberPrecision1;                       \
                    marty::bcd::raw_bcd_number_t bcdNumber2;                                \
                    int                          bcdNumberPrecision2;                       \
                    marty::bcd::raw_bcd_number_t bcdNumberRes;                                 \
                    int                          bcdNumberPrecisionRes;                        \
                                                                                               \
                    bcdNumberPrecision1 = marty::bcd::makeRawBcdNumber( bcdNumber1, strVal1 ); \
                    bcdNumberPrecision2 = marty::bcd::makeRawBcdNumber( bcdNumber2, strVal2 ); \
                                                                                               \
                    bcdNumberPrecisionRes = marty::bcd:: op ( bcdNumberRes, bcdNumber1, bcdNumberPrecision1, bcdNumber2, bcdNumberPrecision2 );  \
                                                                                            \
                    std::string bcdStrFormatted = marty::bcd::formatRawBcdNumber( bcdNumberRes, bcdNumberPrecisionRes );\
                                                                                            \
                    bool bGood = bcdStrFormatted==strResForCompare;                         \
                                                                                            \
                    if (!bGood)                                                             \
                       ++totalRawBcdOpTestsFailed;                                          \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << strVal1 << " " << #op << " " << strVal2 << " = " << bcdStrFormatted << ""; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected " << strResForCompare << ")";                   \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)


    unsigned totalRawBcdFromStringConvertTests        = 0;
    unsigned totalRawBcdFromStringConvertTestsFailed  = 0;


    RAW_BCD_FROM_STRING_TEST( "3.141592654"    , "3.141592654"     );
    RAW_BCD_FROM_STRING_TEST( "314.15"         , "314.15"          );
    RAW_BCD_FROM_STRING_TEST( "31.4159"        , "31.4159"         );
    RAW_BCD_FROM_STRING_TEST( "317.291592654"  , "317.291592654"   );
    RAW_BCD_FROM_STRING_TEST( "311.008407346"  , "311.008407346"   );
    RAW_BCD_FROM_STRING_TEST( "34.557492654"   , "34.557492654"    );
    RAW_BCD_FROM_STRING_TEST( "28.274307346"   , "28.274307346"    );
    RAW_BCD_FROM_STRING_TEST( "303.141592654"  , "303.141592654"   );
    RAW_BCD_FROM_STRING_TEST( "296.858407346"  , "296.858407346"   );
    RAW_BCD_FROM_STRING_TEST( "3003.141592654" , "3003.141592654"  );
    RAW_BCD_FROM_STRING_TEST( "2996.858407346" , "2996.858407346"  );
    RAW_BCD_FROM_STRING_TEST( "0.003"          , "0.003"           );
    RAW_BCD_FROM_STRING_TEST( ".141"           , "0.141"           );
    RAW_BCD_FROM_STRING_TEST( ".141000"        , "0.141"           );
    RAW_BCD_FROM_STRING_TEST( "25.141"         , "25.141"          );
    RAW_BCD_FROM_STRING_TEST( "24.859"         , "24.859"          );
    //RAW_BCD_FROM_STRING_TEST();

    cout << endl;

    cout << "Failed " << totalRawBcdFromStringConvertTestsFailed << " BCD convert tests from total " << totalRawBcdFromStringConvertTests << endl;
    if (!totalRawBcdFromStringConvertTestsFailed)
        cout << "+++ All BCD convert tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;



    unsigned totalRawBcdZeroTests       = 0;
    unsigned totalRawBcdZeroTestsFailed = 0;

    RAW_BCD_ZERO_TEST( "3.141592654"    , false );
    RAW_BCD_ZERO_TEST( "314.15"         , false );
    RAW_BCD_ZERO_TEST( "31.4159"        , false );
    RAW_BCD_ZERO_TEST( "317.291592654"  , false );
    RAW_BCD_ZERO_TEST( "311.008407346"  , false );
    RAW_BCD_ZERO_TEST( "34.557492654"   , false );
    RAW_BCD_ZERO_TEST( "28.274307346"   , false );
    RAW_BCD_ZERO_TEST( "303.141592654"  , false );
    RAW_BCD_ZERO_TEST( "296.858407346"  , false );
    RAW_BCD_ZERO_TEST( "3003.141592654" , false );
    RAW_BCD_ZERO_TEST( "2996.858407346" , false );
    RAW_BCD_ZERO_TEST( "0.003"          , false );
    RAW_BCD_ZERO_TEST( ".141"           , false );
    RAW_BCD_ZERO_TEST( ".141000"        , false );
    RAW_BCD_ZERO_TEST( "25.141"         , false );
    RAW_BCD_ZERO_TEST( "24.859"         , false );
    RAW_BCD_ZERO_TEST( "0"              , true  );
    RAW_BCD_ZERO_TEST( "00"             , true  );
    RAW_BCD_ZERO_TEST( "000"            , true  );
    RAW_BCD_ZERO_TEST( "00.0"           , true  );
    RAW_BCD_ZERO_TEST( "0.000"          , true  );
    RAW_BCD_ZERO_TEST( "00.00"          , true  );


    cout << endl;

    cout << "Failed " << totalRawBcdZeroTestsFailed << " BCD zero tests from total " << totalRawBcdZeroTests << endl;
    if (!totalRawBcdZeroTestsFailed)
        cout << "+++ All BCD zero tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;



    unsigned totalRawBcdCompareTests       = 0;
    unsigned totalRawBcdCompareTestsFailed = 0;

    RAW_BCD_COMPARE_TEST( "3.141592654" , "314.15"      , -1 );
    RAW_BCD_COMPARE_TEST( "314.15"      , "3.141592654" ,  1 );
    RAW_BCD_COMPARE_TEST( "3.141592654" , "3.141592654" ,  0 );
    RAW_BCD_COMPARE_TEST( "3.141592654" , "31.4159"     , -1 );
    RAW_BCD_COMPARE_TEST( "31.4159"     , "3.141592654" ,  1 );
    RAW_BCD_COMPARE_TEST( "31.4159"     , "31.4159"     ,  0 );
    RAW_BCD_COMPARE_TEST( "314.15"      , "31.4159"     ,  1 );
    RAW_BCD_COMPARE_TEST( "31.4159"     , "314.15"      , -1 );
    RAW_BCD_COMPARE_TEST( "314.15"      , "314.15"      ,  0 );
    RAW_BCD_COMPARE_TEST( "314.15"      , "300.0"       ,  1 );
    RAW_BCD_COMPARE_TEST( "300"         , "314.15"      , -1 );
    RAW_BCD_COMPARE_TEST( "300"         , "300.0"       ,  0 );
    RAW_BCD_COMPARE_TEST( "3000"        , "300"         ,  1 );
    RAW_BCD_COMPARE_TEST( "300"         , "3000"        , -1 );
    RAW_BCD_COMPARE_TEST( "3000"        , "3000"        ,  0 );
    RAW_BCD_COMPARE_TEST( "3000.00"     , "3000"        ,  0 );

    cout << endl;

    cout << "Failed " << totalRawBcdCompareTestsFailed << " BCD compare tests from total " << totalRawBcdCompareTests << endl;
    if (!totalRawBcdCompareTestsFailed)
        cout << "+++ All BCD compare tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;



    unsigned totalRawBcdOpTests         = 0;
    unsigned totalRawBcdOpTestsFailed   = 0;

    RAW_BCD_OP_TEST( "3.141592654"   , "314.15"        , rawAddition       , "317.291592654"  ); // 3.141592654 + 314.15 = 317.291592654
    RAW_BCD_OP_TEST( "3.141592654"   , "31.4159"       , rawAddition       , "34.557492654"   ); // 3.141592654+31.4159 = 34.557492654
    RAW_BCD_OP_TEST( "3.141592654"   , "300"           , rawAddition       , "303.141592654"  ); // 3.141592654+300 = 303.141592654
    RAW_BCD_OP_TEST( "3.141592654"   , "3000"          , rawAddition       , "3003.141592654" ); // 3.141592654+3000 = 3003.141592654
    RAW_BCD_OP_TEST( "3.141592654"   , "0.003"         , rawAddition       , "3.144592654"    ); // 3.141592654+0.003 = 3.144592654
    RAW_BCD_OP_TEST( ".141"          , ".141000"       , rawAddition       , "0.282"          ); // .141+.141000 = 0.282
    RAW_BCD_OP_TEST( "25.000"        , ".141000"       , rawAddition       , "25.141"         ); // 25.000+.141000 = 25.141
    RAW_BCD_OP_TEST( "025.000"       , ".141000"       , rawAddition       , "25.141"         ); // 025.000+.141000 = 25.141
    //RAW_BCD_OP_TEST( ""      , "" , rawAddition   , "" ); // 

    cout << endl;

    RAW_BCD_OP_TEST( "314.15"        , "3.141592654"   , rawSubtraction    , "311.008407346"  ); // 314.15 - 3.141592654 = 311.008407346
    RAW_BCD_OP_TEST( "31.4159"       , "3.141592654"   , rawSubtraction    , "28.274307346"   ); // 31.4159-3.141592654 = 28.274307346
    RAW_BCD_OP_TEST( "300"           , "3.141592654"   , rawSubtraction    , "296.858407346"  ); // 300-3.141592654 = 296.858407346
    RAW_BCD_OP_TEST( "3000"          , "3.141592654"   , rawSubtraction    , "2996.858407346" ); // 3000-3.141592654 = 2996.858407346
    RAW_BCD_OP_TEST( "3.141592654"   , "0.003"         , rawSubtraction    , "3.138592654"    ); // 3.141592654-0.003 = 3.138592654
    RAW_BCD_OP_TEST( ".141"          , ".141000"       , rawSubtraction    , "0"              ); // .141-.141000 = 0
    RAW_BCD_OP_TEST( "25.000"        , ".141000"       , rawSubtraction    , "24.859"         ); // 25.000-.141000 = 24.859
    RAW_BCD_OP_TEST( "025.000"       , ".141000"       , rawSubtraction    , "24.859"         ); // 025.000-.141000 = 24.859
    //RAW_BCD_OP_TEST( ""      , "" , rawSubtraction, "" ); // 

    cout << endl;

    RAW_BCD_OP_TEST( "31.4159"       , "94500"         , rawMultiplication , "2968802.55"       ); // 31.4159*94500 = 2968802.55
    RAW_BCD_OP_TEST( "314.15"        , "3.141592654"   , rawMultiplication , "986.9313322541"   ); // 314.15 * 3.141592654 = 986.9313322541
    RAW_BCD_OP_TEST( "31.4159"       , "3.141592654"   , rawMultiplication , "98.6959606587986" ); // 31.4159*3.141592654 = 98.6959606587986
    RAW_BCD_OP_TEST( "300"           , "3.141592654"   , rawMultiplication , "942.4777962"      ); // 300*3.141592654 = 942.4777962
    RAW_BCD_OP_TEST( "3000"          , "3.141592654"   , rawMultiplication , "9424.777962"      ); // 3000*3.141592654 = 9424.777962
    RAW_BCD_OP_TEST( "3.141592654"   , "0.003"         , rawMultiplication , "0.009424777962"   ); // 3.141592654*0.003 = 0.009424777962
    RAW_BCD_OP_TEST( ".141"          , ".141000"       , rawMultiplication , "0.019881"         ); // .141*.141000 = 0.019881
    RAW_BCD_OP_TEST( "25.000"        , ".141000"       , rawMultiplication , "3.525"            ); // 25.000*.141000 = 3.525
    RAW_BCD_OP_TEST( "025.000"       , ".141000"       , rawMultiplication , "3.525"            ); // 025.000*.141000 = 3.525

    cout << endl;

    RAW_BCD_OP_TEST( "0.94500"       , "0.500"         , rawDivision , "1.89"                   ); // 0.94500/0.500 = 1.89
    RAW_BCD_OP_TEST( "9450.005"      , "0.005"         , rawDivision , "1890001"                ); // 9450.005/0.005 = 1890001
    RAW_BCD_OP_TEST( "94500"         , "500"           , rawDivision , "189"                    ); // 94500/500 = 189
    RAW_BCD_OP_TEST( "94500"         , "50"            , rawDivision , "1890"                   ); // 94500/50 = 1890
    RAW_BCD_OP_TEST( "94500"         , "5"             , rawDivision , "18900"                  ); // 94500/5 = 18900
    RAW_BCD_OP_TEST( "94500"         , "0.5"           , rawDivision , "189000"                 ); // 94500/0.5 = 189000
    RAW_BCD_OP_TEST( "94500"         , "0.05"          , rawDivision , "1890000"                ); // 94500/0.05 = 1890000
    RAW_BCD_OP_TEST( "94500"         , "0.005"         , rawDivision , "18900000"               ); // 94500/0.005 = 18900000
                                                                     // 3008.030965211883154708 28465840545710
    RAW_BCD_OP_TEST( "94500"         , "31.4159"       , rawDivision , "3008.030965211883154708" ); // 94500/31.4159 = 3008.0309652118 8315470828 4658405457 1092981579 3913273215 1553831021
                                                                     // 0.000000529152915291 005291005291005291005291005291005291005291
    RAW_BCD_OP_TEST( "500"           , "94500"         , rawDivision , "0.005291005291005291" ); // 500/94500 = 0.0052910052 9100529100 5291005291 0052910052 9100529100 5291005291

    RAW_BCD_OP_TEST( "31.4159"       , "94500"         , rawDivision ,    "0.000332443386243386"    ); // 31.4159/94500 = 0.0003324433 8624338624 3386243386 243386243386243386243386243386243386243386243386243386243386243386243386243386243386243386243386243386243386
    RAW_BCD_OP_TEST( "314.15"        , "3.141592654"   , rawDivision ,   "99.9970507315809378003"   ); // 314.15 / 3.141592654 = 99.9970507315 8093780034 6664548827 9143397818 75489450390088669974334616584572647781598740649461679063373567463148263396722342857884719258068395
    RAW_BCD_OP_TEST( "31.4159"       , "3.141592654"   , rawDivision ,    "9.999991552055621785"    ); // 31.4159/3.141592654 = 9.9999915520 5562178526 7899980262 6862139333 23005535650198907041370997552631786870736679536429804753420460474504279891914975174244852942032631
    RAW_BCD_OP_TEST( "300"           , "3.141592654"   , rawDivision ,   "95.492965842668411077"    ); // 300/3.141592654 = 95.4929658426 6841107784 1793298259 985045152196870396680014645845298058174030858935157161212282361034576063151184080913629485460338741930353393
    RAW_BCD_OP_TEST( "3000"          , "3.141592654"   , rawDivision ,  "954.929658426684110778"    ); // 3000/3.141592654 = 954.9296584266 8411077841 7932982599 850451521968703966800146458452980581740308589351571612122823610345760631511840809136294854603387419303533933
    RAW_BCD_OP_TEST( "3.141592654"   , "0.003"         , rawDivision , "1047.197551333333333333"    ); // 3.141592654/0.003 = 1047.1975513333 3333333333 3333333333 333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
    RAW_BCD_OP_TEST( ".141"          , ".141000"       , rawDivision ,    "1"                       ); // .141/.141000 = 1
    RAW_BCD_OP_TEST( "25.000"        , ".141000"       , rawDivision ,  "177.304964539007092198"    ); // 25.000/.141000 = 177.3049645390 0709219858 1560283687 943262411347517730496453900709219858156028368794326241134751773049645390070921985815602836879432624113475177
    RAW_BCD_OP_TEST( "025.000"       , ".141000"       , rawDivision ,  "177.304964539007092198"    ); // 025.000/.141000 = -//-
    RAW_BCD_OP_TEST( "1020314.159", "0.0000002718281828459045235", rawDivision, "3753526026322.301472254944911079" ); // 1020314.159/0.0000002718281828459045235 = 3753526026322.3014722549 4491107928 005226011384158046314264105198045620005998298872351439449250954737806651337555563555122563988343495323608632
    

    cout << endl;

    cout << "Failed " << totalRawBcdOpTestsFailed << " BCD operation tests from total " << totalRawBcdOpTests << endl;
    if (!totalRawBcdOpTestsFailed)
        cout << "+++ All BCD operation tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;




    unsigned totalCtorTests       = 0;
    unsigned totalCtorTestsFailed = 0;


    #define DECIMAL_CTOR_TEST( val, strResForCompare )                                      \
                do                                                                          \
                {                                                                           \
                    ++totalCtorTests;                                                       \
                    Decimal     decimal    = Decimal(val);                                  \
                    std::string strDecimal = decimal.toString( /* precision - auto */ );    \
                                                                                            \
                    bool bGood /* Johny */ = decimal.checkIsExact(strResForCompare);        \
                    if (!bGood)                                                             \
                       ++totalCtorTestsFailed;                                              \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << (decimal<0 ? "" : " ") << val << " converted to Decimal is '" << strDecimal << "'"; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected '" << strResForCompare << "')";                 \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)

    #define DECIMAL_CTOR_TEST_WITH_PREC( val, prec, strResForCompare )                      \
                do                                                                          \
                {                                                                           \
                    ++totalCtorTests;                                                       \
                    Decimal     decimal    = Decimal(val, marty::DecimalPrecision(prec));   \
                    std::string strDecimal = decimal.toString( /* precision - auto */ );    \
                                                                                            \
                    bool bGood /* Johny */ = decimal.checkIsExact(strResForCompare);        \
                    if (!bGood)                                                             \
                       ++totalCtorTestsFailed;                                              \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << (decimal<0 ? "" : " ") << val << " converted to Decimal with precision " << prec << " is '" << strDecimal << "'"; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected '" << strResForCompare << "')";                 \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)


    DECIMAL_CTOR_TEST( (std::uint64_t)3    ,       "3" );
    DECIMAL_CTOR_TEST( (std::int64_t )7    ,       "7" );
    DECIMAL_CTOR_TEST( (std::int64_t )-10  ,     "-10" );
    DECIMAL_CTOR_TEST( "123.01"            ,     "123.01" );
    DECIMAL_CTOR_TEST( "13.101"            ,     "13.101" );
    DECIMAL_CTOR_TEST( "3.1415"            ,     "3.1415" );
    DECIMAL_CTOR_TEST( 3.1415              ,     "3.1415" );
    DECIMAL_CTOR_TEST( -3.1415             ,    "-3.1415" );
    DECIMAL_CTOR_TEST( -100.10101          ,  "-100.10101" );
    //DECIMAL_CTOR_TEST(  3.1415926535897932  ,  "3.1415926535897932" );
    DECIMAL_CTOR_TEST(  3.1415926          ,  "3.141593" );
    // DECIMAL_CTOR_TEST();

    //Decimal decimal01    = Decimal(((std::uint64_t)3141592654ULL), marty::DecimalPrecision(9));

    // 3.141592654
    //                                             .
    DECIMAL_CTOR_TEST_WITH_PREC(  ((std::uint64_t)3141592654ULL), 9, "3.141592654" );


    cout << endl;

    cout << "Failed " << totalCtorTestsFailed << " CTOR tests from total " << totalCtorTests << endl;
    if (!totalCtorTestsFailed)
        cout << "+++ All CTOR tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;


    #if 0

    #define DECIMAL_OP_TEST( val1, op, val2, strResForCompare )                             \
                do                                                                          \
                {                                                                           \
                    ++totalOpTests;                                                         \
                    Decimal     d1       = Decimal(val1);                                   \
                    Decimal     d2       = Decimal(val2);                                   \
                    Decimal     dRes     = d1 op d2;                                        \
                    std::string strRes   = dRes.toString( /* precision - auto */ );         \
                                                                                            \
                    bool bGood /* Johny */ = dRes.checkIsExact(strResForCompare);           \
                    if (!bGood)                                                             \
                       ++totalOpTestsFailed;                                                \
                                                                                            \
                                                                                            \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << d1 << " " << #op << " " << d2 << " = " << strRes << ""; \
                    if (!bGood)                                                             \
                    {                                                                       \
                        cout << " (expected '" << strResForCompare << "')";                 \
                    }                                                                       \
                    cout << endl;                                                           \
                                                                                            \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)


    unsigned totalOpTests       = 0;
    unsigned totalOpTestsFailed = 0;

    DECIMAL_OP_TEST( 3, + , 4.12, "7.12" );
    DECIMAL_OP_TEST( 35.64745, + , 89.253464, "124.900914" );
    DECIMAL_OP_TEST( 3, - , 4.12, "-1.12" );
    DECIMAL_OP_TEST( 35.64745, - , 89.253464, "-53.606014" );
    DECIMAL_OP_TEST( 4.12, - , 3, "1.12" );
    DECIMAL_OP_TEST( 89.253464, - , 35.64745, "53.606014" );
    //DECIMAL_OP_TEST( 4.12, * , 3.457, "14.24284" );
    DECIMAL_OP_TEST( 89.253464, * , 35.64745, "3181.659678575" );
                                            // 3181.6583952668
                                            // 3181.653790004
    // 89.253464 * 35.64745 = 3181.653790004 (expected '3181.659678575')
    // 89.25346 * 35.6474 = 3181.653790004 (expected '3181.659678575')

    cout << endl;

    cout << "Failed " << totalOpTestsFailed << " OP tests from total " << totalOpTests << endl;
    if (!totalOpTestsFailed)
        cout << "+++ All OP tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;


    /*
    #define PRINT( var ) cout << #var << ": "<< var << endl;

    Decimal d1_u64_3   = (std::uint64_t)(3)  ;     PRINT(d1_u64_3);
    Decimal d1_i64_7   = (std::int64_t )(7)  ;     PRINT(d1_i64_7);
    Decimal d1_i64_m10 = (std::int64_t )(-10);     PRINT(d1_i64_m10);
    Decimal d1_i64_15_000 = Decimal(15, DecimalPrecision(3)); PRINT(d1_i64_15_000);
    Decimal d1 = Decimal::fromString("123.01");    PRINT(d1);
    Decimal d2 = Decimal::fromString("13.101");    PRINT(d2);
    Decimal d1_2_sum = d1 + d2;                    PRINT(d1_2_sum);  // 123.01 + 13.101 =  136.111
    Decimal d1_2_raz = d1 - d2;                    PRINT(d1_2_raz);  // 123.01 - 13.101 =  109.909
    Decimal d1_2_mul = d1 * d2;                    PRINT(d1_2_mul);  // 123.01 * 13.101 = 1611.55401
    Decimal d1_2_div = d1 / d2;                    PRINT(d1_2_div);  // 123.01 / 13.101 =    9.389359590870925883520341958629112281505228608503167697122357071979238226089611480039691626593389817571177772689107701702160140447294099687
    Decimal d1_2_ost = d1 % d2;                    PRINT(d1_2_ost);  // 123.01 % 13.101 =    5.101

    auto    d1_2_sum_plus_15_a = d1_2_sum + (std::int64_t )(15); PRINT(d1_2_sum_plus_15_a); // 136.111 + 15 = 151.111
    auto    d1_2_sum_plus_15_b = d1_2_sum + (std::uint64_t)(15); PRINT(d1_2_sum_plus_15_b);
    auto    d1_2_sum_plus_15_c = d1_2_sum + (int          )(15); PRINT(d1_2_sum_plus_15_c);
    auto    d1_2_sum_plus_15_d = d1_2_sum + (unsigned     )(15); PRINT(d1_2_sum_plus_15_d);
    auto    d1_2_sum_plus_15_e = d1_2_sum + (double       )(15); PRINT(d1_2_sum_plus_15_e);


    Decimal divideD1   = Decimal( 100, DecimalPrecision(2) );
    Decimal divideD2   = Decimal(  25, DecimalPrecision(1) );
    Decimal divideRes  = divideD1.divide(divideD2, 6);
    
    cout << divideRes.toString() << endl;
    */




    unsigned totalRoundingTests       = 0;
    unsigned totalRoundingTestsFailed = 0;


    #define ROUNDING_TEST( dblVal, strResForCompare, roundingPrecision, roundingMethod ) \
                do                                                                       \
                {                                                                        \
                    ++totalRoundingTests;                                                \
                    Decimal decimal    = Decimal(dblVal);                                \
                    Decimal roundedVal = decimal.rounded( roundingPrecision, Decimal::RoundingMethod::roundingMethod ); \
                    /*std::ostringstream os;*/                                           \
                    std::string strRes = roundedVal.toString(roundingPrecision);         \
                                                                                         \
                    bool bGood /* Johny */ = decimal.checkIsExact(strResForCompare);        \
                    if (!bGood)                                                          \
                       ++totalRoundingTestsFailed;                                       \
                                                                                         \
                                                                                         \
                    cout << "[" << (bGood ? "+" : "-") << "]  " << (decimal<0 ? "" : " ") << decimal.toString(roundingPrecision+1) << " rounded to " << roundingPrecision << " signs with " << #roundingMethod << " rounding method is " << strRes; \
                    if (!bGood)                                                          \
                    {                                                                    \
                        cout << " (expected " << strResForCompare << ")";                \
                    }                                                                    \
                    cout << endl;                                                        \
                                                                                         \
                    if (!bGood)                                                             \
                        THROW_ERROR();                                                      \
                                                                                            \
                } while(0)


    // https://en.wikipedia.org/wiki/Rounding#Round_half_to_odd

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.01 ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.001,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.112,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.499,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.5  ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.51 ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.501,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.511,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.69 ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.699,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.7  ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.71 ,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.701,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.911,  "23", 0, roundFloor           );
    ROUNDING_TEST(   23.999,  "23", 0, roundFloor           );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundFloor           );
    ROUNDING_TEST(   24.01 ,  "24", 0, roundFloor           );
    ROUNDING_TEST(   24.001,  "24", 0, roundFloor           );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundFloor           );
    ROUNDING_TEST(  -23.01 , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.001, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.112, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.2  , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.21 , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.49 , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.499, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.5  , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.501, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.511, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.911, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -23.999, "-24", 0, roundFloor           );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundFloor           );
    ROUNDING_TEST(  -24.01 , "-25", 0, roundFloor           );
    ROUNDING_TEST(  -24.011, "-25", 0, roundFloor           );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundCeil            );
    ROUNDING_TEST(   23.01 ,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.001,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.2  ,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.49 ,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.499,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.5  ,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.501,  "24", 0, roundCeil            );
    ROUNDING_TEST(   23.999,  "24", 0, roundCeil            );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundCeil            );
    ROUNDING_TEST(   24.001,  "25", 0, roundCeil            );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.01 , "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.001, "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.499, "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.5  , "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.51 , "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.501, "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.7  , "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.701, "-23", 0, roundCeil            );
    ROUNDING_TEST(  -23.999, "-23", 0, roundCeil            );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundCeil            );
    ROUNDING_TEST(  -24.01 , "-24", 0, roundCeil            );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.01 ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.499,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.5  ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.51 ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.7  ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   23.99 ,  "23", 0, roundTrunc           );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundTrunc           );
    ROUNDING_TEST(   24.01 ,  "24", 0, roundTrunc           );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.01 , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.499, "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.5  , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.51 , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.7  , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -23.99 , "-23", 0, roundTrunc           );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundTrunc           );
    ROUNDING_TEST(  -24.01 , "-24", 0, roundTrunc           );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.01 ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.11 ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.2  ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.49 ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.499,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.5  ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.7  ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   23.99 ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundTowardsInf      );
    ROUNDING_TEST(   24.01 ,  "25", 0, roundTowardsInf      );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.01 , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.2  , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.49 , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.499, "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.5  , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.5  , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.501, "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.7  , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -23.99 , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundTowardsInf      );
    ROUNDING_TEST(  -24.01 , "-25", 0, roundTowardsInf      );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundHalfUp          );
    ROUNDING_TEST(   23.1  ,  "23", 0, roundHalfUp          );
    ROUNDING_TEST(   23.01 ,  "23", 0, roundHalfUp          );
    ROUNDING_TEST(   23.3  ,  "23", 0, roundHalfUp          );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundHalfUp          );
    ROUNDING_TEST(   23.499,  "23", 0, roundHalfUp          );
    ROUNDING_TEST(   23.5  ,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   23.501,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   23.7  ,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   23.9  ,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   23.99 ,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundHalfUp          );
    ROUNDING_TEST(   24.01 ,  "24", 0, roundHalfUp          );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.1  , "-23", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.499, "-23", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.5  , "-23", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.501, "-24", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.9  , "-24", 0, roundHalfUp          );
    ROUNDING_TEST(  -23.99 , "-24", 0, roundHalfUp          );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundHalfUp          );
    ROUNDING_TEST(  -24.01 , "-24", 0, roundHalfUp          );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundHalfDown        );
    ROUNDING_TEST(   23.1  ,  "23", 0, roundHalfDown        );
    ROUNDING_TEST(   23.3  ,  "23", 0, roundHalfDown        );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundHalfDown        );
    ROUNDING_TEST(   23.499,  "23", 0, roundHalfDown        );
    ROUNDING_TEST(   23.5  ,  "23", 0, roundHalfDown        );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundHalfDown        );
    ROUNDING_TEST(   23.501,  "24", 0, roundHalfDown        );
    ROUNDING_TEST(   23.9  ,  "24", 0, roundHalfDown        );
    ROUNDING_TEST(   23.99 ,  "24", 0, roundHalfDown        );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundHalfDown        );
    ROUNDING_TEST(   24.01 ,  "24", 0, roundHalfDown        );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.1  , "-23", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.3  , "-23", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.499, "-23", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.5  , "-24", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.501, "-24", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.9  , "-24", 0, roundHalfDown        );
    ROUNDING_TEST(  -23.99 , "-24", 0, roundHalfDown        );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundHalfDown        );
    ROUNDING_TEST(  -24.1  , "-24", 0, roundHalfDown        );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.1  ,  "23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.499,  "23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.5  ,  "23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.511,  "24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.9  ,  "24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   23.99 ,  "24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(   24.1  ,  "24", 0, roundHalfTowardsZero );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.1  , "-23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.499, "-23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.5  , "-23", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.501, "-24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.9  , "-24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -23.99 , "-24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundHalfTowardsZero );
    ROUNDING_TEST(  -24.1  , "-24", 0, roundHalfTowardsZero );

    // roundMath
    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.1  ,  "23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.499,  "23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.5  ,  "24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.501,  "24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.9  ,  "24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   23.99 ,  "24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   24.0  ,  "24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(   24.1  ,  "24", 0, roundHalfTowardsInf  );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.1  , "-23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.499, "-23", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.5  , "-24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.501, "-24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.9  , "-24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -23.99 , "-24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -24.0  , "-24", 0, roundHalfTowardsInf  );
    ROUNDING_TEST(  -24.1  , "-24", 0, roundHalfTowardsInf  );

    cout << endl;
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.0  ,  "23", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.1  ,  "23", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.49 ,  "23", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.499,  "23", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.5  ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.51 ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.501,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.9  ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   23.99 ,  "24", 0, roundHalfToEven      );

    cout << endl;
    ROUNDING_TEST(   24.0  ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.1  ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.49 ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.499,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.5  ,  "24", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.51 ,  "25", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.501,  "25", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.9  ,  "25", 0, roundHalfToEven      );
    ROUNDING_TEST(   24.99 ,  "25", 0, roundHalfToEven      );

    cout << endl;
    ROUNDING_TEST(  -23.0  , "-23", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.1  , "-23", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.49 , "-23", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.499, "-23", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.5  , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.51 , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.501, "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.9  , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -23.99 , "-24", 0, roundHalfToEven      );

    cout << endl;
    ROUNDING_TEST(  -24.0  , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.1  , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.49 , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.499, "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.5  , "-24", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.51 , "-25", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.501, "-25", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.9  , "-25", 0, roundHalfToEven      );
    ROUNDING_TEST(  -24.99 , "-25", 0, roundHalfToEven      );

    #endif

    /*
    cout << "------------------------------" << endl;
    ROUNDING_TEST(   23.5,  "23", 0, roundHalfToOdd       );
    ROUNDING_TEST(   22.5,  "23", 0, roundHalfToOdd       );
    ROUNDING_TEST(  -23.5, "-23", 0, roundHalfToOdd       );
    ROUNDING_TEST(  -22.5, "-23", 0, roundHalfToOdd       );
    */

    cout << endl;

    cout << "Failed " << totalRoundingTestsFailed << " rounding tests from total " << totalRoundingTests << endl;
    if (!totalRoundingTestsFailed)
        cout << "+++ All rounding tests passed"  << endl;

    cout << "------------------------------" << endl << endl << endl;

    return 0;
}



