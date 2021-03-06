#pragma once


#include <iostream>
#include <ostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <optional>
#include <atomic>
#include <deque>
#include <cmath>


#include <QtGlobal>

#include <QString>
#include <QTest>
#include <QElapsedTimer>

#include "invest_openapi.h"
#include "factory.h"
#include "openapi_completable_future.h"
#include "invest_openapi/ioa_db_dictionaries.h"


#include "database_config.h"
#include "database_manager.h"
#include "qt_time_helpers.h"

#include "db_utils.h"
#include "ioa_utils.h"
#include "ioa_ostream.h"
#include "ioa_db_dictionaries.h"

#include "openapi_limits.h"

#include "market_glass.h"
#include "market_instrument_state.h"

#include "operation_helpers.h"
#include "order_helpers.h"
#include "portfolio_helpers.h"

#include "terminal_helpers.h"
#include "format_helpers.h"

#include "invest_openapi/terminal_config.h"



//----------------------------------------------------------------------------
namespace invest_openapi {


namespace trading_terminal {



//----------------------------------------------------------------------------
struct InstrumentInfoLineData
{
    typedef   marty::Decimal  Decimal;

    static const std::size_t  maxLastOperations = 3;


    QString   ticker;
    QString   figi  ;


    bool      isTraded; //!< Торгуется ли сейчас? (From streaming MarketInstrumentState)

    Decimal   priceIncrement; //!< From streaming MarketInstrumentState
    unsigned  lotSize       ; //!< From streaming MarketInstrumentState

    int       pricePrecision = 2;


    Decimal   avgPrice; //!< Средняя цена покупки - брать из портфолио
    Decimal   expYield; //!< Ожидаемая доходность - брать из портфолио
    int       quantity; //!< Количество бумаг в портфеле - брать из портфолио


    Decimal   curPrice    ; //!< Текущая цена - берём из стакана, если не торгуется - показываем последнюю. Или не показывать?
    Decimal   bestAskPrice; //!< берём из стакана
    Decimal   bestBidPrice; //!< берём из стакана
    Decimal   spread      ; //!< берём из стакана
    int       spreadPoints; //!< берём из стакана


    Decimal   lastSellPrice   ; //!< Цена (средняя) последней продажи. Берём из операций по инструменту
    unsigned  lastSellQuantity; //!< Количество executed акций (не лотов). Берём из операций по инструменту

    Decimal   lastBuyPrice    ; //!< Цена (средняя) последней покупки. Берём из операций по инструменту
    unsigned  lastBuyQuantity ; //!< Количество executed акций/бумаг (не лотов). Берём из операций по инструменту


    Decimal   minAskPrice; // Цена продажи (в заявке), если заявка установлена, или 0 (выводим прочерк). Берём из orders минимальную по цене заявку на продажу
    unsigned  minAskQuantity; // Количество акций/бумаг в заявке. Берём из orders минимальную по цене заявку на продажу

    Decimal   maxBidPrice; // Цена покупки (в заявке), если заявка установлена, или 0 (выводим прочерк). Берём из orders максимальную по цене заявку на покупку
    unsigned  maxBidQuantity; // Количество акций/бумаг в заявке. Берём из orders максимальную по цене заявку на покупку

    std::vector<OpenAPI::Order>    sellOrders;
    std::vector<OpenAPI::Order>    buyOrders ;

    std::vector< OpenAPI::Operation >  lastSellOperations;
    std::vector< OpenAPI::Operation >  lastBuyOperations ;


    void invalidateMarketStateFields()
    {
        isTraded       = false;
        priceIncrement = Decimal(0);
        lotSize        = 0;

        pricePrecision = 2;
    
    }

    void invalidateMarketGlassFields()
    {
        curPrice     = Decimal (0);
        bestAskPrice = Decimal (0);
        bestBidPrice = Decimal (0);
        spread       = Decimal (0);
        spreadPoints = unsigned(0);
    }

    void invalidatePortfolioFields()
    {
        // брать из портфолио
        avgPrice = Decimal(0);
        expYield = Decimal(0);
        quantity = 0;
    }


    void init( const DatabaseDictionaries &dicts, QString _figi )
    {
        if (!ticker.isEmpty())
            return;

        figi     = dicts.findFigiByAnyIdString (_figi);
        ticker   = dicts.getTickerByFigiChecked(figi);

        invalidateMarketStateFields();
        invalidatePortfolioFields();
        invalidateMarketGlassFields();

        lastSellPrice    = Decimal(0);
        lastSellQuantity = 0;

        lastBuyPrice     = Decimal(0);
        lastBuyQuantity  = 0;         


        minAskPrice      = Decimal(0);
        minAskQuantity   = 0;         

        maxBidPrice      = Decimal(0);
        maxBidQuantity   = 0;         

    }

    void update( const OpenAPI::PortfolioPosition &portfolioPosition )
    {
        avgPrice = portfolioPosition.getAveragePositionPrice().getValue();
        expYield = portfolioPosition.getExpectedYield().getValue();
        quantity = int(portfolioPosition.getBalance());
    }

    void update( const MarketInstrumentState &marketInstrumentState )
    {
        isTraded       = marketInstrumentState.isTradeStatusNormalTrading();
        priceIncrement = marketInstrumentState.priceIncrement;
        lotSize        = marketInstrumentState.lotSize       ;

        if (priceIncrement!=Decimal(0))
        {
            pricePrecision = priceIncrement.precision();
        }

        if (!isTraded)
        {
            bestAskPrice = Decimal(0);
            bestBidPrice = Decimal(0);
            spread       = Decimal(0);
            spreadPoints = 0;
        }

    }

    void update( const MarketGlass &marketGlass )
    {
        if (priceIncrement==Decimal(0))
        {
            curPrice     = Decimal(0);
            spreadPoints = 0;
        }
        else
        {
            curPrice     = marketGlass.calcInstrumentPrice(priceIncrement);
            spreadPoints = marketGlass.getPriceSpreadPoints(priceIncrement);
        }
        
        bestAskPrice = marketGlass.getAskBestPrice();
        bestBidPrice = marketGlass.getBidBestPrice();
        spread       = marketGlass.getPriceSpread ();

    }

    //! \param operations must be sorted by date in descending order
    void update( const std::vector< OpenAPI::Operation > &operations )
    {
        lastSellOperations.clear();
        lastBuyOperations.clear();
        getSomeOfFirstOperations( maxLastOperations, operations, lastSellOperations, lastBuyOperations, false /* bAppend */ ); 

        if (lastSellOperations.empty())
        {
            lastSellPrice    = Decimal(0);
            lastSellQuantity = 0;
        }
        else
        {
            lastSellPrice    = lastSellOperations[0].getPrice();
            lastSellQuantity = lastSellOperations[0].getQuantityExecuted();
        }

        if (lastBuyOperations.empty())
        {
            lastBuyPrice     = Decimal(0);
            lastBuyQuantity  = 0;
        }
        else
        {
            lastBuyPrice     = lastBuyOperations[0].getPrice();
            lastBuyQuantity  = lastBuyOperations[0].getQuantityExecuted();
        }

    }

    void update( const std::vector<OpenAPI::Order> &ordersParam )
    {
        //getPrice()
        //getRequestedLots()

        std::vector<OpenAPI::Order> orders;
        auto inserter = std::back_inserter(orders);
        for( const auto &o : ordersParam )
        {
           if (isOrderStatusActiveOrder(o))
               *inserter++ = o;
           else
           {
               //std::cerr << "Bad order status, FIGI: " << o.getFigi() << ", Status: " << o.getStatus().asJson() << std::endl;
           }
        }

        sellOrders.clear();
        buyOrders.clear();
        splitOrdersByOperationType( orders, sellOrders, buyOrders, false /* bAppend */ );

        sortOrdersByPrice( sellOrders, SortType::ascending  );
        sortOrdersByPrice( buyOrders , SortType::descending );

        if (sellOrders.empty())
        {
            minAskPrice    = Decimal(0);
            minAskQuantity = 0;
        }
        else
        {
            minAskPrice    = sellOrders[0].getPrice();
            minAskQuantity = sellOrders[0].getRequestedLots();
        }

        if (buyOrders.empty())
        {
            maxBidPrice    = Decimal(0);
            maxBidQuantity = 0;
        }
        else
        {
            maxBidPrice    = buyOrders[0].getPrice();
            maxBidQuantity = buyOrders[0].getRequestedLots();
        }
    
    }


    static
    int sign( int val )
    {
        if (val<0) return -1;
        else if (val>0) return 1;
        return 0;
    }

    static
    int abs( int val )
    {
        if (val<0) return -val;
        return val;
    }

    //------------------------------
    std::string formatFieldAbsHelper( const FieldFormat &ff, bool allowAbsVals, int *pSignOut
                                    , int val
                                    ) const
    {
        if (!allowAbsVals || !pSignOut)
            return invest_openapi::format_field( ff, val );


        *pSignOut = sign(val);
        return invest_openapi::format_field( ff, this->abs(val) );
    }

    std::string formatFieldAbsHelper( const FieldFormat &ff, bool allowAbsVals, int *pSignOut
                                    , Decimal val, int precision = -1
                                    ) const
    {
        if (!allowAbsVals || !pSignOut)
            return invest_openapi::format_field( ff, val, precision );


        *pSignOut = val.sign();
        return invest_openapi::format_field( ff, val.abs(), precision );
    }


    //------------------------------
    std::string format_field( const FieldFormat &ff
                            , QSharedPointer<TerminalConfig> pTermConfig
                            , bool allowAbsVals
                            , int *pSignOut
                            ) const
    {
        QString id = ff.id.toUpper();

        using invest_openapi::format_field;

        if (id=="VB" || id=="VB1" || id=="VB2" || id=="VB3")
        {
            return format_field( ff, "|" );
        }

        else if (id=="SP" || id=="SP1" || id=="SP2" || id=="SP3")
        {
            return format_field( ff, " " );
        }

        else if (id=="TICKER")
        {
            return format_field( ff, ticker.toStdString() );
        }

        else if (id=="FIGI")
        {
            return format_field( ff, figi.toStdString() );
        }

        else if (id=="INSTRUMENT_ACTIVITY_STATE")
        {
            return format_field( ff, isTraded ? "T" : "-" );
        }

        else if (id=="PRICE_INC")
        {
            if (priceIncrement==Decimal(0)) return format_field( ff, "-" );

            return format_field( ff, priceIncrement );
        }

        else if (id=="LOT_SIZE")
        {
            if (lotSize==0) return format_field( ff, "-" );

            return format_field( ff, lotSize );
        }

        else if (id=="PORTFOLIO_PRICE")
        {
            if (avgPrice==Decimal(0))
                return format_field( ff, "-" );

            return format_field( ff, avgPrice, pricePrecision );
        }

        else if (id=="PORTFOLIO_EXPECTED_YIELD")
        {
            if (expYield==Decimal(0))
                return format_field( ff, "-" );

            //return format_field( ff, expYield, 1 );
            return formatFieldAbsHelper( ff, allowAbsVals, pSignOut, expYield, 1 );
        }

        else if (id=="PORTFOLIO_EXPECTED_YIELD_PERCENT")
        {
            // avgPrice * quantity - 100%
            // expYield

            if (quantity==0 || avgPrice.zer() || expYield.zer())
                return format_field( ff, "-" );
            

            auto portfolioCost    = avgPrice * quantity;
            auto absPortfolioCost = portfolioCost.abs();

            auto profitPercent    = absPortfolioCost.getPercent(expYield);

            if (profitPercent>=Decimal(100))
                profitPercent.rounded(0, marty::Decimal::RoundingMethod::roundMath );

            else if (profitPercent>=Decimal(10))
                profitPercent.rounded(1, marty::Decimal::RoundingMethod::roundMath );

            else
                profitPercent.rounded(2, marty::Decimal::RoundingMethod::roundMath );

            return formatFieldAbsHelper( ff, allowAbsVals, pSignOut, profitPercent, -1 );
        }

        else if (id=="PORTFOLIO_QUANTITY")
        {
            // return format_field( ff, quantity );
            return formatFieldAbsHelper( ff, allowAbsVals, pSignOut, quantity );
        }
        else if (id=="CUR_PRICE")
        {
            if (curPrice==Decimal(0))
                return format_field( ff, "-" );

            if (avgPrice.zer() || curPrice.zer())
               return format_field( ff, curPrice, pricePrecision );

            auto priceDelta = curPrice - avgPrice;
            if (pSignOut)
               *pSignOut = priceDelta.sgn();

            return format_field( ff, curPrice, pricePrecision );
        }
        else if (id=="BEST_BID")
        {
            if (bestBidPrice==Decimal(0)) return format_field( ff, "-" );

            return format_field( ff, bestBidPrice );
        }
        else if (id=="BEST_ASK")
        {
            if (bestAskPrice==Decimal(0)) return format_field( ff, "-" );

            return format_field( ff, bestAskPrice );
        }
        else if (id=="SPREAD_POINTS")
        {
            if (spreadPoints<1) return format_field( ff, "-" );

            return format_field( ff, spreadPoints );
        }


        // Last Buy/Sell and they quantity - если инструмент не торгуется, выводим пустышку
        // Просто это мощный блок информации, и когда он не очищен, то возникает дисонанс с другими полями

        else if (id=="LAST_BUY_PRICE")
        {
            if (lastBuyPrice==Decimal(0) || !isTraded) return format_field( ff, "-" );

            return format_field( ff, lastBuyPrice, pricePrecision );
        }
        else if (id=="LAST_BUY_QUANTITY")
        {
            if (lastBuyQuantity==0 || !isTraded) return format_field( ff, "-" );

            return format_field( ff, lastBuyQuantity );
        }
        else if (id=="LAST_SELL_PRICE")
        {
            if (lastSellPrice==Decimal(0) || !isTraded) return format_field( ff, "-" );

            return format_field( ff, lastSellPrice, pricePrecision );
        }
        else if (id=="LAST_SELL_QUANTITY")
        {
            if (lastSellQuantity==0 || !isTraded) return format_field( ff, "-" );

            return format_field( ff, lastSellQuantity );
        }
        else if (id=="MAX_BID_PRICE")
        {
            if (maxBidPrice==Decimal(0)) return format_field( ff, "-" );

            return format_field( ff, maxBidPrice, pricePrecision );
        }
        else if (id=="MAX_BID_QUANTITY")
        {
            if (maxBidQuantity==0 || lotSize==0) return format_field( ff, "-" );

            return format_field( ff, maxBidQuantity*lotSize );
        }
        else if (id=="MIN_ASK_PRICE")
        {
            if (minAskPrice==Decimal(0)) return format_field( ff, "-" );

            return format_field( ff, minAskPrice, pricePrecision );
        }
        else if (id=="MIN_ASK_QUANTITY")
        {
            if (minAskQuantity==0 || lotSize==0) return format_field( ff, "-" );

            return format_field( ff, minAskQuantity*lotSize );
        }
        // else if (id=="")
        // {
        // }

        return format_field( ff, "N/I" );

    }

    //------------------------------
    #define DECLARE_IOA_TRADING_TERMINAL_LINE_DATA_UPDATE_FUNCTION( dataType )                \
    static                                                                                    \
    void updateTerminalData( std::map< QString, InstrumentInfoLineData > &terminalData        \
                           , const DatabaseDictionaries &dicts, QString figi                  \
                           , const dataType &data                                             \
                           )                                                                  \
    {                                                                                         \
        figi           = figi.toUpper();                                                      \
        auto &lineData = terminalData[figi];                                                  \
                                                                                              \
        lineData.init(dicts, figi); /* Если новый элемент - инициализируется, если существующий - ничего не происходит */ \
                                                                                              \
        lineData.update(data);                                                                \
    }

    DECLARE_IOA_TRADING_TERMINAL_LINE_DATA_UPDATE_FUNCTION(MarketInstrumentState)
    DECLARE_IOA_TRADING_TERMINAL_LINE_DATA_UPDATE_FUNCTION(MarketGlass)
    DECLARE_IOA_TRADING_TERMINAL_LINE_DATA_UPDATE_FUNCTION(std::vector< OpenAPI::Operation >)
    DECLARE_IOA_TRADING_TERMINAL_LINE_DATA_UPDATE_FUNCTION(std::vector<OpenAPI::Order>)


}; // struct InstrumentInfoLineData

//----------------------------------------------------------------------------






//----------------------------------------------------------------------------
class TradingTerminalData
{

public:

    enum class ConnectionState
    {
        disconnected = 0,
        connecting   = 1,
        connected    = 2
    };


protected:

    QStringList                                                            instrumentList;
    std::map< QString, int >                                               instrumentNumbers;
    DatabaseDictionaries                                                  *pDicts;

    std::map< QString, MarketInstrumentState >                             instrumentStates;
    std::map< QString, MarketGlass           >                             instrumentGlasses;
    std::map< QString, std::vector< OpenAPI::Operation > >                 instrumentOperations;
    std::map< QString, std::vector< OpenAPI::Order > >                     activeOrders;
    std::map< QString, OpenAPI::PortfolioPosition >                        currentPortfolio;

    QString                                                                statusStr;
    QDateTime                                                              statusChangedDateTime;

    ConnectionState                                                        connectionStatus;
    QDateTime                                                              connectionStatusChangedDateTime;

    std::map< QString, CurrencyPosition >                                  currencyPositions;

    QSharedPointer<TerminalConfig>                                         pTermConfig;

    std::map< QString, InstrumentInfoLineData >                            terminalLinesData;

    //------------------------------

    std::set< QString >   updatedFigis;
    bool statusUpdated             = false;
    bool connectionStatusUpdated   = false;

    bool captionUpdated   = true;

    //------------------------------

    template< typename ValType >
    bool updateVal( ValType &val, ValType newVal ) const //!< Возвращает true, если значение поменялось
    {
        bool res = false;

        if ( val!=newVal )
            res = true;

        val = newVal;

        return res;
    }


    //------------------------------
    bool updateFigiDataImpl( QString figi, MarketInstrumentState &curData, const MarketInstrumentState &newData )
    {
        bool changed = false;

        // Тупо чекаем, что нужно почекать
        auto curTradeStatusValue = curData.tradeStatus.getValue();
        changed |= updateVal( curTradeStatusValue, newData.tradeStatus.getValue() );

        changed |= updateVal( curData.priceIncrement, newData.priceIncrement );
        changed |= updateVal( curData.lotSize       , newData.lotSize );

        curData = newData; // И присваиваем всё скопом

        return changed;
    }

    bool updateFigiDataImpl( QString figi, MarketGlass &curData, const MarketGlass &newData )
    {
        curData = newData;
        return true;
    }

    bool updateFigiDataImpl( QString figi, std::vector< OpenAPI::Operation > &curData, const std::vector< OpenAPI::Operation > &newData )
    {
        curData = newData; // UNDONE
        return true;
    }

    bool updateFigiDataImpl( QString figi, std::vector< OpenAPI::Order > &curData, const std::vector< OpenAPI::Order > &newData )
    {

        for( const auto &order: newData )
        {
            qDebug().nospace().noquote() << "Active Order, "
                                         <<      "FIGI: " << order.getFigi()                   << ", "
                                         <<       " ID: " << order.getOrderId()                << ", "
                                         << "Operation: " << order.getOperation().asJson()     << ", "
                                         <<    "Status: " << order.getStatus().asJson()        << ", "
                                         <<      "Type: " << order.getType().asJson()          << ", "
                                         <<  "Req Lots: " << order.getRequestedLots()          << ", "
                                         <<     "Price: " << order.getPrice().toString().c_str()       ; // << ", "
                                         

        }
        
        //void sortOrdersByPriceDescending( std::vector< OpenAPI::Order > & ops )

        bool eq = true;

        if (curData.size()!=newData.size())
        {
            eq = false;
        }
        else // sizes are equal
        {
            std::vector< OpenAPI::Order > curDataSorted = curData;  sortOrdersById(curDataSorted);
            std::vector< OpenAPI::Order > newDataSorted = newData;  sortOrdersById(newDataSorted);

            std::vector< OpenAPI::Order >::const_iterator it1 = curDataSorted.begin();
            std::vector< OpenAPI::Order >::const_iterator it2 = newDataSorted.begin();

            for( ; it1!=curDataSorted.end(); ++it1, ++it2 )
            {
                if (it1->getOrderId() != it2->getOrderId())
                {
                    eq = false;
                    break;
                }
            }
        }


        if (!eq)
        {
            curData = newData;
        }
        
        return !eq;
    }


    bool updateFigiDataImpl( QString figi, OpenAPI::PortfolioPosition &curData, const OpenAPI::PortfolioPosition &newData )
    {
        curData = newData; // UNDONE
        return true;
    }



public:


    //------------------------------
    TradingTerminalData( DatabaseDictionaries *pd )
    : instrumentList       ()
    , instrumentNumbers    ()
    , pDicts               (pd)
    , instrumentStates     ()
    , instrumentGlasses    ()
    , instrumentOperations ()
    , activeOrders         ()
    , statusStr            ()
    , statusChangedDateTime(QDateTime::currentDateTime())
    , connectionStatus     (ConnectionState::disconnected)
    , connectionStatusChangedDateTime(QDateTime::currentDateTime())
    , terminalLinesData    ()
    {
    }
   
    TradingTerminalData( DatabaseDictionaries *pd, QStringList il )
    : instrumentList       ()
    , instrumentNumbers    ()
    , pDicts               (pd)
    , instrumentStates     ()
    , instrumentGlasses    ()
    , instrumentOperations ()
    , activeOrders         ()
    , statusStr            ()
    , statusChangedDateTime(QDateTime::currentDateTime())
    , connectionStatus     (ConnectionState::disconnected)
    , connectionStatusChangedDateTime(QDateTime::currentDateTime())
    , pTermConfig          (0)
    , terminalLinesData    ()
    {
        setInstrumentList( il );
    }


    //------------------------------
    template< typename MapElemenType >
    void fillMapKeysWithFigis( std::map< QString, MapElemenType > &m, MapElemenType defValue ) const
    {
        for( auto figi : instrumentList )
        {
            m[ figi.toUpper() ] = defValue;
        }
    }

    //------------------------------




    //------------------------------
    void setStatus( QString str )
    {
        statusStr             = str;
        statusChangedDateTime = QDateTime::currentDateTime();
        statusUpdated         = true;

        qInfo().nospace().noquote() << "Status: " << statusStr;
    }

    QString   getStatus() const           { return statusStr; }
    QDateTime getStatusDateTime() const   { return statusChangedDateTime; }
    QString   getStatusDateTimeStr() const 
    {
        std::ostringstream oss;
        oss << statusChangedDateTime;
        return QString::fromStdString(oss.str());
    }
/*
    enum class ConnectionState
    {
        disconnected,
        connecting  ,
        connected
    };
*/
    void setConnectionState( ConnectionState st )
    {

        connectionStatus                = st;
        connectionStatusChangedDateTime = QDateTime::currentDateTime();
        connectionStatusUpdated         = true;

        switch(st)
        {
            case ConnectionState::disconnected: qInfo().nospace().noquote() << "Connection State: Disconnected"  ; break;
            case ConnectionState::connecting  : qInfo().nospace().noquote() << "Connection State: Connecting"    ; break;
            case ConnectionState::connected   : qInfo().nospace().noquote() << "Connection State: Connected (OK)"; break;
            default:                            qInfo().nospace().noquote() << "Connection State: UNKNOWN"       ; break;
        }

    }

    QString   getConnectionStateStr() const
    {
        switch(connectionStatus)
        {
            case ConnectionState::disconnected: return "Disconnected" ;
            case ConnectionState::connecting  : return "Connecting...";
            case ConnectionState::connected   : return "Connected"    ;
            default                           : return "Unknown"      ;
        };
    }

    #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
    umba::term::colors::SgrColor getConnectionStateColor() const
    {
        
        switch(connectionStatus)
        {
            case ConnectionState::disconnected: return pTermConfig->colors.getStateColor(0);
            case ConnectionState::connecting  : return pTermConfig->colors.getStateColor(1);
            case ConnectionState::connected   : return pTermConfig->colors.getStateColor(2);
            default                           : return pTermConfig->colors.getStateColor(0);
        };
    }
    #endif

    QDateTime getConnectionStateDateTime() const { return connectionStatusChangedDateTime; }

    QString   getConnectionStateDateTimeStr() const 
    {
        std::ostringstream oss;
        oss << connectionStatusChangedDateTime;
        return QString::fromStdString(oss.str());
    }

    //------------------------------

    QString makeCurrencyPositionsStr( const std::map< QString, OpenAPI::CurrencyPosition > &positions ) const
    {
        QString resStr;

        std::map< QString, CurrencyPosition >::const_iterator it = positions.begin();
        for(; it != positions.end(); ++it)
        {
            marty::Decimal balance = it->second.getBalance();
            if (balance==marty::Decimal(0))
                continue;

            std::ostringstream oss;

            oss << it->first << ": " << it->second.getBalance().rounded(2, marty::Decimal::RoundingMethod::roundMath );

            marty::Decimal blocked = it->second.getBlocked();
            if (blocked!=marty::Decimal(0))
                oss << " (" << blocked.rounded(2, marty::Decimal::RoundingMethod::roundMath ) << ")";

            oss << ";";

            if (!resStr.isEmpty())
            {
                resStr += " ";
            }

            resStr += QString::fromStdString(oss.str());
        
        }

        return resStr;
    
    }


    QString getPortfolioBalanceStr() const
    {

        std::map< QString, OpenAPI::CurrencyPosition > portfolioBalanceCurrencyPositions = currencyPositions; // Остатки валют тоже учитываем

        // FIGI -> PortfolioPosition map
        std::map< QString, OpenAPI::PortfolioPosition >::const_iterator pit = currentPortfolio.begin();


        marty::Decimal sumValue = 0;

        for(; pit!=currentPortfolio.end(); ++pit)
        {
            // Ignore FIGI

            //OpenAPI::CurrencyPosition pos;

            OpenAPI::MoneyAmount positionCurrentCost = getPortfolioPositionCurrentCost( pit->second );
                                                    // getPortfolioPositionSpentCost

            marty::Decimal curCostValue = positionCurrentCost.getValue();
            sumValue += curCostValue;

            /*
            qDebug().nospace().noquote() << "??? " << "getPortfolioBalanceStr for " << pit->second.getTicker() << " - "
                                                   << "Cost: " << curCostValue.toString().c_str() << ", "
                                                   << "Summary: " << sumValue.toString().c_str(); // << ", "
                                                   //<< "Cost: " << cost.toString().c_str() ; // << ", "
            */

            QString strCurrency = positionCurrentCost.getCurrency().asJson().toUpper();
            std::map< QString, OpenAPI::CurrencyPosition >::iterator pbcpIt = portfolioBalanceCurrencyPositions.find(strCurrency);

            if (pbcpIt==portfolioBalanceCurrencyPositions.end())
            {
                OpenAPI::CurrencyPosition pos;
                pos.setBlocked( marty::Decimal(0) ); 
                pos.setCurrency( positionCurrentCost.getCurrency() );
                pos.setBalance ( curCostValue );
                portfolioBalanceCurrencyPositions[strCurrency] = pos;
            }
            else
            {
                marty::Decimal sum = pbcpIt->second.getBalance() + curCostValue;

                // Каждый раз, ну да ладно, лень вначале обнулить, что пришло из currencyPositions
                pbcpIt->second.setBalance ( sum );
                pbcpIt->second.setBlocked( marty::Decimal(0) ); 
            }

            // pos.setCurrency( positionCurrentCost.getCurrency() );
            // pos.setBalance ( curCostValue );

            //portfolioBalanceCurrencyPositions[pos.getCurrency()] = pos;
            //OpenAPI::CurrencyPosition &pos = portfolioBalanceCurrencyPositions[]; // = pos;

            // Каждый раз, ну да ладно
            //pos.setBlocked( marty::Decimal(0) ); 
            //pos.setCurrency( positionCurrentCost.getCurrency() );

            //pos.setBalance ( pos.getBalance() + curCostValue );


            // pos.getCurrency().asJson().toUpper()

        }

        QString resStr = makeCurrencyPositionsStr( portfolioBalanceCurrencyPositions );



/*

std::map< QString, CurrencyPosition >::const_iterator it = currencyPositions.begin();


CurrencyPosition

    Currency getCurrency() const;
    void setCurrency(const Currency &currency);
    bool is_currency_Set() const;
    bool is_currency_Valid() const;

    marty::Decimal getBalance() const;
    void setBalance(const marty::Decimal &balance);
    bool is_balance_Set() const;
    bool is_balance_Valid() const;

    marty::Decimal getBlocked() const;
    void setBlocked(const marty::Decimal &blocked);
    bool is_blocked_Set() const;
    bool is_blocked_Valid() const;


Currency



PortfolioPosition



    marty::Decimal getBalance() const;
    void setBalance(const marty::Decimal &balance);
    bool is_balance_Set() const;
    bool is_balance_Valid() const;

    marty::Decimal getBlocked() const;
    void setBlocked(const marty::Decimal &blocked);
    bool is_blocked_Set() const;
    bool is_blocked_Valid() const;

    MoneyAmount getExpectedYield() const;
    void setExpectedYield(const MoneyAmount &expected_yield);
    bool is_expected_yield_Set() const;
    bool is_expected_yield_Valid() const;

    qint32 getLots() const;
    void setLots(const qint32 &lots);
    bool is_lots_Set() const;
    bool is_lots_Valid() const;

    MoneyAmount getAveragePositionPrice() const;
    void setAveragePositionPrice(const MoneyAmount &average_position_price);
    bool is_average_position_price_Set() const;
    bool is_average_position_price_Valid() const;

*/


        if (resStr.isEmpty())
        {
            if (portfolioBalanceCurrencyPositions.empty())
                resStr = "Portfolio Not Available";
            else
                resStr = "Portfolio Balance is zero";
        }

        return resStr;

    }
    

    //------------------------------
    QString getCurrenciesBalanceStr() const
    {
        QString resStr = makeCurrencyPositionsStr( currencyPositions );

        /*
        std::map< QString, CurrencyPosition >::const_iterator it = currencyPositions.begin();
        for(; it != currencyPositions.end(); ++it)
        {
            marty::Decimal balance = it->second.getBalance();
            if (balance==marty::Decimal(0))
                continue;

            std::ostringstream oss;

            oss << it->first << ": " << it->second.getBalance().rounded(2, marty::Decimal::RoundingMethod::roundMath );

            marty::Decimal blocked = it->second.getBlocked();
            if (blocked!=marty::Decimal(0))
                oss << " (" << blocked.rounded(2, marty::Decimal::RoundingMethod::roundMath ) << ")";

            oss << ";";

            if (!resStr.isEmpty())
            {
                resStr += " ";
            }

            resStr += QString::fromStdString(oss.str());
        
        }
        */

        if (resStr.isEmpty())
        {
            if (currencyPositions.empty())
                resStr = "Currency Positions Not Available";
            else
                resStr = "Balance is zero";
        }

        return resStr;
    }


    //std::map< QString, CurrencyPosition >                                  currencyPositions;


    //------------------------------
    void setConfig( QSharedPointer<TerminalConfig> cfg )
    {
        pTermConfig = cfg;
    }


    //------------------------------
    bool addInstrument( QString figi )
    {
        figi = pDicts->findFigiByAnyIdString(figi);

        if (instrumentNumbers.find(figi)!=instrumentNumbers.end())
            return false;

        instrumentNumbers[figi] = (int)instrumentList.size();
        instrumentList.push_back(figi);

        auto &lineData = terminalLinesData[figi];
        
        lineData.init( *pDicts, figi );

        updatedFigis.insert(figi);

        captionUpdated = true; // На самом деле оно заставляет перерисовывать все рамки

        return true;
    }

    void setInstrumentList( QStringList il )
    {
         for( auto figi : il)
         {
             addInstrument(figi);
         }
    }

    bool isKnownInstrument( QString figi ) const
    {
        figi = pDicts->findFigiByAnyIdString(figi);

        for( auto fg : instrumentList )
        {
            if (fg==figi)
            {
                return true;
            }
        }

        return false;
    }

    // addInstrument( QString figi )
    

    //------------------------------

    QStringList::const_iterator instrumentListBegin() const { return instrumentList.begin(); }
    QStringList::const_iterator instrumentListEnd  () const { return instrumentList.end  (); }

    int getFigiCount( ) const { return instrumentNumbers.size(); }

    int getFigiIndex( QString figi ) const
    {
        figi.toUpper();

        std::map< QString, int >::const_iterator it = instrumentNumbers.find(figi);
        if (it==instrumentNumbers.end())
            return -1;

        return it->second;
    }

    QString getFigiByIndex( int idx ) const
    {
        std::size_t szIdx = (std::size_t)idx;
        if (szIdx>=(std::size_t)instrumentList.size())
            throw std::runtime_error("TradingTerminalData::getFigiByIndex: Instrument index is out of range");
        return instrumentList[szIdx];
    }

    //------------------------------
    bool isStatusChanged() const { return statusUpdated; }

    bool isConnectionStatusChanged() const { return connectionStatusUpdated; }

    bool isFigiChanged()   const { return !updatedFigis.empty(); } 

    bool isFigiChanged( QString figi ) const
    {
        figi.toUpper();

        return updatedFigis.find(figi)!=updatedFigis.end();
    } 

    bool isCaptionChanged() const { return captionUpdated; }

    //------------------------------

    void clearChangedFlags()
    {
        statusUpdated              = false;
        captionUpdated             = false;
        connectionStatusUpdated    = false;
        updatedFigis.clear();
    }

    //------------------------------

    std::set<QString> getTickersLikeThis( QString t ) const
    {
        std::set<QString> resSet;

        for( auto figi : instrumentList )
        {
            QString ticker = pDicts->getTickerByFigi( figi );
            if (ticker.isEmpty())
                continue;

            if (ticker.startsWith(t, Qt::CaseInsensitive)  /* && ticker.size()>t.size() */  )
            {
                resSet.insert(ticker);
            }
        }

        return resSet;
    }

    QString getTickerLikeThis( QString t ) const
    {
        std::set<QString> ids = getTickersLikeThis( t );
        if (ids.size()!=1)
            return QString();
        return *ids.begin();
    }


    //------------------------------
    bool getInstrumentMarketState( QString figi, MarketInstrumentState &st ) const
    {
        figi = pDicts->findFigiByAnyIdString(figi);

        std::map< QString, MarketInstrumentState >::const_iterator it = instrumentStates.find(figi);
        if (it==instrumentStates.end())
            return false;

        st = it->second;

        return true;
    }

    bool getInstrumentMarketGlass( QString figi, MarketGlass &gl ) const
    {
        figi = pDicts->findFigiByAnyIdString(figi);

        std::map< QString, MarketGlass >::const_iterator it = instrumentGlasses.find(figi);
        if (it==instrumentGlasses.end())
            return false;

        gl = it->second;

        return true;
    }


    //------------------------------
    std::vector< std::size_t > calcColPositionsBySizes( const std::vector< std::size_t > &sizesVec ) const
    {
        std::vector< std::size_t > posVec;

        std::size_t pos = 0;

        for( auto sz: sizesVec)
        {
            posVec.push_back(pos);
            pos += sz;
        }

        return posVec;
    }

    //------------------------------
    std::size_t getMainViewColsCount() const
    {
        return pTermConfig->fieldsFormat.size();
    }

    std::vector< std::size_t > getMainViewColSizes() const
    {
        std::vector< std::size_t > res;

        std::size_t i = 0;
        for(; i!=pTermConfig->fieldsFormat.size(); ++i)
        {
            res.push_back( pTermConfig->fieldsFormat[i].getSummaryWidth() );
        }

        return res;
    }

    std::vector< std::size_t > getMainViewColPositions() const
    {
        return calcColPositionsBySizes( getMainViewColSizes() );
    }

    std::size_t getMainViewTotalWidth() const
    {
        std::vector< std::size_t > sizes = getMainViewColSizes();
        if (sizes.empty())
            return 0;

        std::vector< std::size_t > positions = calcColPositionsBySizes(sizes);
        if (positions.empty())
            return 0;

        return positions.back() + sizes.back();
    }

    //------------------------------

    std::string formatMainViewColCaption( std::size_t colNo ) const
    {
        if (colNo>=pTermConfig->fieldsFormat.size())
            throw std::runtime_error( "TradingTerminalData::formatCaption: column index is out of range" );
        
        return format_field_caption( pTermConfig->fieldsFormat[colNo] );
    }

    std::string formatMainViewField( QString figi, std::size_t colNo
                                   #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
                                   , umba::term::colors::SgrColor *pColorOut = 0 
                                   #endif
                                   ) const
    {
        if (colNo>=pTermConfig->fieldsFormat.size())
            throw std::runtime_error( "TradingTerminalData::formatMainViewField: column index is out of range" );


        figi = pDicts->findFigiByAnyIdString(figi);

        std::map< QString, InstrumentInfoLineData >::const_iterator it = terminalLinesData.find(figi);

        if (it==terminalLinesData.end())
        {
            #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
            if (pColorOut)
               *pColorOut = pTermConfig->colors.genericNormal;
            #endif
            return "???";
        }


        bool allowAbsVals = true;

        #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
        if (!pColorOut)
            allowAbsVals = false;
        #endif

        if (!pTermConfig->colors.hasLessColor() || !pTermConfig->colors.hasGreaterColor())
            allowAbsVals = false;

        int valSign = 0;

        auto fieldFormat = pTermConfig->fieldsFormat[colNo];

        std::string resStr = it->second.format_field( fieldFormat, pTermConfig, allowAbsVals, &valSign );

        #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
        if (pColorOut)
        {
            if (fieldFormat.hasCustomColor())
            {
                *pColorOut = pTermConfig->fieldsFormat[colNo].color;
            }
            else
            {
                if (valSign<0)
                   *pColorOut = pTermConfig->colors.genericLess;
                else if (valSign>0)
                   *pColorOut = pTermConfig->colors.genericGreater;
                else
                   *pColorOut = pTermConfig->colors.genericNormal;
            }
        }
        #endif

        return resStr;

    }

    std::string formatMainViewField( int figiIdx, std::size_t colNo
                                   #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
                                   , umba::term::colors::SgrColor *pColorOut = 0
                                   #endif
                                   ) const
    {
        return formatMainViewField( getFigiByIndex(figiIdx), colNo
                                  #if defined(INVEST_OPENAPI_TEXT_TERMINAL_ENABLED_COLORS)
                                  , pColorOut
                                  #endif
                                  );
    }
/*
    std::string format_field( const FieldFormat &ff
                            , QSharedPointer<TerminalConfig> pTermConfig
                            , bool allowAbsVals
                            , int *pSignOut
                            ) const

*/


    //------------------------------

    void update( const std::map< QString, CurrencyPosition > &m )
    {
        currencyPositions = m;
    }

    void update( const std::map< QString, std::vector<OpenAPI::Order> >  &activeOrders )
    {
        std::map< QString, std::vector<OpenAPI::Order> >::const_iterator it = activeOrders.begin();
        for( ; it!=activeOrders.end(); ++it)
        {
            update( it->first, it->second );
        }
    }

    void update( const std::map< QString, OpenAPI::PortfolioPosition >  &portfolioPositions )
    {
        std::map< QString, OpenAPI::PortfolioPosition >::const_iterator it = portfolioPositions.begin();
        for( ; it!=portfolioPositions.end(); ++it)
        {
            update( it->first, it->second );
        }
    }


    


    // std::string format_field( const FieldFormat &ff ) const
    


    /*
    bool update( QString figi                                                                 
               , const MarketInstrumentState &data                                                         
               )                                                                              
    {                                                                                         
        figi           = figi.toUpper();                                                      
                                                                                              
        auto &figiData = instrumentStates [figi];                                                      
        if (updateFigiDataImpl(figiData,data))                                                
        {                                                                                     
            updatedFigis.insert(figi);                                                        
            auto &lineData = terminalLinesData[figi];                                         
            lineData.update(data);                                                            
                                                                                              
            return true;                                                                      
        }                                                                                     
                                                                                              
        return false;                                                                         
    }
    */

    static QString getTypeNameForLogInUpdateMethod( const MarketInstrumentState             & )    { return QString("Market Instrument State"); }
    static QString getTypeNameForLogInUpdateMethod( const MarketGlass                       & )    { return QString("Market Glass"); }
    static QString getTypeNameForLogInUpdateMethod( const std::vector< OpenAPI::Operation > & )    { return QString("Operations List"); }
    static QString getTypeNameForLogInUpdateMethod( const std::vector< OpenAPI::Order     > & )    { return QString("Active Orders List"); }
    static QString getTypeNameForLogInUpdateMethod( const OpenAPI::PortfolioPosition        & )    { return QString("Portfolio Position"); }

    static int getSizeForLogInUpdateMethod( const MarketInstrumentState             &  )    { return -1; }
    static int getSizeForLogInUpdateMethod( const MarketGlass                       &  )    { return -1; }
    static int getSizeForLogInUpdateMethod( const std::vector< OpenAPI::Operation > &v )    { return (int)v.size(); }
    static int getSizeForLogInUpdateMethod( const std::vector< OpenAPI::Order     > &v )    { return (int)v.size(); }
    static int getSizeForLogInUpdateMethod( const OpenAPI::PortfolioPosition        &v )    { return -1; }


    // bool updateFigiDataImpl( MarketInstrumentState &curData, const MarketInstrumentState &newData )
    // bool updateFigiDataImpl( MarketGlass &curData, const MarketGlass &newData )
    // bool updateFigiDataImpl( std::vector< OpenAPI::Operation > &curData, const std::vector< OpenAPI::Operation > &newData )
    // bool updateFigiDataImpl( std::vector< OpenAPI::Order > &curData, const std::vector< OpenAPI::Order > &newData )
    // bool updateFigiDataImpl( OpenAPI::PortfolioPosition &curData, const OpenAPI::PortfolioPosition &newData )


    #define DECLARE_IOA_TRADING_TERMINAL_DATA_UPDATE_FUNCTION( dataType, mapName )            \
    bool update( QString figi                                                                 \
               , const dataType &data                                                         \
               )                                                                              \
    {                                                                                         \
        figi           = figi.toUpper();                                                      \
        auto ticker    = pDicts->getTickerByFigiChecked(figi);                                \
                                                                                              \
                                                                                              \
        QString updatedName = getTypeNameForLogInUpdateMethod(data);                          \
        int numElementsInUpdate = getSizeForLogInUpdateMethod(data);                          \
        if (numElementsInUpdate<0)                                                            \
        {                                                                                     \
            qInfo().nospace().noquote() << "Updating " << updatedName << " for '" << ticker << " (" << figi << ")"; \
        }                                                                                     \
        else                                                                                  \
        {                                                                                     \
            qInfo().nospace().noquote() << "Updating " << updatedName << " for '" << ticker << " (" << figi << "), number of elements: " << numElementsInUpdate;  \
        }                                                                                     \
                                                                                              \
                                                                                              \
        auto &figiData = mapName [figi];                                                      \
                                                                                              \
        if (updateFigiDataImpl( figi, figiData, data ))                                       \
        {                                                                                     \
            updatedFigis.insert(figi);                                                        \
            auto &lineData = terminalLinesData[figi];                                         \
            lineData.update(data);                                                            \
                                                                                              \
            return true;                                                                      \
        }                                                                                     \
                                                                                              \
        return false;                                                                         \
    }


    DECLARE_IOA_TRADING_TERMINAL_DATA_UPDATE_FUNCTION( MarketInstrumentState             , instrumentStates     )
    DECLARE_IOA_TRADING_TERMINAL_DATA_UPDATE_FUNCTION( MarketGlass                       , instrumentGlasses    )
    DECLARE_IOA_TRADING_TERMINAL_DATA_UPDATE_FUNCTION( std::vector< OpenAPI::Operation > , instrumentOperations )
    DECLARE_IOA_TRADING_TERMINAL_DATA_UPDATE_FUNCTION( std::vector< OpenAPI::Order >     , activeOrders         )

    DECLARE_IOA_TRADING_TERMINAL_DATA_UPDATE_FUNCTION( OpenAPI::PortfolioPosition        , currentPortfolio     )

    

    

}; // class TradingTerminalData

//----------------------------------------------------------------------------







//----------------------------------------------------------------------------


} // namespace trading_terminal

} // namespace invest_openapi

//----------------------------------------------------------------------------

