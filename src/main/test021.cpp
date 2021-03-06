/*! \file
    \brief Configs lookup test

 */

#include <iostream>
#include <exception>
#include <stdexcept>

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

#include "invest_openapi/invest_openapi.h"
#include "invest_openapi/factory.h"
#include "invest_openapi/openapi_completable_future.h"

#include "invest_openapi/ioa_utils.h"
#include "invest_openapi/ioa_ostream.h"



INVEST_OPENAPI_MAIN()
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("test021");
    QCoreApplication::setApplicationVersion("1.0");

    QCoreApplication::setOrganizationName("al-martyn1");
    QCoreApplication::setOrganizationDomain("https://github.com/al-martyn1/");

    using std::cout;
    using std::endl;

    cout<<"Path to exe   : "<<QCoreApplication::applicationDirPath().toStdString()<<endl;

    cout << endl;

    namespace tkf=invest_openapi;
    using tkf::config_helpers::lookupForConfigFile;
    using tkf::config_helpers::FileReadable;

    QStringList lookupConfSubfolders = QString("conf;config").split( ';', Qt::SkipEmptyParts );


    auto logConfigFullFileName   = lookupForConfigFile( "logging.properties" , lookupConfSubfolders, FileReadable(), QCoreApplication::applicationDirPath(), true, -1 );
    auto apiConfigFullFileName   = lookupForConfigFile( "config.properties"  , lookupConfSubfolders, FileReadable(), QCoreApplication::applicationDirPath(), true, -1 );
    auto authConfigFullFileName  = lookupForConfigFile( "auth.properties"    , lookupConfSubfolders, FileReadable(), QCoreApplication::applicationDirPath(), true, -1 );

    qDebug().nospace().noquote() << "Log  Config File: "<< logConfigFullFileName  ;
    qDebug().nospace().noquote() << "API  Config File: "<< apiConfigFullFileName  ;
    qDebug().nospace().noquote() << "Auth Config File: "<< authConfigFullFileName ;

    auto apiConfig     = tkf::ApiConfig    ( apiConfigFullFileName  );
    auto authConfig    = tkf::AuthConfig   ( authConfigFullFileName );
    auto loggingConfig = tkf::LoggingConfig( logConfigFullFileName  );

    QSharedPointer<tkf::IOpenApi> pOpenApi = tkf::createOpenApi( apiConfig, authConfig, loggingConfig );

    tkf::ISanboxOpenApi* pSandboxOpenApi = tkf::getSandboxApi(pOpenApi);

    if (pSandboxOpenApi)
    {
        pSandboxOpenApi->setBrokerAccountId( authConfig.getBrokerAccountId() );
    }
    else
    {
        pOpenApi->setBrokerAccountId( authConfig.getBrokerAccountId() );
    }


    QDateTime curTime   = QDateTime::currentDateTime();
    QDateTime startDate = curTime.addYears(-1); //curTime.date().addYears(-1);

    // BBG004731354 - ROSN
    // BBG00Y91R9T3 - OZON
    auto // OperationsResponse
    operationsRes = pOpenApi->operations( startDate, curTime, "BBG004731354" ); // Если не задать фигу, то ошибки нет, просто пустой список

    operationsRes->join();
    tkf::checkAbort(operationsRes);

    auto // std::vector< tkf::Operation >
    operations = tkf::makeOrderedVectorFromList( operationsRes->value.getPayload().getOperations()
                                               //, tkf::WithGetDateGreater<tkf::Operation>()
                                               , tkf::WithGetDateLess<tkf::Operation>()
                                               );

    marty::Decimal totalBalance = 0;

    for( const auto &op : operations )
    {
        if (!op.isSet() || !op.isValid())
            continue;

        QString operationTypeStr   = op.getOperationType().asJson().toUpper();
        QString operationStatusStr = op.getStatus().asJson().toUpper();

        #if 0
            if ( (operationTypeStr=="SELL" || operationTypeStr=="BUY") && operationStatusStr=="DONE")
            {
                cout << "Operation Type    : " << op.getOperationType()    << endl;
                cout << "Payment           : " << op.getPayment()          << endl;
            }
        #else

            cout << op;
            cout << "------------------------------------------------" << endl;

        #endif

        // Operation Type    : Sell/Buy
        // Operation Status  : Done

        if ( (operationTypeStr=="SELL" || operationTypeStr=="BUY") && operationStatusStr=="DONE")
        {
            totalBalance += op.getPayment();
        }

    }

    cout << endl << "--------------------" << endl;
    cout << "Total balance: " << totalBalance << endl;
    
    #if 0
    auto // PortfolioResponse
    portolioRes = pOpenApi->portfolio();

    portolioRes->join();
    tkf::checkAbort(portolioRes);

    auto portfolio = portolioRes->value.getPayload();
    auto /* QList<PortfolioPosition> */ portfolioPositions = portfolio.getPositions();
    #endif



    // TKF_IOA_ABSTRACT_METHOD( OperationsResponse , operations ( const QDateTime &from, const QDateTime &to, const QString &figi, QString broker_account_id = QString()) );
    return 0;
}



