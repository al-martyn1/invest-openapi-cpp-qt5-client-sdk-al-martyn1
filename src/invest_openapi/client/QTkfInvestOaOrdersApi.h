/**
 * OpenAPI
 * tinkoff.ru/invest OpenAPI.
 *
 * The version of the OpenAPI document: 1.0.0
 * Contact: n.v.melnikov@tinkoff.ru
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#ifndef QTkfInvestOa_QTkfInvestOaOrdersApi_H
#define QTkfInvestOa_QTkfInvestOaOrdersApi_H

#include "QTkfInvestOaHttpRequest.h"

#include "QTkfInvestOaEmpty.h"
#include "QTkfInvestOaError.h"
#include "QTkfInvestOaLimitOrderRequest.h"
#include "QTkfInvestOaLimitOrderResponse.h"
#include "QTkfInvestOaMarketOrderRequest.h"
#include "QTkfInvestOaMarketOrderResponse.h"
#include "QTkfInvestOaOrdersResponse.h"
#include <QString>

#include <QObject>

namespace OpenAPI {

class QTkfInvestOaOrdersApi : public QObject {
    Q_OBJECT

public:
    QTkfInvestOaOrdersApi(const QString &scheme = "https", const QString &host = "api-invest.tinkoff.ru", int port = 0, const QString &basePath = "/openapi", const int timeOut = 0);
    ~QTkfInvestOaOrdersApi();

    void setScheme(const QString &scheme);
    void setHost(const QString &host);
    void setPort(int port);
    void setBasePath(const QString &basePath);
    void setTimeOut(const int timeOut);
    void setWorkingDirectory(const QString &path);
    void addHeaders(const QString &key, const QString &value);
    void enableRequestCompression();
    void enableResponseCompression();
    void abortRequests();

    void ordersCancelPost(const QString &order_id, const QString &broker_account_id);
    void ordersGet(const QString &broker_account_id);
    void ordersLimitOrderPost(const QString &figi, const QTkfInvestOaLimitOrderRequest &q_tkf_invest_oa_limit_order_request, const QString &broker_account_id);
    void ordersMarketOrderPost(const QString &figi, const QTkfInvestOaMarketOrderRequest &q_tkf_invest_oa_market_order_request, const QString &broker_account_id);

private:
    QString _scheme, _host;
    int _port;
    QString _basePath;
    int _timeOut;
    QString _workingDirectory;
    QMap<QString, QString> defaultHeaders;
    bool isResponseCompressionEnabled;
    bool isRequestCompressionEnabled;

    void ordersCancelPostCallback(QTkfInvestOaHttpRequestWorker *worker);
    void ordersGetCallback(QTkfInvestOaHttpRequestWorker *worker);
    void ordersLimitOrderPostCallback(QTkfInvestOaHttpRequestWorker *worker);
    void ordersMarketOrderPostCallback(QTkfInvestOaHttpRequestWorker *worker);

signals:

    void ordersCancelPostSignal(QTkfInvestOaEmpty summary);
    void ordersGetSignal(QTkfInvestOaOrdersResponse summary);
    void ordersLimitOrderPostSignal(QTkfInvestOaLimitOrderResponse summary);
    void ordersMarketOrderPostSignal(QTkfInvestOaMarketOrderResponse summary);

    void ordersCancelPostSignalFull(QTkfInvestOaHttpRequestWorker *worker, QTkfInvestOaEmpty summary);
    void ordersGetSignalFull(QTkfInvestOaHttpRequestWorker *worker, QTkfInvestOaOrdersResponse summary);
    void ordersLimitOrderPostSignalFull(QTkfInvestOaHttpRequestWorker *worker, QTkfInvestOaLimitOrderResponse summary);
    void ordersMarketOrderPostSignalFull(QTkfInvestOaHttpRequestWorker *worker, QTkfInvestOaMarketOrderResponse summary);

    void ordersCancelPostSignalE(QTkfInvestOaEmpty summary, QNetworkReply::NetworkError error_type, QString error_str);
    void ordersGetSignalE(QTkfInvestOaOrdersResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void ordersLimitOrderPostSignalE(QTkfInvestOaLimitOrderResponse summary, QNetworkReply::NetworkError error_type, QString error_str);
    void ordersMarketOrderPostSignalE(QTkfInvestOaMarketOrderResponse summary, QNetworkReply::NetworkError error_type, QString error_str);

    void ordersCancelPostSignalEFull(QTkfInvestOaHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void ordersGetSignalEFull(QTkfInvestOaHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void ordersLimitOrderPostSignalEFull(QTkfInvestOaHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void ordersMarketOrderPostSignalEFull(QTkfInvestOaHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);

    void abortRequestsSignal(); 
};

} // namespace OpenAPI
#endif
