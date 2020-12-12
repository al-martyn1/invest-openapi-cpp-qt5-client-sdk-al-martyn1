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

#include "QTkfInvestOaUserAccount.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "QTkfInvestOaHelpers.h"

namespace OpenAPI {

QTkfInvestOaUserAccount::QTkfInvestOaUserAccount(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

QTkfInvestOaUserAccount::QTkfInvestOaUserAccount() {
    this->initializeModel();
}

QTkfInvestOaUserAccount::~QTkfInvestOaUserAccount() {}

void QTkfInvestOaUserAccount::initializeModel() {

    m_broker_account_type_isSet = false;
    m_broker_account_type_isValid = false;

    m_broker_account_id_isSet = false;
    m_broker_account_id_isValid = false;
}

void QTkfInvestOaUserAccount::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void QTkfInvestOaUserAccount::fromJsonObject(QJsonObject json) {

    m_broker_account_type_isValid = ::OpenAPI::fromJsonValue(broker_account_type, json[QString("brokerAccountType")]);
    m_broker_account_type_isSet = !json[QString("brokerAccountType")].isNull() && m_broker_account_type_isValid;

    m_broker_account_id_isValid = ::OpenAPI::fromJsonValue(broker_account_id, json[QString("brokerAccountId")]);
    m_broker_account_id_isSet = !json[QString("brokerAccountId")].isNull() && m_broker_account_id_isValid;
}

QString QTkfInvestOaUserAccount::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject QTkfInvestOaUserAccount::asJsonObject() const {
    QJsonObject obj;
    if (broker_account_type.isSet()) {
        obj.insert(QString("brokerAccountType"), ::OpenAPI::toJsonValue(broker_account_type));
    }
    if (m_broker_account_id_isSet) {
        obj.insert(QString("brokerAccountId"), ::OpenAPI::toJsonValue(broker_account_id));
    }
    return obj;
}

QTkfInvestOaBrokerAccountType QTkfInvestOaUserAccount::getBrokerAccountType() const {
    return broker_account_type;
}
void QTkfInvestOaUserAccount::setBrokerAccountType(const QTkfInvestOaBrokerAccountType &broker_account_type) {
    this->broker_account_type = broker_account_type;
    this->m_broker_account_type_isSet = true;
}

bool QTkfInvestOaUserAccount::is_broker_account_type_Set() const{
    return m_broker_account_type_isSet;
}

bool QTkfInvestOaUserAccount::is_broker_account_type_Valid() const{
    return m_broker_account_type_isValid;
}

QString QTkfInvestOaUserAccount::getBrokerAccountId() const {
    return broker_account_id;
}
void QTkfInvestOaUserAccount::setBrokerAccountId(const QString &broker_account_id) {
    this->broker_account_id = broker_account_id;
    this->m_broker_account_id_isSet = true;
}

bool QTkfInvestOaUserAccount::is_broker_account_id_Set() const{
    return m_broker_account_id_isSet;
}

bool QTkfInvestOaUserAccount::is_broker_account_id_Valid() const{
    return m_broker_account_id_isValid;
}

bool QTkfInvestOaUserAccount::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (broker_account_type.isSet()) {
            isObjectUpdated = true;
            break;
        }

        if (m_broker_account_id_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool QTkfInvestOaUserAccount::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_broker_account_type_isValid && m_broker_account_id_isValid && true;
}

} // namespace OpenAPI
