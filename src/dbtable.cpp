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
#include <cppconn/resultset_metadata.h>
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
    
    void DbTable::gatherTargetFields() {
        sql::ResultSet *res;
        sql::Statement *stmt = target_conn->createStatement();
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
    
    bool DbTable::gatherAndValidateSourceFields() {
        sql::ResultSet *res;
        sql::Statement *stmt = source_conn->createStatement();
        std::stringstream ss;
        std::string query;
        
        ss << select_statement << " LIMIT 1";
        query = ss.str();
        
        std::cout << "\tJob: gatherAndValidateSourceFields | Table: " << table_name << std::endl;
        std::cout << "\t\tStarting column validation" << std::endl;
        std::cout << "\t\tExecuting query to get the source column count [" << query << "]" << std::endl;
        
        res = stmt->executeQuery(query);
        res->next();
        sql::ResultSetMetaData *meta = res->getMetaData();
        int source_count = meta->getColumnCount();

        if (source_count !=  source_fields.size()) {
            std::cerr << "\t\tThe query for table " << table_name << " resolves to " << source_count << " cols. The target table has " << source_fields.size() << std::endl;
            std::cerr << "\t\tThe field alignment is by position, so take care that your query has the exact column count and the cols match in order." << std::endl;
            std::cerr << "\tFail: Validation for table " << table_name << std::endl;
            return false;
        }
        else {
            std::cout << "\t\tColumn count looks ok. Proceeding." << std::endl;
        }
        std::cout << "\tDone: gatherAndValidateSourceFields" << std::endl;
        return true;
    }
}