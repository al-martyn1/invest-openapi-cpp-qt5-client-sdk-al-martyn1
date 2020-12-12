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
 * QTkfInvestOaMarketOrderRequest.h
 *
 * 
 */

#ifndef QTkfInvestOaMarketOrderRequest_H
#define QTkfInvestOaMarketOrderRequest_H

#include <QJsonObject>

#include "QTkfInvestOaOperationType.h"

#include "QTkfInvestOaEnum.h"
#include "QTkfInvestOaObject.h"

namespace OpenAPI {

class QTkfInvestOaMarketOrderRequest : public QTkfInvestOaObject {
public:
    QTkfInvestOaMarketOrderRequest();
    QTkfInvestOaMarketOrderRequest(QString json);
    ~QTkfInvestOaMarketOrderRequest() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    qint32 getLots() const;
    void setLots(const qint32 &lots);
    bool is_lots_Set() const;
    bool is_lots_Valid() const;

    QTkfInvestOaOperationType getOperation() const;
    void setOperation(const QTkfInvestOaOperationType &operation);
    bool is_operation_Set() const;
    bool is_operation_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    qint32 lots;
    bool m_lots_isSet;
    bool m_lots_isValid;

    QTkfInvestOaOperationType operation;
    bool m_operation_isSet;
    bool m_operation_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::QTkfInvestOaMarketOrderRequest)

#endif // QTkfInvestOaMarketOrderRequest_H
