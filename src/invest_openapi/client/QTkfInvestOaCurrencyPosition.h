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
 * QTkfInvestOaCurrencyPosition.h
 *
 * 
 */

#ifndef QTkfInvestOaCurrencyPosition_H
#define QTkfInvestOaCurrencyPosition_H

#include <QJsonObject>

#include "QTkfInvestOaCurrency.h"

#include "QTkfInvestOaEnum.h"
#include "QTkfInvestOaObject.h"

namespace OpenAPI {

class QTkfInvestOaCurrencyPosition : public QTkfInvestOaObject {
public:
    QTkfInvestOaCurrencyPosition();
    QTkfInvestOaCurrencyPosition(QString json);
    ~QTkfInvestOaCurrencyPosition() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QTkfInvestOaCurrency getCurrency() const;
    void setCurrency(const QTkfInvestOaCurrency &currency);
    bool is_currency_Set() const;
    bool is_currency_Valid() const;

    double getBalance() const;
    void setBalance(const double &balance);
    bool is_balance_Set() const;
    bool is_balance_Valid() const;

    double getBlocked() const;
    void setBlocked(const double &blocked);
    bool is_blocked_Set() const;
    bool is_blocked_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QTkfInvestOaCurrency currency;
    bool m_currency_isSet;
    bool m_currency_isValid;

    double balance;
    bool m_balance_isSet;
    bool m_balance_isValid;

    double blocked;
    bool m_blocked_isSet;
    bool m_blocked_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::QTkfInvestOaCurrencyPosition)

#endif // QTkfInvestOaCurrencyPosition_H
