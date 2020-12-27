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

#include "SandboxSetPositionBalanceRequest.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "Helpers.h"

namespace OpenAPI {

SandboxSetPositionBalanceRequest::SandboxSetPositionBalanceRequest(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

SandboxSetPositionBalanceRequest::SandboxSetPositionBalanceRequest() {
    this->initializeModel();
}

SandboxSetPositionBalanceRequest::~SandboxSetPositionBalanceRequest() {}

void SandboxSetPositionBalanceRequest::initializeModel() {

    m_figi_isSet = false;
    m_figi_isValid = false;

    m_balance_isSet = false;
    m_balance_isValid = false;
}

void SandboxSetPositionBalanceRequest::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void SandboxSetPositionBalanceRequest::fromJsonObject(QJsonObject json) {

    m_figi_isValid = ::OpenAPI::fromJsonValue(figi, json[QString("figi")]);
    m_figi_isSet = !json[QString("figi")].isNull() && m_figi_isValid;

    m_balance_isValid = ::OpenAPI::fromJsonValue(balance, json[QString("balance")]);
    m_balance_isSet = !json[QString("balance")].isNull() && m_balance_isValid;
}

QString SandboxSetPositionBalanceRequest::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject SandboxSetPositionBalanceRequest::asJsonObject() const {
    QJsonObject obj;
    if (m_figi_isSet) {
        obj.insert(QString("figi"), ::OpenAPI::toJsonValue(figi));
    }
    if (m_balance_isSet) {
        obj.insert(QString("balance"), ::OpenAPI::toJsonValue(balance));
    }
    return obj;
}

QString SandboxSetPositionBalanceRequest::getFigi() const {
    return figi;
}
void SandboxSetPositionBalanceRequest::setFigi(const QString &figi) {
    this->figi = figi;
    this->m_figi_isSet = true;
}

bool SandboxSetPositionBalanceRequest::is_figi_Set() const{
    return m_figi_isSet;
}

bool SandboxSetPositionBalanceRequest::is_figi_Valid() const{
    return m_figi_isValid;
}

marty::Decimal SandboxSetPositionBalanceRequest::getBalance() const {
    return balance;
}
void SandboxSetPositionBalanceRequest::setBalance(const marty::Decimal &balance) {
    this->balance = balance;
    this->m_balance_isSet = true;
}

bool SandboxSetPositionBalanceRequest::is_balance_Set() const{
    return m_balance_isSet;
}

bool SandboxSetPositionBalanceRequest::is_balance_Valid() const{
    return m_balance_isValid;
}

bool SandboxSetPositionBalanceRequest::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_figi_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_balance_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool SandboxSetPositionBalanceRequest::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_balance_isValid && true;
}

} // namespace OpenAPI
