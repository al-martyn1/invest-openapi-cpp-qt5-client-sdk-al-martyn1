QT += network

HEADERS += \
# Models
    $${PWD}/BrokerAccountType.h \
    $${PWD}/Candle.h \
    $${PWD}/CandleResolution.h \
    $${PWD}/Candles.h \
    $${PWD}/CandlesResponse.h \
    $${PWD}/Currencies.h \
    $${PWD}/Currency.h \
    $${PWD}/CurrencyPosition.h \
    $${PWD}/Empty.h \
    $${PWD}/Error.h \
    $${PWD}/Error_payload.h \
    $${PWD}/InstrumentType.h \
    $${PWD}/LimitOrderRequest.h \
    $${PWD}/LimitOrderResponse.h \
    $${PWD}/MarketInstrument.h \
    $${PWD}/MarketInstrumentList.h \
    $${PWD}/MarketInstrumentListResponse.h \
    $${PWD}/MarketInstrumentResponse.h \
    $${PWD}/MarketOrderRequest.h \
    $${PWD}/MarketOrderResponse.h \
    $${PWD}/MoneyAmount.h \
    $${PWD}/Operation.h \
    $${PWD}/OperationStatus.h \
    $${PWD}/OperationTrade.h \
    $${PWD}/OperationType.h \
    $${PWD}/OperationTypeWithCommission.h \
    $${PWD}/Operations.h \
    $${PWD}/OperationsResponse.h \
    $${PWD}/Order.h \
    $${PWD}/OrderResponse.h \
    $${PWD}/OrderStatus.h \
    $${PWD}/OrderType.h \
    $${PWD}/Orderbook.h \
    $${PWD}/OrderbookResponse.h \
    $${PWD}/OrdersResponse.h \
    $${PWD}/PlacedLimitOrder.h \
    $${PWD}/PlacedMarketOrder.h \
    $${PWD}/Portfolio.h \
    $${PWD}/PortfolioCurrenciesResponse.h \
    $${PWD}/PortfolioPosition.h \
    $${PWD}/PortfolioResponse.h \
    $${PWD}/SandboxAccount.h \
    $${PWD}/SandboxCurrency.h \
    $${PWD}/SandboxRegisterRequest.h \
    $${PWD}/SandboxRegisterResponse.h \
    $${PWD}/SandboxSetCurrencyBalanceRequest.h \
    $${PWD}/SandboxSetPositionBalanceRequest.h \
    $${PWD}/SearchMarketInstrument.h \
    $${PWD}/SearchMarketInstrumentResponse.h \
    $${PWD}/TradeStatus.h \
    $${PWD}/UserAccount.h \
    $${PWD}/UserAccounts.h \
    $${PWD}/UserAccountsResponse.h \
# APIs
    $${PWD}/MarketApi.h \
    $${PWD}/OperationsApi.h \
    $${PWD}/OrdersApi.h \
    $${PWD}/PortfolioApi.h \
    $${PWD}/SandboxApi.h \
    $${PWD}/UserApi.h \
# Others
    $${PWD}/Helpers.h \
    $${PWD}/HttpRequest.h \
    $${PWD}/Object.h \
    $${PWD}/Enum.h \
    $${PWD}/HttpFileElement.h

SOURCES += \
# Models
    $${PWD}/BrokerAccountType.cpp \
    $${PWD}/Candle.cpp \
    $${PWD}/CandleResolution.cpp \
    $${PWD}/Candles.cpp \
    $${PWD}/CandlesResponse.cpp \
    $${PWD}/Currencies.cpp \
    $${PWD}/Currency.cpp \
    $${PWD}/CurrencyPosition.cpp \
    $${PWD}/Empty.cpp \
    $${PWD}/Error.cpp \
    $${PWD}/Error_payload.cpp \
    $${PWD}/InstrumentType.cpp \
    $${PWD}/LimitOrderRequest.cpp \
    $${PWD}/LimitOrderResponse.cpp \
    $${PWD}/MarketInstrument.cpp \
    $${PWD}/MarketInstrumentList.cpp \
    $${PWD}/MarketInstrumentListResponse.cpp \
    $${PWD}/MarketInstrumentResponse.cpp \
    $${PWD}/MarketOrderRequest.cpp \
    $${PWD}/MarketOrderResponse.cpp \
    $${PWD}/MoneyAmount.cpp \
    $${PWD}/Operation.cpp \
    $${PWD}/OperationStatus.cpp \
    $${PWD}/OperationTrade.cpp \
    $${PWD}/OperationType.cpp \
    $${PWD}/OperationTypeWithCommission.cpp \
    $${PWD}/Operations.cpp \
    $${PWD}/OperationsResponse.cpp \
    $${PWD}/Order.cpp \
    $${PWD}/OrderResponse.cpp \
    $${PWD}/OrderStatus.cpp \
    $${PWD}/OrderType.cpp \
    $${PWD}/Orderbook.cpp \
    $${PWD}/OrderbookResponse.cpp \
    $${PWD}/OrdersResponse.cpp \
    $${PWD}/PlacedLimitOrder.cpp \
    $${PWD}/PlacedMarketOrder.cpp \
    $${PWD}/Portfolio.cpp \
    $${PWD}/PortfolioCurrenciesResponse.cpp \
    $${PWD}/PortfolioPosition.cpp \
    $${PWD}/PortfolioResponse.cpp \
    $${PWD}/SandboxAccount.cpp \
    $${PWD}/SandboxCurrency.cpp \
    $${PWD}/SandboxRegisterRequest.cpp \
    $${PWD}/SandboxRegisterResponse.cpp \
    $${PWD}/SandboxSetCurrencyBalanceRequest.cpp \
    $${PWD}/SandboxSetPositionBalanceRequest.cpp \
    $${PWD}/SearchMarketInstrument.cpp \
    $${PWD}/SearchMarketInstrumentResponse.cpp \
    $${PWD}/TradeStatus.cpp \
    $${PWD}/UserAccount.cpp \
    $${PWD}/UserAccounts.cpp \
    $${PWD}/UserAccountsResponse.cpp \
# APIs
    $${PWD}/MarketApi.cpp \
    $${PWD}/OperationsApi.cpp \
    $${PWD}/OrdersApi.cpp \
    $${PWD}/PortfolioApi.cpp \
    $${PWD}/SandboxApi.cpp \
    $${PWD}/UserApi.cpp \
# Others
    $${PWD}/Helpers.cpp \
    $${PWD}/HttpRequest.cpp \
    $${PWD}/HttpFileElement.cpp

