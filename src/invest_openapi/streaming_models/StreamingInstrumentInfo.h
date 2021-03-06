/**
 */

/*
 * StreamingInstrumentInfo.h
 *
 * 
 */

#ifndef StreamingInstrumentInfo_H
#define StreamingInstrumentInfo_H

#include <QJsonObject>

//#include "Currency.h"
//#include "InstrumentType.h"
#include <QString>

#include "../models/client/TradeStatus.h"

#include "../models/client/Enum.h"
#include "../models/client/Object.h"

#include "../marty_decimal.h"


namespace OpenAPI {

class StreamingInstrumentInfo : public Object {
public:
    StreamingInstrumentInfo();
    StreamingInstrumentInfo(QString json);
    ~StreamingInstrumentInfo() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getFigi() const;
    void setFigi(const QString &figi);
    bool is_figi_Set() const;
    bool is_figi_Valid() const;

    TradeStatus getTradeStatus() const;
    void setTradeStatus(const TradeStatus &trade_status);
    bool is_trade_status_Set() const;
    bool is_trade_status_Valid() const;

    marty::Decimal getMinPriceIncrement() const;
    void setMinPriceIncrement(const marty::Decimal &min_price_increment);
    bool is_min_price_increment_Set() const;
    bool is_min_price_increment_Valid() const;

    qint32 getLot() const;
    void setLot(const qint32 &lot);
    bool is_lot_Set() const;
    bool is_lot_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString figi;
    bool m_figi_isSet;
    bool m_figi_isValid;

    TradeStatus trade_status;
    bool m_trade_status_isSet;
    bool m_trade_status_isValid;

    marty::Decimal min_price_increment;
    bool m_min_price_increment_isSet;
    bool m_min_price_increment_isValid;

    qint32 lot;
    bool m_lot_isSet;
    bool m_lot_isValid;

};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::StreamingInstrumentInfo)

#endif // StreamingInstrumentInfo_H
