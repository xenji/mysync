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
    }
    
    void DbTable::setSourceConnection(sql::Connection *c) {
        source_conn = c;
    }
    
    void DbTable::setTargetConnection(sql::Connection *c) {
        target_conn = c;
    }
    
    void DbTable::setBatchSize(int bsize) {
        batch_size = bsize;
    }
    
    void DbTable::setTableName(std::string _table_name) {
        table_name = _table_name;
    }
    
    std::string DbTable::getTableName() {
        return table_name;
    }
    
    void DbTable::setMethodProxy(MySync::MethodProxy *_method_proxy) {
        method_proxy = _method_proxy;
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
            delete res;
            delete stmt;
        }
        else {
            std::cout << "\t\tColumn count looks ok. Proceeding." << std::endl;
        }
        delete res;
        delete stmt;
        std::cout << "\tDone: gatherAndValidateSourceFields" << std::endl;
        return true;
    }
    
    void DbTable::run() {
        std::cout << "\tJob: run | Table: " << table_name << std::endl;
        method_proxy->setFields(source_fields);
        method_proxy->setTable(table_name);
        
        int work_count = 0;
//        int offset = 0; // for later. make it work first.
        sql::ResultSet *source_result;
        sql::ResultSetMetaData *meta;
        sql::Statement *source_statement = source_conn->createStatement();
        source_result = source_statement->executeQuery(select_statement);

        std::cout << "\t\tReceived data from the source, starting target run." << std::endl;
        
        while (source_result->next()) {
            sql::Statement *target_statement = target_conn->createStatement();
            std::vector<std::string> values;
            
            meta = source_result->getMetaData();
            int col_count = meta->getColumnCount();
            
            for (int i = 1; i != col_count; i++) {
                values.push_back(source_result->getString(i));
            }
            
            std::string statement = method_proxy->generateStatement(values);
            std::cout << "\t\t\tDEBUG: " << statement << std::endl;
            //target_statement->execute(statement);
            ++work_count;
        }
        
        delete source_result;
        delete source_statement;
        
        std::cout << "\t\tProcessed " << work_count << " rows." << std::endl;
        std::cout << "\tDone: run" << std::endl;
    }
}