//
//  method_proxy.h
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#ifndef __mysync__method_proxy__
#define __mysync__method_proxy__

#include <iostream>
#include <vector>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset_metadata.h>

namespace MySync {
    class MethodProxy {
    public:
        virtual std::string getMethodName() = 0;
        virtual sql::PreparedStatement* generateStatement(sql::ResultSetMetaData* md) = 0;
        virtual sql::PreparedStatement* applyValues(sql::PreparedStatement* statement, std::vector<std::string> values) = 0;
        virtual std::string enhanceStatement(std::string key, std::string statement) = 0;
        void setBatchSize(int);
        void setSourceConnection(sql::Connection *conn);
        void setTargetConnection(sql::Connection *conn);
        void setFields(const std::vector<std::string> _fields);
        void setTable(const std::string _table_name);
        void setKeyField(const std::string _key);
        std::string getKeyField();
        virtual ~MethodProxy() {};
    protected:
        int size;
        sql::Connection *source_conn;
        sql::Connection *target_conn;
        std::string key;
        std::vector<std::string> fields;
        std::string table_name;
    };
}
#endif /* defined(__mysync__method_proxy__) */
