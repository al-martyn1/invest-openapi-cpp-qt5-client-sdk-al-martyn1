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
 * QTkfInvestOaMarketInstrumentListResponse.h
 *
 * 
 */

#ifndef QTkfInvestOaMarketInstrumentListResponse_H
#define QTkfInvestOaMarketInstrumentListResponse_H

#include <QJsonObject>

#include "QTkfInvestOaMarketInstrumentList.h"
#include <QString>

#include "QTkfInvestOaEnum.h"
#include "QTkfInvestOaObject.h"

namespace OpenAPI {

class QTkfInvestOaMarketInstrumentListResponse : public QTkfInvestOaObject {
public:
    QTkfInvestOaMarketInstrumentListResponse();
    QTkfInvestOaMarketInstrumentListResponse(QString json);
    ~QTkfInvestOaMarketInstrumentListResponse() override;

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

    QTkfInvestOaMarketInstrumentList getPayload() const;
    void setPayload(const QTkfInvestOaMarketInstrumentList &payload);
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

    QTkfInvestOaMarketInstrumentList payload;
    bool m_payload_isSet;
    bool m_payload_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::QTkfInvestOaMarketInstrumentListResponse)

#endif // QTkfInvestOaMarketInstrumentListResponse_H
