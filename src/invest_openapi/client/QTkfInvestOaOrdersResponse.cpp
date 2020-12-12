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

#include "QTkfInvestOaOrdersResponse.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "QTkfInvestOaHelpers.h"

namespace OpenAPI {

QTkfInvestOaOrdersResponse::QTkfInvestOaOrdersResponse(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

QTkfInvestOaOrdersResponse::QTkfInvestOaOrdersResponse() {
    this->initializeModel();
}

QTkfInvestOaOrdersResponse::~QTkfInvestOaOrdersResponse() {}

void QTkfInvestOaOrdersResponse::initializeModel() {

    m_tracking_id_isSet = false;
    m_tracking_id_isValid = false;

    m_status_isSet = false;
    m_status_isValid = false;

    m_payload_isSet = false;
    m_payload_isValid = false;
}

void QTkfInvestOaOrdersResponse::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void QTkfInvestOaOrdersResponse::fromJsonObject(QJsonObject json) {

    m_tracking_id_isValid = ::OpenAPI::fromJsonValue(tracking_id, json[QString("trackingId")]);
    m_tracking_id_isSet = !json[QString("trackingId")].isNull() && m_tracking_id_isValid;

    m_status_isValid = ::OpenAPI::fromJsonValue(status, json[QString("status")]);
    m_status_isSet = !json[QString("status")].isNull() && m_status_isValid;

    m_payload_isValid = ::OpenAPI::fromJsonValue(payload, json[QString("payload")]);
    m_payload_isSet = !json[QString("payload")].isNull() && m_payload_isValid;
}

QString QTkfInvestOaOrdersResponse::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject QTkfInvestOaOrdersResponse::asJsonObject() const {
    QJsonObject obj;
    if (m_tracking_id_isSet) {
        obj.insert(QString("trackingId"), ::OpenAPI::toJsonValue(tracking_id));
    }
    if (m_status_isSet) {
        obj.insert(QString("status"), ::OpenAPI::toJsonValue(status));
    }
    if (payload.size() > 0) {
        obj.insert(QString("payload"), ::OpenAPI::toJsonValue(payload));
    }
    return obj;
}

QString QTkfInvestOaOrdersResponse::getTrackingId() const {
    return tracking_id;
}
void QTkfInvestOaOrdersResponse::setTrackingId(const QString &tracking_id) {
    this->tracking_id = tracking_id;
    this->m_tracking_id_isSet = true;
}

bool QTkfInvestOaOrdersResponse::is_tracking_id_Set() const{
    return m_tracking_id_isSet;
}

bool QTkfInvestOaOrdersResponse::is_tracking_id_Valid() const{
    return m_tracking_id_isValid;
}

QString QTkfInvestOaOrdersResponse::getStatus() const {
    return status;
}
void QTkfInvestOaOrdersResponse::setStatus(const QString &status) {
    this->status = status;
    this->m_status_isSet = true;
}

bool QTkfInvestOaOrdersResponse::is_status_Set() const{
    return m_status_isSet;
}

bool QTkfInvestOaOrdersResponse::is_status_Valid() const{
    return m_status_isValid;
}

QList<QTkfInvestOaOrder> QTkfInvestOaOrdersResponse::getPayload() const {
    return payload;
}
void QTkfInvestOaOrdersResponse::setPayload(const QList<QTkfInvestOaOrder> &payload) {
    this->payload = payload;
    this->m_payload_isSet = true;
}

bool QTkfInvestOaOrdersResponse::is_payload_Set() const{
    return m_payload_isSet;
}

bool QTkfInvestOaOrdersResponse::is_payload_Valid() const{
    return m_payload_isValid;
}

bool QTkfInvestOaOrdersResponse::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_tracking_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_status_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (payload.size() > 0) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool QTkfInvestOaOrdersResponse::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_tracking_id_isValid && m_status_isValid && m_payload_isValid && true;
}

} // namespace OpenAPI
