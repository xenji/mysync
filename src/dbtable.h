//
//  dbtable.h
//  mysync
//
//  Created by Mario Mueller on 07.08.13.
//
//

#ifndef __mysync__dbtable__
#define __mysync__dbtable__

#include <iostream>
#include "mysql_connection.h"
#include <vector>
#include "method_proxy.h"
#define DEFAULT_BATCH_SIZE 1000

namespace MySync {
    class DbTable {
        
    public:
        DbTable(std::string table_name, std::string select_statement);
        void setSourceConnection(sql::Connection *conn);
        void setTargetConnection(sql::Connection *conn);
        void setBatchSize(int bsize);
        void setMethodProxy(MethodProxy _method_proxy);
        void setTableName(std::string _table_name);
        std::string getTableName();
        bool gatherAndValidateSourceFields();
        void gatherTargetFields();
        
    private:
        int limit;
        std::vector<std::string> source_fields;
        std::string table_name;
        sql::Connection *source_conn;
        sql::Connection *target_conn;
        std::string select_statement;
    };
}
#endif /* defined(__mysync__dbtable__) */
