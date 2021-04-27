/*! \file
    \brief Нахождение 1/N

 */

#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <optional>

#include <QCoreApplication>
#include <QString>
#include <QSettings>
#include <QTest>
#include <QDir>
#include <QElapsedTimer>

#include "invest_openapi/config_helpers.h"
#include "invest_openapi/api_config.h"
#include "invest_openapi/auth_config.h"
#include "invest_openapi/currencies_config.h"
#include "invest_openapi/balance_config.h"

#include "invest_openapi/invest_openapi.h"
#include "invest_openapi/factory.h"
#include "invest_openapi/openapi_completable_future.h"

#include "invest_openapi/database_config.h"
#include "invest_openapi/database_manager.h"
#include "invest_openapi/qt_time_helpers.h"

#include "invest_openapi/db_utils.h"
#include "invest_openapi/ioa_utils.h"
#include "invest_openapi/ioa_ostream.h"
#include "invest_openapi/ioa_db_dictionaries.h"

#include "invest_openapi/console_break_helper.h"

#include "invest_openapi/marty_decimal.h"


#include "cpp/cpp.h"
// std::string  cpp::expandAtFront( const char    * pStr, std::size_t expandToSize, char    ch =  ' ' )


//----------------------------------------------------------------------------
// int - удобнее
inline
int getFirstDiffPos( const std::string &v1, const std::string &v2 )
{
    // std::string::size_type i = 0, sz = std::min( v1.size(), v2.size() );
    std::string::size_type i = 0;
    // std::string::size_type sz = std::min( v1.size(), v2.size() );
    std::string::size_type sz = v1.size() < v2.size() ? v1.size() : v2.size();

    for(; i!=sz; ++i )
    {
        if (v1[i]!=v2[i])
           return i;
    }

    return -1;
}

//----------------------------------------------------------------------------
inline
std::string makeDiffMarkerString( int strDiffRes, char marker = '|' )
{
    if (strDiffRes<0)
       return std::string();

    if (marker==0)
        return cpp::expandAtFront( std::string(), strDiffRes );

    return cpp::expandAtFront( std::string(), strDiffRes ) + std::string(1, marker);
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
inline
void printDiffPair( const std::string &v1, const std::string &v2, std::string indend, bool printExMarker = true )
{
    using std::cout;
    using std::endl;

    auto mrkPos  = getFirstDiffPos( v1, v2 );
    auto mrkLine = makeDiffMarkerString(mrkPos);

    std::string mrkPosStr;

    std::string mrkIndend = indend;

    if (printExMarker)
    {
        if (mrkPos<0)
        {
            mrkIndend += "+";
        }
        else
        {
            mrkIndend += "!";
            mrkPosStr += " - ";
            mrkPosStr += std::to_string(mrkPos);
        }

        mrkIndend += " ";

        indend    += "  ";
    }

    cout << indend    << v1 << endl;
    cout << mrkIndend << mrkLine << mrkPosStr << endl;
    cout << indend    << v2 << endl;

}

//----------------------------------------------------------------------------
inline
void printDiffPair( const marty::Decimal &v1, const marty::Decimal &v2, std::string indend, bool printExMarker = true )
{
    printDiffPair( v1.toString(), v2.toString(), indend, printExMarker );
}

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------

INVEST_OPENAPI_MAIN()
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("test034");
    QCoreApplication::setApplicationVersion("1.0");

    QCoreApplication::setOrganizationName("al-martyn1");
    QCoreApplication::setOrganizationDomain("https://github.com/al-martyn1/");

    using std::cout;
    using std::endl;

    auto fmtWidth = cout.width();


    const std::string testValStr    = "317860096764605239302343.916106517608230092939884907517544912278778303586057323950081374211073655936951665940766";
    const std::string testValStrBad = "317860016764605239302343.916106517608230092939884907517544912278778303586057323950081374211073655936951665940766";

    marty::Decimal dGood = testValStr;
    marty::Decimal dBad  = testValStrBad;


    // Simple shows reciprocates
    #if 0

    for( auto i=1; i<=100; ++i )
    {
        auto reciprocated = marty::Decimal(i).reciprocated();
        cout << std::setw(3) << i << std::setw(fmtWidth) 
             << "  -   1/" 
             << std::setw(3) << i << std::setw(fmtWidth)
             << " - reciprocated: " << reciprocated 
             << endl;
    }
    cout << endl << "------------------------------" << endl << endl;

    #endif


    // Difference test
    #if 0

    cout << "String difference marker test 1" << endl << endl;
    auto mrkPos = getFirstDiffPos( testValStr, testValStrBad );
    cout << testValStr    << endl;
    cout << makeDiffMarkerString(mrkPos) << endl;
    cout << testValStrBad << endl;
    cout << endl;
    cout << endl;


    cout << "String difference marker test 2" << endl << endl;
    printDiffPair( testValStr, testValStrBad, "  " );
    cout << endl;
    cout << endl;

    #endif

    cout << "String difference marker test 3" << endl << endl;
    printDiffPair( dGood, dBad, "  " );
    cout << endl;
    cout << endl;

    cout << "String difference marker test 4" << endl << endl;
    printDiffPair( dGood, dGood, "  " );
    cout << endl;
    cout << endl;



    // makeDiffMarkerString( int strDiffRes, char marker = 0 )
    // getFirstDiffPos( const std::string &v1, const std::string &v2 )

/*

    Decimal& add( const Decimal &d );
    Decimal& sub( const Decimal &d );
    Decimal& mul( const Decimal &d );
    Decimal& div( const Decimal &d, int precision = MARTY_DECIMAL_DEFAULT_DIVISION_PRECISION );

    std::string toString( int precision = -1 ) const;

*/
    
    return 0;
}



