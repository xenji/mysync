//
//  dbtable.cpp
//  mysync
//
//  Created by Mario Mueller on 07.08.13.
//
//

#include "dbtable.h"
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <sstream>
namespace MySync {
    DbTable::DbTable(std::string table, std::string statement) {
        select_statement = statement;
        table_name = table;
        limit = DEFAULT_BATCH_SIZE;
    }
    
    void DbTable::setSourceConnection(sql::Connection *c) {
        source_conn = c;
    }
    
    void DbTable::setTargetConnection(sql::Connection *c) {
        target_conn = c;
    }
    
    void DbTable::setBatchSize(int bsize) {
        limit = bsize;
    }
    
    void DbTable::setTableName(std::string _table_name) {
        table_name = _table_name;
    }
    
    std::string DbTable::getTableName() {
        return table_name;
    }
    
    void DbTable::gatherSourceFields() {
        sql::ResultSet *res;
        sql::Statement *stmt = source_conn->createStatement();
        std::stringstream ss;
        
        ss << "DESCRIBE " << table_name;
        res = stmt->executeQuery(ss.str());
        
        std::cout << "\tJob: GatherSourceFields | Table: " << table_name << std::endl;
        
        while (res->next()) {
            std::string field_name = res->getString(1);
            source_fields.push_back(field_name);
            std::cout << "\t\tFound field: " << field_name <<  std::endl;
        }
        std::cout << "\tDone: GatherSourceFields" << std::endl;
    }
}