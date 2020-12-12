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

#include "QTkfInvestOaCandlesResponse.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "QTkfInvestOaHelpers.h"

namespace OpenAPI {

QTkfInvestOaCandlesResponse::QTkfInvestOaCandlesResponse(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

QTkfInvestOaCandlesResponse::QTkfInvestOaCandlesResponse() {
    this->initializeModel();
}

QTkfInvestOaCandlesResponse::~QTkfInvestOaCandlesResponse() {}

void QTkfInvestOaCandlesResponse::initializeModel() {

    m_tracking_id_isSet = false;
    m_tracking_id_isValid = false;

    m_status_isSet = false;
    m_status_isValid = false;

    m_payload_isSet = false;
    m_payload_isValid = false;
}

void QTkfInvestOaCandlesResponse::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void QTkfInvestOaCandlesResponse::fromJsonObject(QJsonObject json) {

    m_tracking_id_isValid = ::OpenAPI::fromJsonValue(tracking_id, json[QString("trackingId")]);
    m_tracking_id_isSet = !json[QString("trackingId")].isNull() && m_tracking_id_isValid;

    m_status_isValid = ::OpenAPI::fromJsonValue(status, json[QString("status")]);
    m_status_isSet = !json[QString("status")].isNull() && m_status_isValid;

    m_payload_isValid = ::OpenAPI::fromJsonValue(payload, json[QString("payload")]);
    m_payload_isSet = !json[QString("payload")].isNull() && m_payload_isValid;
}

QString QTkfInvestOaCandlesResponse::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject QTkfInvestOaCandlesResponse::asJsonObject() const {
    QJsonObject obj;
    if (m_tracking_id_isSet) {
        obj.insert(QString("trackingId"), ::OpenAPI::toJsonValue(tracking_id));
    }
    if (m_status_isSet) {
        obj.insert(QString("status"), ::OpenAPI::toJsonValue(status));
    }
    if (payload.isSet()) {
        obj.insert(QString("payload"), ::OpenAPI::toJsonValue(payload));
    }
    return obj;
}

QString QTkfInvestOaCandlesResponse::getTrackingId() const {
    return tracking_id;
}
void QTkfInvestOaCandlesResponse::setTrackingId(const QString &tracking_id) {
    this->tracking_id = tracking_id;
    this->m_tracking_id_isSet = true;
}

bool QTkfInvestOaCandlesResponse::is_tracking_id_Set() const{
    return m_tracking_id_isSet;
}

bool QTkfInvestOaCandlesResponse::is_tracking_id_Valid() const{
    return m_tracking_id_isValid;
}

QString QTkfInvestOaCandlesResponse::getStatus() const {
    return status;
}
void QTkfInvestOaCandlesResponse::setStatus(const QString &status) {
    this->status = status;
    this->m_status_isSet = true;
}

bool QTkfInvestOaCandlesResponse::is_status_Set() const{
    return m_status_isSet;
}

bool QTkfInvestOaCandlesResponse::is_status_Valid() const{
    return m_status_isValid;
}

QTkfInvestOaCandles QTkfInvestOaCandlesResponse::getPayload() const {
    return payload;
}
void QTkfInvestOaCandlesResponse::setPayload(const QTkfInvestOaCandles &payload) {
    this->payload = payload;
    this->m_payload_isSet = true;
}

bool QTkfInvestOaCandlesResponse::is_payload_Set() const{
    return m_payload_isSet;
}

bool QTkfInvestOaCandlesResponse::is_payload_Valid() const{
    return m_payload_isValid;
}

bool QTkfInvestOaCandlesResponse::isSet() const {
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

        if (payload.isSet()) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool QTkfInvestOaCandlesResponse::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_tracking_id_isValid && m_status_isValid && m_payload_isValid && true;
}

} // namespace OpenAPI
