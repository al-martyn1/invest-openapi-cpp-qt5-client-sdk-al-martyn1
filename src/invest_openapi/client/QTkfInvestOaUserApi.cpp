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

#include "QTkfInvestOaUserApi.h"
#include "QTkfInvestOaHelpers.h"

#include <QJsonArray>
#include <QJsonDocument>

namespace OpenAPI {

QTkfInvestOaUserApi::QTkfInvestOaUserApi(const QString &scheme, const QString &host, int port, const QString &basePath, const int timeOut)
    : _scheme(scheme),
      _host(host),
      _port(port),
      _basePath(basePath),
      _timeOut(timeOut),
      isResponseCompressionEnabled(false),
      isRequestCompressionEnabled(false) {}

QTkfInvestOaUserApi::~QTkfInvestOaUserApi() {
}

void QTkfInvestOaUserApi::setScheme(const QString &scheme) {
    _scheme = scheme;
}

void QTkfInvestOaUserApi::setHost(const QString &host) {
    _host = host;
}

void QTkfInvestOaUserApi::setPort(int port) {
    _port = port;
}

void QTkfInvestOaUserApi::setBasePath(const QString &basePath) {
    _basePath = basePath;
}

void QTkfInvestOaUserApi::setTimeOut(const int timeOut) {
    _timeOut = timeOut;
}

void QTkfInvestOaUserApi::setWorkingDirectory(const QString &path) {
    _workingDirectory = path;
}

void QTkfInvestOaUserApi::addHeaders(const QString &key, const QString &value) {
    defaultHeaders.insert(key, value);
}

void QTkfInvestOaUserApi::enableRequestCompression() {
    isRequestCompressionEnabled = true;
}

void QTkfInvestOaUserApi::enableResponseCompression() {
    isResponseCompressionEnabled = true;
}

void QTkfInvestOaUserApi::abortRequests(){
    emit abortRequestsSignal();
}

void QTkfInvestOaUserApi::userAccountsGet() {
    QString fullPath = QString("%1://%2%3%4%5")
                           .arg(_scheme)
                           .arg(_host)
                           .arg(_port ? ":" + QString::number(_port) : "")
                           .arg(_basePath)
                           .arg("/user/accounts");

    QTkfInvestOaHttpRequestWorker *worker = new QTkfInvestOaHttpRequestWorker(this);
    worker->setTimeOut(_timeOut);
    worker->setWorkingDirectory(_workingDirectory);
    QTkfInvestOaHttpRequestInput input(fullPath, "GET");

    foreach (QString key, this->defaultHeaders.keys()) { input.headers.insert(key, this->defaultHeaders.value(key)); }

    connect(worker, &QTkfInvestOaHttpRequestWorker::on_execution_finished, this, &QTkfInvestOaUserApi::userAccountsGetCallback);
    connect(this, &QTkfInvestOaUserApi::abortRequestsSignal, worker, &QObject::deleteLater); 
    worker->execute(&input);
}

void QTkfInvestOaUserApi::userAccountsGetCallback(QTkfInvestOaHttpRequestWorker *worker) {
    QString msg;
    QString error_str = worker->error_str;
    QNetworkReply::NetworkError error_type = worker->error_type;

    if (worker->error_type == QNetworkReply::NoError) {
        msg = QString("Success! %1 bytes").arg(worker->response.length());
    } else {
        msg = "Error: " + worker->error_str;
        error_str = QString("%1, %2").arg(worker->error_str).arg(QString(worker->response));
    }
    QTkfInvestOaUserAccountsResponse output(QString(worker->response));
    worker->deleteLater();

    if (worker->error_type == QNetworkReply::NoError) {
        emit userAccountsGetSignal(output);
        emit userAccountsGetSignalFull(worker, output);
    } else {
        emit userAccountsGetSignalE(output, error_type, error_str);
        emit userAccountsGetSignalEFull(worker, error_type, error_str);
    }
}

} // namespace OpenAPI
