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

#include "OrderResponse.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "Helpers.h"

namespace OpenAPI {

OrderResponse::OrderResponse(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OrderResponse::OrderResponse() {
    this->initializeModel();
}

OrderResponse::~OrderResponse() {}

void OrderResponse::initializeModel() {

    m_price_isSet = false;
    m_price_isValid = false;

    m_quantity_isSet = false;
    m_quantity_isValid = false;
}

void OrderResponse::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OrderResponse::fromJsonObject(QJsonObject json) {

    m_price_isValid = ::OpenAPI::fromJsonValue(price, json[QString("price")]);
    m_price_isSet = !json[QString("price")].isNull() && m_price_isValid;

    m_quantity_isValid = ::OpenAPI::fromJsonValue(quantity, json[QString("quantity")]);
    m_quantity_isSet = !json[QString("quantity")].isNull() && m_quantity_isValid;
}

QString OrderResponse::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OrderResponse::asJsonObject() const {
    QJsonObject obj;
    if (m_price_isSet) {
        obj.insert(QString("price"), ::OpenAPI::toJsonValue(price));
    }
    if (m_quantity_isSet) {
        obj.insert(QString("quantity"), ::OpenAPI::toJsonValue(quantity));
    }
    return obj;
}

marty::Decimal OrderResponse::getPrice() const {
    return price;
}
void OrderResponse::setPrice(const marty::Decimal &price) {
    this->price = price;
    this->m_price_isSet = true;
}

bool OrderResponse::is_price_Set() const{
    return m_price_isSet;
}

bool OrderResponse::is_price_Valid() const{
    return m_price_isValid;
}

qint32 OrderResponse::getQuantity() const {
    return quantity;
}
void OrderResponse::setQuantity(const qint32 &quantity) {
    this->quantity = quantity;
    this->m_quantity_isSet = true;
}

bool OrderResponse::is_quantity_Set() const{
    return m_quantity_isSet;
}

bool OrderResponse::is_quantity_Valid() const{
    return m_quantity_isValid;
}

bool OrderResponse::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_price_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_quantity_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OrderResponse::isValid() const {
    // only required properties are required for the object to be considered valid
    return m_price_isValid && m_quantity_isValid && true;
}

} // namespace OpenAPI
