/*! \file
    \brief Name (cpp.h) functions test

 */

#include <iostream>
#include <exception>
#include <stdexcept>
#include <map>
#include <set>
#include <cstring>

#include <QCoreApplication>
#include <QString>
#include <QSettings>
#include <QTest>
#include <QDir>
#include <QElapsedTimer>

#include "invest_openapi/invest_openapi.h"
#include "invest_openapi/utility.h"
#include "invest_openapi/model_to_strings.h"
#include "cpp/cpp.h"



INVEST_OPENAPI_MAIN()
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("test013");
    QCoreApplication::setApplicationVersion("1.0");

    QCoreApplication::setOrganizationName("al-martyn1");
    QCoreApplication::setOrganizationDomain("https://github.com/al-martyn1/");

    using std::cout;
    using std::endl;


    cout<<"Launched from : "<<QDir::currentPath().toStdString()<<endl;
    cout<<"Launched exe  : "<<QCoreApplication::applicationFilePath().toStdString()<<endl;
    cout<<"Path to exe   : "<<QCoreApplication::applicationDirPath().toStdString()<<endl;

    cout << endl;

    std::string q1 = "A B(1,2), C D E,";
    std::string q2 = "A B(1,2), C D E" ;

    QVector<QString> qs1 = invest_openapi::splitByCharWithBraces( q1, ',' );
    QVector<QString> qs2 = invest_openapi::splitByCharWithBraces( q2, ',' );

    QVector<QString> qn1 = invest_openapi::getColumnNamesFromTableSqlSchema( q1 );
    QVector<QString> qn2 = invest_openapi::getColumnNamesFromTableSqlSchema( q2 );


    cpp::NameStyleSet  /* std::set<cpp::NameStyle> */  allNameStyles = cpp::makeAllNameStyles();
    cpp::NameStyleSet  /* std::set<cpp::NameStyle> */  testedNameStyles;

    const char *testNames[] = 
    {   ""
    ,   "__"
    ,   "_TestName"
    ,   "_testName"
    ,   "_Test_Name"
    ,   "_test_Name"
    ,   "_TEST_NAME"
    ,   "_test_name"
    ,   "__TestName__"
    ,   "__testName__"
    ,   "__Test_Name__"
    ,   "__test_Name__"
    ,   "__TEST_NAME__"
    ,   "__test_name__"
    ,   "TEST_NAME"
    ,   "testName"
    ,   "TestName"
    ,   "Test_Name"
    ,   "test_Name"
    ,   "test_name"
    ,   0
    };

    std::size_t testNameIdx = 0;
    const char* testName    = testNames[testNameIdx];
    while(testName)
    {
        cpp::NameStyle nameStyle = cpp::detectNameStyle(testName);
        testedNameStyles.insert(nameStyle);
        
        if (!std::strlen(testName))
            cout << "\"\"            ";
        else
            cout << cpp::expandAtBack(testName, 14);
        cout << "  : " << cpp::toString(nameStyle) << endl;
        testName    = testNames[++testNameIdx];
    }

    std::set<cpp::NameStyle> notTestedNameStyles;
    for ( auto nameStyleIt = allNameStyles.begin(); nameStyleIt!=allNameStyles.end(); ++nameStyleIt)
    {
        if (testedNameStyles.find(*nameStyleIt)==testedNameStyles.end())
            notTestedNameStyles.insert(*nameStyleIt);
    }

    if (!notTestedNameStyles.empty())
    {

        cout << endl << "No tested name styles:" << endl;
        //std::set<cpp::NameStyle> 
        auto notTestedNameStylesIt = notTestedNameStyles.begin();
        for( ; notTestedNameStylesIt != notTestedNameStyles.end(); ++notTestedNameStylesIt )
        {
            cout << "  " << cpp::toString(*notTestedNameStylesIt) << endl;
        }
    }


    cout << endl;

    allNameStyles.erase(cpp::NameStyle::unknownStyle);
    allNameStyles.erase(cpp::NameStyle::invalidName);

    testNameIdx = 0;
    testName    = testNames[testNameIdx];
    while(testName)
    {
        //cpp::NameStyle nameStyle = cpp::detectNameStyle(testName);
        //testedNameStyles.insert(nameStyle);

        if (!std::strlen(testName) || std::strcmp(testName,"__")==0)
        {
            testName    = testNames[++testNameIdx];
            continue;
        }

        cout<< endl << "Format        : \"" << testName << "\":" << endl;
        cout<<         "No underscores: \"" << cpp::trimUnderscores(testName) << "\":" << endl;

        for ( auto nameStyleIt = allNameStyles.begin(); nameStyleIt!=allNameStyles.end(); ++nameStyleIt)
        {
            auto formattedName = cpp::formatName(testName, *nameStyleIt);
            cout << "    as " << cpp::expandAtBack( cpp::toString(*nameStyleIt), 30) << " : \"" << formattedName << "\"" << endl;
        }
        
        testName    = testNames[++testNameIdx];
    }





    
    return 0;
}



