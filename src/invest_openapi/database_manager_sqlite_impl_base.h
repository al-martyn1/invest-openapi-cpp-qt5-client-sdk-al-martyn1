#pragma once

#include <QObject>
#include <QVariant>
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlField>
#include <QSqlQuery>
#include <QSharedPointer>

#include <exception>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include "database_manager_impl_base.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
namespace invest_openapi
{


//----------------------------------------------------------------------------
class DatabaseManagerSQLiteImplBase : public DatabaseManagerImplBase
{

protected:

    //------------------------------
    friend QSharedPointer<IDatabaseManager> createDatabaseManager( QSharedPointer<QSqlDatabase> pDb, QSharedPointer<DatabaseConfig> pDatabaseConfig, QSharedPointer<LoggingConfig> pLoggingConfig );


    //------------------------------
    DatabaseManagerSQLiteImplBase( QSharedPointer<QSqlDatabase> pDb, QSharedPointer<DatabaseConfig> pDatabaseConfig, QSharedPointer<LoggingConfig> pLoggingConfig )
    : DatabaseManagerImplBase(pDb, pDatabaseConfig, pLoggingConfig)
    {}

    //------------------------------
    virtual QString   defDecimal( ) const override
    {
        return defDecimal( m_defDecimalTotal, m_defDecimalFrac );
    }
    //------------------------------

    virtual QString q( const QString &str ) const override
    {
        QString escapedStr = sqlEscape( *m_pDb, str );

        if (escapedStr.isEmpty())
            return QString("''");

        if (escapedStr.front()=='\'')
            return escapedStr;

        return QString("'")
             + escapedStr
             + QString("'")
             ;
    }

    //------------------------------
    virtual QString lf( char comma = ' ' ) const override
    {
        return QString("\r\n")
             + QString( (QChar)comma )
             + QString(" ")
             ;
    }

    //------------------------------
    virtual QString   defDecimal      ( unsigned total, unsigned frac ) const override
    {
        return QString("DECIMAL(%1,%2)").arg(total).arg(frac);
    }

    //------------------------------
    virtual bool    tableCheckExist    ( const QString &tableName  ) const override
    {
        // https://stackoverflow.com/questions/1601151/how-do-i-check-in-sqlite-whether-a-table-exists

        QString queryText = QString("SELECT name FROM sqlite_master WHERE type='table' AND name=") + tableMapName(tableName) + QString(";");
        QSqlQuery query(*m_pDb);

        IOA_SQL_EXEC_QUERY( query, queryText );

        if (getQueryResultSize( query, true /* needBool */ ))
            return true;

        return false;
    }

    //------------------------------
    QString exists( IDatabaseManager::IfExists e ) const
    {
        switch(e)
        {
            case IDatabaseManager::IfExists::ifExists   :     return QString("IF EXISTS");
            case IDatabaseManager::IfExists::ifNotExists:     return QString("IF NOT EXISTS");
            default                                     :     return QString("");
        }
    }

    //------------------------------
    virtual QVector<QString> tableGetNames    () const override
    {
        QSqlQuery query = selectExecHelper("SELECT NAME FROM sqlite_master where type=\'table\';");
        return queryToSingleStringVector( query, 0, "sqlite_sequence", false );
    }
    
    //------------------------------
    virtual QVector<QString> tableGetColumnsInternal ( const QString &internalTableName ) const override
    {
        QSqlQuery query = selectExecHelper(QString("PRAGMA table_info(%1);").arg(sqlQuote(internalTableName)));
        return queryToSingleStringVector( query, 1 );
    }

    //------------------------------
    virtual bool    tableDrop          ( const QString &tableName, IfExists existence ) const override
    {
        // https://www.sqlitetutorial.net/sqlite-drop-table/
        QSqlQuery query(*m_pDb);
        QString queryText = QString("DROP TABLE %1 %2;").arg(exists(existence)).arg(tableMapName(tableName));
        RETURN_IOA_SQL_EXEC_QUERY( query, queryText );
        // return query.exec( queryText );
    }

    //------------------------------
    virtual bool    tableCreate        ( const QString &tableName, IfExists existence ) const override
    {
        QSqlQuery query(*m_pDb);
        QString queryText = QString("CREATE TABLE %1 %2 (%3)").arg(exists(existence)).arg(tableMapName(tableName)).arg(tableGetShema(tableName));
        RETURN_IOA_SQL_EXEC_QUERY( query, queryText );
        // return query.exec( queryText );
    }

    virtual bool insertToImpl( const QString &tableName, const QVector<QVector<QString> >  &vals, const QVector<QString> &tableColumns ) const override
    {
        QVector<QString> valuesList;

        int columnsSize = tableColumns.size();

        std::transform( vals.begin(), vals.end(), std::back_inserter(valuesList)
                      , [this, columnsSize]( const QVector<QString> &v )
                        {
                            if (columnsSize!=v.size())
                            {
                                throw std::runtime_error("DatabaseManagerSQLiteImplBase::insertToImpl: number of columns mismatch number of taken values");
                            }

                            return QString("(%1)").arg(mergeString( sqlQuote(v), ", "));
                        }
                      );

        QString queryText = QString("INSERT INTO %1 (%2)%3VALUES %5").arg(tableMapName(tableName))
                                                                     .arg( mergeString(tableColumns, ", ") )
                                                                     .arg(lf())
                                                                     .arg(mergeString( valuesList, QString(",") + lf() + QString("       ")) )
                                                                     ;
        QSqlQuery query(*m_pDb);
        RETURN_IOA_SQL_EXEC_QUERY( query, queryText );
    }

    virtual bool insertToImpl( const QString &tableName, const QVector<QString>   &vals, const QVector<QString> &tableColumns ) const override
    {
        QVector<QVector<QString> > tmp; tmp.push_back(vals);
        return insertToImpl( tableName, tmp, tableColumns );
    }

    virtual bool insertToImpl( const QString &tableName, const QVector<QVector<QVariant> > &vals, const QVector<QString> &tableColumns ) const override
    {
        return insertToImpl( tableName, toStringVector(vals), tableColumns );
    }

    virtual bool insertToImpl( const QString &tableName, const QVector<QVariant> &vals, const QVector<QString> &tableColumns ) const override
    {
        return insertToImpl( tableName, toStringVector(vals), tableColumns );
    }


    //------------------------------
    virtual bool      metaInsertForTablesBulk ( const QString &bulkText ) const override
    {
        //QStringList = toStringVector(splitString(bulkText, ";"));
        auto vec = splitString(bulkText, ";", ",.:;");
        return insertTo( "_META_TABLES", splitString(bulkText, ";", ",.:;"), tableGetColumns("_META_TABLES") /* "TABLE_NAME;DISPLAY_NAME;DESCRIPTION" */  );
    }

    //------------------------------
    virtual bool      metaInsertForColumnBulk ( QString tableName, const QString &bulkText ) const override
    {
        auto vec = splitString(bulkText, ";", ",.:;");
        return insertTo( "_META_COLUMNS", splitString(bulkText, ";", ",.:;"), tableGetColumns("_META_COLUMNS") /* "TABLE_NAME;DISPLAY_NAME;DESCRIPTION" */  );
    }



}; // DatabaseManagerSQLiteImplBase

//----------------------------------------------------------------------------




} // namespace invest_openapi
