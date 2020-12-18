#pragma once

#include <QString>
#include <QUrl>

#include <exception>
#include <stdexcept>

#include "models.h"


namespace invest_openapi
{


class OpenApiFactory
{

public:


    OpenApiFactory( const ApiConfig &apiConfig, const QString &token, bool sandboxMode )
    : m_apiConfig(apiConfig)
    , m_authConfig( token, sandboxMode )
    {
        m_apiConfig.checkValid();
    }

    OpenApiFactory( const ApiConfig &apiConfig, const AuthConfig &authConfig )
    : m_apiConfig(apiConfig)
    , m_authConfig(authConfig)
    {
        m_apiConfig.checkValid();
    }

    OpenApiFactory( const OpenApiFactory &f )
    : m_apiConfig(f.m_apiConfig)
    , m_authConfig(f.m_authConfig)
    {
        m_apiConfig.checkValid();
    }


    SandboxApi* getSandboxApi(const int timeOut = 0) const
    {
        //if (!m_sandboxMode)
        //    throw std::runtime_error("Attempting to use the sandbox in combat mode");

        if (!isSandboxMode())
            return 0;
        
        QUrl apiUrl = getApiUrl();

        SandboxApi *pApi = new SandboxApi( apiUrl.scheme(), apiUrl.host(), apiUrl.port(0), apiUrl.path(), timeOut );
        if (!pApi)
            return pApi;

        pApi->addHeaders( "Authorization", QString("Bearer ") + m_authConfig.token );
        pApi->addHeaders( "Accept", "application/json" );

        return pApi;
    }

    MarketApi* getMarketApi(const int timeOut = 0) const
    {
        QUrl apiUrl = getApiUrl();

        MarketApi *pApi = new MarketApi( apiUrl.scheme(), apiUrl.host(), apiUrl.port(0), apiUrl.path(), timeOut );

        if (!pApi)
            return pApi;

        pApi->addHeaders( "Authorization", QString("Bearer ") + m_authConfig.token );
        pApi->addHeaders( "Accept", "application/json" );
        
        return pApi;
    }

    bool isSandboxMode() const
    {
        return m_authConfig.sandboxMode;
    }


protected:

    QUrl getApiUrl() const
    {
        if (!isSandboxMode())
            return QUrl::fromUserInput( m_apiConfig.url );
        else
            return QUrl::fromUserInput( m_apiConfig.urlSandbox );
    }

    ApiConfig    m_apiConfig;
    AuthConfig   m_authConfig;


}; // class OpenApiFactory



} // namespace invest_openapi
