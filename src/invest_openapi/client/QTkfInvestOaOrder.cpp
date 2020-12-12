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

#include "QTkfInvestOaOrder.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "QTkfInvestOaHelpers.h"

namespace OpenAPI {

QTkfInvestOaOrder::QTkfInvestOaOrder(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

QTkfInvestOaOrder::QTkfInvestOaOrder() {
    this->initializeModel();
}

QTkfInvestOaOrder::~QTkfInvestOaOrder() {}

void QTkfInvestOaOrder::initializeModel() {

    m_order_id_isSet = false;
    m_order_id_isValid = false;

    m_figi_isSet = false;
    m_figi_isValid = false;

    m_operation_isSet = false;
    m_operation_isValid = false;

    m_status_isSet = false;
    m_status_isValid = false;

    m_requested_lots_isSet = false;
    m_requested_lots_isValid = false;

    m_executed_lots_isSet = false;
    m_executed_lots_isValid = false;

    m_type_isSet = false;
    m_type_isValid = false;

    m_price_isSet = false;
    m_price_isValid = false;
}

void QTkfInvestOaOrder::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void QTkfInvestOaOrder::fromJsonObject(QJsonObject json) {

    m_order_id_isValid = ::OpenAPI::fromJsonValue(order_id, json[QString("orderId")]);
    m_order_id_isSet = !json[QString("orderId")].isNull() && m_order_id_isValid;

    m_figi_isValid = ::OpenAPI::fromJsonValue(figi, json[QString("figi")]);
    m_figi_isSet = !json[QString("figi")].isNull() && m_figi_isValid;

    m_operation_isValid = ::OpenAPI::fromJsonValue(operation, json[QString("operation")]);
    m_operation_isSet = !json[QString("operation")].isNull() && m_operation_isValid;

    m_status_isValid = ::OpenAPI::fromJsonValue(status, json[QString("status")]);
    m_status_isSet = !json[QString("status")].isNull() && m_status_isValid;

    m_requested_lots_isValid = ::OpenAPI::fromJsonValue(requested_lots, json[QString("requestedLots")]);
    m_requested_lots_isSet = !json[QString("requestedLots")].isNull() && m_requested_lots_isValid;

    m_executed_lots_isValid = ::OpenAPI::fromJsonValue(executed_lots, json[QString("executedLots")]);
    m_executed_lots_isSet = !json[QString("executedLots")].isNull() && m_executed_lots_isValid;

    m_type_isValid = ::OpenAPI::fromJsonValue(type, json[QString("type")]);
    m_type_isSet = !json[QString("type")].isNull() && m_type_isValid;

    m_price_isValid = ::OpenAPI::fromJsonValue(price, json[QString("price")]);
    m_price_isSet = !json[QString("price")].isNull() && m_price_isValid;
}

QString QTkfInvestOaOrder::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject QTkfInvestOaOrder::asJsonObject() const {
    QJsonObject obj;
    if (m_order_id_isSet) {
        obj.insert(QString("orderId"), ::OpenAPI::toJsonValue(order_id));
    }
    if (m_figi_isSet) {
        obj.insert(QString("figi"), ::OpenAPI::toJsonValue(figi));
    }
    if (operation.isSet()) {
        obj.insert(QString("operation"), ::OpenAPI::toJsonValue(operation));
    }
    if (status.isSet()) {
        obj.insert(QString("status"), ::OpenAPI::toJsonValue(status));
    }
    if (m_requested_lots_isSet) {
        obj.insert(QString("requestedLots"), ::OpenAPI::toJsonValue(requested_lots));
    }
    if (m_executed_lots_isSet) {
        obj.insert(QString("executedLots"), ::OpenAPI::toJsonValue(executed_lots));
    }
    if (type.isSet()) {
        obj.insert(QString("type"), ::OpenAPI::toJsonValue(type));
    }
    if (m_price_isSet) {
        obj.insert(QString("price"), ::OpenAPI::toJsonValue(price));
    }
    return obj;
}

QString QTkfInvestOaOrder::getOrderId() const {
    return order_id;
}
void QTkfInvestOaOrder::setOrderId(const QString &order_id) {
    this->order_id = order_id;
    this->m_order_id_isSet = true;
}

bool QTkfInvestOaOrder::is_order_id_Set() const{
    return m_order_id_isSet;
}

bool QTkfInvestOaOrder::is_order_id_Valid() const{
    return m_order_id_isValid;
}

QString QTkfInvestOaOrder::getFigi() const {
    return figi;
}
void QTkfInvestOaOrder::setFigi(const QString &figi) {
    this->figi = figi;
    this->m_figi_isSet = true;
}

bool QTkfInvestOaOrder::is_figi_Set() const{
    return m_figi_isSet;
}

bool QTkfInvestOaOrder::is_figi_Valid() const{
    return m_figi_isValid;
}

QTkfInvestOaOperationType QTkfInvestOaOrder::getOperation() const {
    return operation;
}
void QTkfInvestOaOrder::setOperation(const QTkfInvestOaOperationType &operation) {
    this->operation = operation;
    this->m_operation_isSet = true;
}

bool QTkfInvestOaOrder::is_operation_Set() const{
    return m_operation_isSet;
}

bool QTkfInvestOaOrder::is_operation_Valid() const{
    return m_operation_isValid;
}

QTkfInvestOaOrderStatus QTkfInvestOaOrder::getStatus() const {
    return status;
}
void QTkfInvestOaOrder::setStatus(const QTkfInvestOaOrderStatus &status) {
    this->status = status;
    this->m_status_isSet = true;
}

bool QTkfInvestOaOrder::is_status_Set() const{
    return m_status_isSet;
}

bool QTkfInvestOaOrder::is_status_Valid() const{
    return m_status_isValid;
}

qint32 QTkfInvestOaOrder::getRequestedLots() const {
    return requested_lots;
}
void QTkfInvestOaOrder::setRequestedLots(const qint32 &requested_lots) {
    this->requested_lots = requested_lots;
    this->m_requested_lots_isSet = true;
}

bool QTkfInvestOaOrder::is_requested_lots_Set() const{
    return m_requested_lots_isSet;
}

bool QTkfInvestOaOrder::is_requested_lots_Valid() const{
    return m_requested_lots_isValid;
}

qint32 QTkfInvestOaOrder::getExecutedLots() const {
    return executed_lots;
}
void QTkfInvestOaOrder::setExecutedLots(const qint32 &executed_lots) {
    this->executed_lots = executed_lots;
    this->m_executed_lots_isSet = true;
}

bool QTkfInvestOaOrder::is_executed_lots_Set() const{
    return m_executed_lots_isSet;
}

bool QTkfInvestOaOrder::is_executed_lots_Valid() const{
    return m_executed_lots_isValid;
}

QTkfInvestOaOrderType QTkfInvestOaOrder::getType() const {
    return type;
}
void QTkfInvestOaOrder::setType(const QTkfInvestOaOrderType &type) {
    this->type = type;
    this->m_type_isSet = true;
}

bool QTkfInvestOaOrder::is_type_Set() const{
    return m_type_isSet;
}

bool QTkfInvestOaOrder::is_type_Valid() const{
    return m_type_isValid;
}

double QTkfInvestOaOrder::getPrice() const {
    return price;
}
void QTkfInvestOaOrder::setPrice(const double &price) {
    this->price = price;
    this->m_price_isSet = true;
}

bool QTkfInvestOaOrder::is_price_Set() const{
    return m_price_isSet;
}

bool QTkfInvestOaOrder::is_price_Valid() const{
    return m_price_isValid;
}

bool QTkfInvestOaOrder::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_order_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_figi_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (operation.isSet()) {
            isObjectUpdated = true;
            break;
        }

        if (status.isSet()) {
            isObjectUpdated = true;
            break;
        }

        if (m_requested_lots_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_executed_lots_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (type.isSet()) {
            isObjectUpdated = true;
            break;
        }

        if (m_price_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool QTkfInvestOaOrder::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_order_id_isValid && m_figi_isValid && m_operation_isValid && m_status_isValid && m_requested_lots_isValid && m_executed_lots_isValid && m_type_isValid && m_price_isValid && true;
}

} // namespace OpenAPI
