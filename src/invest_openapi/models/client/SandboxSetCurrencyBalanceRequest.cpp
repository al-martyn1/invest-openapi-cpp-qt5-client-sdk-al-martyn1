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

#include "SandboxSetCurrencyBalanceRequest.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "Helpers.h"

namespace OpenAPI {

SandboxSetCurrencyBalanceRequest::SandboxSetCurrencyBalanceRequest(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

SandboxSetCurrencyBalanceRequest::SandboxSetCurrencyBalanceRequest() {
    this->initializeModel();
}

SandboxSetCurrencyBalanceRequest::~SandboxSetCurrencyBalanceRequest() {}

void SandboxSetCurrencyBalanceRequest::initializeModel() {

    m_currency_isSet = false;
    m_currency_isValid = false;

    m_balance_isSet = false;
    m_balance_isValid = false;
}

void SandboxSetCurrencyBalanceRequest::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void SandboxSetCurrencyBalanceRequest::fromJsonObject(QJsonObject json) {

    m_currency_isValid = ::OpenAPI::fromJsonValue(currency, json[QString("currency")]);
    m_currency_isSet = !json[QString("currency")].isNull() && m_currency_isValid;

    m_balance_isValid = ::OpenAPI::fromJsonValue(balance, json[QString("balance")]);
    m_balance_isSet = !json[QString("balance")].isNull() && m_balance_isValid;
}

QString SandboxSetCurrencyBalanceRequest::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject SandboxSetCurrencyBalanceRequest::asJsonObject() const {
    QJsonObject obj;
    if (currency.isSet()) {
        obj.insert(QString("currency"), ::OpenAPI::toJsonValue(currency));
    }
    if (m_balance_isSet) {
        obj.insert(QString("balance"), ::OpenAPI::toJsonValue(balance));
    }
    return obj;
}

SandboxCurrency SandboxSetCurrencyBalanceRequest::getCurrency() const {
    return currency;
}
void SandboxSetCurrencyBalanceRequest::setCurrency(const SandboxCurrency &currency) {
    this->currency = currency;
    this->m_currency_isSet = true;
}

bool SandboxSetCurrencyBalanceRequest::is_currency_Set() const{
    return m_currency_isSet;
}

bool SandboxSetCurrencyBalanceRequest::is_currency_Valid() const{
    return m_currency_isValid;
}

marty::Decimal SandboxSetCurrencyBalanceRequest::getBalance() const {
    return balance;
}
void SandboxSetCurrencyBalanceRequest::setBalance(const marty::Decimal &balance) {
    this->balance = balance;
    this->m_balance_isSet = true;
}

bool SandboxSetCurrencyBalanceRequest::is_balance_Set() const{
    return m_balance_isSet;
}

bool SandboxSetCurrencyBalanceRequest::is_balance_Valid() const{
    return m_balance_isValid;
}

bool SandboxSetCurrencyBalanceRequest::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (currency.isSet()) {
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

bool SandboxSetCurrencyBalanceRequest::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_currency_isValid && m_balance_isValid && true;
}

} // namespace OpenAPI
