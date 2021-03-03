/*! \file
    \brief Database creation exsample

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

#include "invest_openapi/database_config.h"
#include "invest_openapi/database_manager.h"
#include "invest_openapi/qt_time_helpers.h"

#include "cpp/cpp.h"


INVEST_OPENAPI_MAIN()
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("_tkf_create_db");
    QCoreApplication::setApplicationVersion("1.0");

    QCoreApplication::setOrganizationName("al-martyn1");
    QCoreApplication::setOrganizationDomain("https://github.com/al-martyn1/");

    using std::cout;
    using std::endl;


    qDebug().nospace().noquote() << "Path to exe   : "<<QCoreApplication::applicationDirPath() /* .toStdString() */; // <<endl;
    //cout << endl;

    namespace tkf=invest_openapi;
    using tkf::config_helpers::lookupForConfigFile;
    using tkf::config_helpers::FileReadable;

    QString lookupConfSubfolders = "conf;config";

    auto dbConfigFullName  = lookupForConfigFile( "database.properties", lookupConfSubfolders, FileReadable() );
    auto logConfigFullName = lookupForConfigFile( "logging.properties" , lookupConfSubfolders, FileReadable() );

    qDebug().nospace().noquote() << "DB  Config : "<< dbConfigFullName ;
    qDebug().nospace().noquote() << "Log Config : "<< logConfigFullName ;


    QSharedPointer<tkf::DatabaseConfig> pDatabaseConfig = QSharedPointer<tkf::DatabaseConfig>( new tkf::DatabaseConfig(dbConfigFullName, tkf::DatabasePlacementStrategyDefault()) );
    QSharedPointer<tkf::LoggingConfig>  pLoggingConfig  = QSharedPointer<tkf::LoggingConfig> ( new tkf::LoggingConfig(logConfigFullName) );

    qDebug().nospace().noquote() << "DB name: " << pDatabaseConfig->dbFilename;

    QSharedPointer<QSqlDatabase> pSqlDb = QSharedPointer<QSqlDatabase>( new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")) );
    pSqlDb->setDatabaseName( pDatabaseConfig->dbFilename );

    if (!pSqlDb->open())
    {
      qDebug() << pSqlDb->lastError().text();
      return 0;
    }

    QSharedPointer<tkf::IDatabaseManager> pDbMan = tkf::createDatabaseManager( pSqlDb, pDatabaseConfig, pLoggingConfig );

    //pDbMan->setDefaultDecimalFormat(18,8);
    pDbMan->applyDefDecimalFormatFromConfig( *pDatabaseConfig );


    QElapsedTimer timer;
    timer.start();

    //auto sandboxRegisterPostInterval = timer.restart();

    int curLevel = 0;

    QSet<QString> tablesForCreation = pDbMan->tableGetTableNamesForCreation( curLevel );
    for(; !tablesForCreation.empty(); ++curLevel, tablesForCreation = pDbMan->tableGetTableNamesForCreation( curLevel ) )
    {
        qDebug().nospace().noquote() << "Level "<<curLevel;
        //cout<<endl<<
        //<<endl<<endl;
        
        for( auto tableName : tablesForCreation )
        {
            //cout<<"    "<<tableName.toStdString()<<endl;

            auto expandStrForTableName = cpp::makeExpandString( tableName.toStdString(), 20 );
            auto qexp = QString::fromStdString(expandStrForTableName);

            //qDebug().nospace().noquote() << "Drop   table '" << tableName << "'" << qexp << ": " << pDbMan->tableDrop(tableName);
            bool createRes = pDbMan->tableCreate(tableName, tkf::IDatabaseManager::IfExists::ifNotExists );
            auto elapsed = timer.restart();
            qDebug().nospace().noquote() << "Create table '" << tableName << "'" << qexp << ": " << createRes<<", tie elapsed: " << elapsed;
            //qDebug().nospace().noquote() <<"\n";
            //qDebug().nospace().noquote() <<"";
        }
    }

    qDebug().nospace().noquote() << "\n";

    
    if (pDatabaseConfig->createClean)
    {
        qDebug().nospace().noquote() << "Clean database created. Exiting due config option";
        return 0;
    }


    timer.restart();

    // Fill ref books here

    qDebug().nospace().noquote() << "Fill 'BROKER_ACCOUNT_TYPE' table: " 
                                 << pDbMan->insertToBulkFromString( "BROKER_ACCOUNT_TYPE"
                                                                  , "0,INVALID,Invalid BrokerAccountType value;"
                                                                    "1,TINKOFF,Tinkoff broker account;"
                                                                    "2,TINKOFFIIS,Tinkoff IIS account"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'CURRENCY' table: " 
                                 << pDbMan->insertToBulkFromString( "CURRENCY"
                                                                  , "0,INVALID,Invalid Currency value;"
                                                                    "1,RUB,Russian Ruble;"
                                                                    "2,USD:US Dollar;"
                                                                    "3,EUR:European Euro;"
                                                                    "4,GBP:Great Britain Pound Sterling;"
                                                                    "5,HKD:Hong Kong Dollar;"
                                                                    "6,CHF:Swiss Franc;"
                                                                    "7,JPY:Japanese Yen;"
                                                                    "8,CNY:Chinese Yuan;"
                                                                    "9,TRY:Turkish Lira"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'INSTRUMENT_TYPE' table: " 
                                 << pDbMan->insertToBulkFromString( "INSTRUMENT_TYPE"
                                                                  , "0,INVALID,Invalid InstrumentType value;"
                                                                    "1,STOCK,Stocks;"
                                                                    "2,CURRENCY,Currencies;"
                                                                    "3,BOND,Bonds;"
                                                                    "4,ETF,Etfs"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'CANDLE_RESOLUTION' table: " 
                                 << pDbMan->insertToBulkFromString( "CANDLE_RESOLUTION"
                                                                  , "0,INVALID,INVALID,INVALID,Invalid CandleResolution value;"
                                                                    "1,1MIN,1MIN,1DAY,1 min;"                    /* 1min [1 minute, 1 day]    */
                                                                    "2,2MIN,2MIN,1DAY,2 min;"                    /* 2min [2 minutes, 1 day]   */
                                                                    "3,3MIN,3MIN,1DAY,3 min;"                    /* 3min [3 minutes, 1 day]   */
                                                                    "4,5MIN,5MIN,1DAY,5 min;"                    /* 5min [5 minutes, 1 day]   */
                                                                    "5,10MIN,10MIN,1DAY,10 min;"                 /* 10min [10 minutes, 1 day] */
                                                                    "6,15MIN,15MIN,1DAY,15 min;"                 /* 15min [15 minutes, 1 day] */
                                                                    "7,30MIN,30MIN,1DAY,30 min;"                 /* 30min [30 minutes, 1 day] */
                                                                    "8,HOUR,1HOUR,7DAY,Hour (60 min);"            /* hour [1 hour, 7 days]     */
                                                                    "9,DAY,1DAY,1YEAR,Day (1440 min);"            /* day [1 day, 1 year]       */
                                                                    "10,WEEK,7DAY,2YEAR,Week (10080 min);"        /* week [7 days, 2 years]    */
                                                                    "11,MONTH,1MONTH,10YEAR,Month (Avg 43200 min)"  /* month [1 month, 10 years] */
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'OPERATION_TYPE' table: " 
                                 << pDbMan->insertToBulkFromString( "OPERATION_TYPE"
                                                                  , "0,INVALID,Invalid OperationType value;"
                                                                    "1,BUY,Purchaise;"
                                                                    "2,SELL,Sell"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'ORDER_STATUS' table: " 
                                 << pDbMan->insertToBulkFromString( "ORDER_STATUS"
                                                                  , "0,INVALID,Invalid OrderStatus value;"
                                                                    "1,NEW,New order;"
                                                                    "2,PARTIALLYFILL,Partially fill;"
                                                                    "3,FILL,Fill;"
                                                                    "4,CANCELLED,Cancelled;"
                                                                    "5,REPLACED,Replaced;"
                                                                    "6,PENDINGCANCEL,Pending cancel;"
                                                                    "7,REJECTED,Rejected;"
                                                                    "8,PENDINGREPLACE,Pending replace;"
                                                                    "9,PENDINGNEW,Pending new"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'ORDER_TYPE' table: " 
                                 << pDbMan->insertToBulkFromString( "ORDER_TYPE"
                                                                  , "0,INVALID,Invalid OrderType value;"
                                                                    "1,LIMIT,Limit;"
                                                                    "2,MARKET,Market"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'OPERATION_STATUS' table: " 
                                 << pDbMan->insertToBulkFromString( "OPERATION_STATUS"
                                                                  , "0,INVALID,Invalid OperationStatus value;"
                                                                    "1,DONE,Operation completed successfully;"
                                                                    "2,DECLINE,Operation declined;"
                                                                    "3,PROGRESS,Operation in progress"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'OPERATION_TYPE_WITH_COMMISSION' table: " 
                                 << pDbMan->insertToBulkFromString( "OPERATION_TYPE_WITH_COMMISSION"
                                                                  , "0,INVALID,Invalid OperationTypeWithCommission value;"
                                                                    "1,BUY,Buy;"
                                                                    "2,BUYCARD,Buycard;"
                                                                    "3,SELL,Sell;"
                                                                    "4,BROKERCOMMISSION,Broker commission;"
                                                                    "5,EXCHANGECOMMISSION,Exchange commission;"
                                                                    "6,SERVICECOMMISSION,Service commission;"
                                                                    "7,MARGINCOMMISSION,Margin commission;"
                                                                    "8,OTHERCOMMISSION,Other commission;"
                                                                    "9,PAYIN,Payin;"
                                                                    "10,PAYOUT,Payout;"
                                                                    "11,TAX,Tax;"
                                                                    "12,TAXLUCRE,Tax lucre;"
                                                                    "13,TAXDIVIDEND,Tax dividend;"
                                                                    "14,TAXCOUPON,Tax coupon;"
                                                                    "15,TAXBACK,Tax back;"
                                                                    "16,REPAYMENT,Repayment;"
                                                                    "17,PARTREPAYMENT,Part repayment;"
                                                                    "18,COUPON,Coupon;"
                                                                    "19,DIVIDEND,Dividend;"
                                                                    "20,SECURITYIN,Security in;"
                                                                    "21,SECURITYOUT,Securityout"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    QVector<QString> tzColumns     = pDbMan->tableGetColumnsFromSchema("TIMEZONE");
    QVector<QString> tzColumnsNoId = tkf::removeFirstItems(tzColumns, 1);
    
    std::vector<std::string> tzWlknList = qt_helpers::getTimezonesAliasList<std::string>();

    for( const auto &tzWlkn : tzWlknList )
    {
        // Pair - Name:Description

        std::string name = tzWlkn;
        std::string description = qt_helpers::getTimezoneAliasDesciption(name).toStdString();

        QVector<QString> vals;
        vals.push_back( QString::fromStdString(name) );
        vals.push_back( QString::fromStdString(description) );

        pDbMan->insertTo( "TIMEZONE", vals, tzColumnsNoId );
    }

    QList<QByteArray> allTzIdList = QTimeZone::availableTimeZoneIds();
    for( auto tzId : allTzIdList )
    {
        std::string strTzId = tzId.toStdString();
        QTimeZone qtz = QTimeZone(tzId);
        std::string descr = qtz.comment().toStdString();

        QVector<QString> vals;
        vals.push_back( QString::fromStdString(strTzId) );
        vals.push_back( QString::fromStdString(descr) );

        pDbMan->insertTo( "TIMEZONE", vals, tzColumnsNoId );
    }

    qDebug().nospace().noquote() << "Fill 'TIMEZONE' table"
                                 << ", Elapsed time: " << timer.restart();


    qDebug().nospace().noquote() << "Fill 'STOCK_EXCHANGE_LIST' table: " 
                                 << pDbMan->insertToBulkFromString( "STOCK_EXCHANGE_LIST"
                                                                  , "MOEX,2011-12-19,0,PAO Moskovskaya Birzha"
                                                                  //, QString("NAME;FOUNDATION_DATE;DESCRIPTION").split(";")
                                                                  , "NAME;FOUNDATION_DATE;TIMEZONE_ID;DESCRIPTION"
                                                                  )
                                 << ", Elapsed time: " << timer.restart();


    QString tzMskSelectQueryText = pDbMan->makeSimpleSelectQueryText( "TIMEZONE", "NAME", "MSK", "ID" );
    auto tzMskSelectRes          = pDbMan->execHelper( tzMskSelectQueryText );

    QVector< QVector<QString> > tzMskSelectStrings = pDbMan->selectResultToStringVectors(tzMskSelectRes);
    if (!tzMskSelectStrings.empty() && !tzMskSelectStrings[0].empty())
    {
        QVector<QString>  mskTimezoneIdVec = tzMskSelectStrings[0];
        mskTimezoneIdVec.push_back("MSK");

        QString seUpdateQueryText = pDbMan->makeSimpleUpdateQueryText( "STOCK_EXCHANGE_LIST", "NAME", "MOEX", mskTimezoneIdVec, "TIMEZONE_ID;TIMEZONE_NAME" );

        bool bRes = false;
        auto updateResultQuery = pDbMan->execHelper(seUpdateQueryText, &bRes);
        if (!bRes)
        {
            qDebug().nospace().noquote() << "Updating MOEX in STOCK_EXCHANGE_LIST failed: " << updateResultQuery.lastError().text();
        }

    }

    //QString updateQuery = pDbMan->makeSimpleUpdateQueryText( "MARKET_INSTRUMENT", "FIGI", instrumentInfo.getFigi(), values, instrumentColsNoLotMarket );
    //pDbMan->execHelper( updateQuery );



    
    return 0;
}



