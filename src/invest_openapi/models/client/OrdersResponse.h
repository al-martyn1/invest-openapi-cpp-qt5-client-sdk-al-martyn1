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

/*
 * OrdersResponse.h
 *
 * 
 */

#ifndef OrdersResponse_H
#define OrdersResponse_H

#include <QJsonObject>

#include "Order.h"
#include <QList>
#include <QString>

#include "Enum.h"
#include "Object.h"

namespace OpenAPI {

class OrdersResponse : public Object {
public:
    OrdersResponse();
    OrdersResponse(QString json);
    ~OrdersResponse() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getTrackingId() const;
    void setTrackingId(const QString &tracking_id);
    bool is_tracking_id_Set() const;
    bool is_tracking_id_Valid() const;

    QString getStatus() const;
    void setStatus(const QString &status);
    bool is_status_Set() const;
    bool is_status_Valid() const;

    QList<Order> getPayload() const;
    void setPayload(const QList<Order> &payload);
    bool is_payload_Set() const;
    bool is_payload_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString tracking_id;
    bool m_tracking_id_isSet;
    bool m_tracking_id_isValid;

    QString status;
    bool m_status_isSet;
    bool m_status_isValid;

    QList<Order> payload;
    bool m_payload_isSet;
    bool m_payload_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OrdersResponse)

#endif // OrdersResponse_H
