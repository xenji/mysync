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
#include <stdio.h>
#include <time.h>
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
    
    /**
     Gathers the target fields by describing the target's table and acquiring 
     the names of the fields. This means that your source table needs to match or 
     you get very very strange errors. Although we validate the field counts, it must
     not match exactly.
     
     @author Mario Mueller
     */
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
    
    /**
     We validate the source fields against the target fields by simply counting them.
     This is not the best method on earth but it is simple enough to keep things slim
     and add a minimum validity.
     
     @author Mario Mueller
     */
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
        std::size_t source_count = meta->getColumnCount();
        
        if (source_count !=  source_fields.size()) {
            std::cerr << "\t\tThe query for table " << table_name << " resolves to " << source_count << " cols. The target table has " << source_fields.size() << std::endl;
            std::cerr << "\t\tThe field alignment is by position, so take care that your query has the exact column count and the cols match in order." << std::endl;
            std::cerr << "\tFail: Validation for table " << table_name << std::endl;
            delete res;
            delete stmt;
            return false;
        }
        else {
            std::cout << "\t\tColumn count looks ok. Proceeding." << std::endl;
        }
        delete res;
        delete stmt;
        std::cout << "\tDone: gatherAndValidateSourceFields" << std::endl;
        return true;
    }
    
    /**
     And now some magic. 
     */
    void DbTable::run() {
        std::cout << "\tJob: run | Table: " << table_name << std::endl;
        method_proxy->setFields(source_fields);
        method_proxy->setTable(table_name);
        time_t start_time = time(NULL);
        
        int work_count = 0;
        int mod = 10000;

        // int offset = 0; // for later. make it work first.
        sql::ResultSet *source_result;
        sql::Statement *source_statement = source_conn->createStatement();
        std::string enhStatement = method_proxy->enhanceStatement(method_proxy->getKeyField(), select_statement);
        std::cout << "\t\tUsing statement for selection of source (first 150 chars): [" << enhStatement.substr(0, 150) << "]" << std::endl;
        source_result = source_statement->executeQuery(enhStatement);
        std::cout << "\t\tStatement returned " << source_result->rowsCount() << " rows." << std::endl;
        // Determine the steps when we show some output.
        if (source_result->rowsCount() < 1000000) {
            mod = 5000;
        }
        else if (source_result->rowsCount() < 100000) {
            mod = 1000;
        }
        else if (source_result->rowsCount() < 10000) {
            mod = 100;
        }
        
        std::cout << "\t\tReceived data from the source, starting target run." << std::endl;
        
        sql::ResultSetMetaData *md = source_result->getMetaData();
        sql::PreparedStatement* statement = method_proxy->generateStatement(md);
        while (source_result->next()) {
            std::vector<std::string> values;
            for (std::size_t i = 1; i <= md->getColumnCount(); i++) {
                values.push_back(source_result->getString(i));
            }
            
            // TODO: This is ugly. We should pass the connection to create the prep statement
            // from to the generateStatement method for not letting the method_proxy know
            // on what connection he works.
            try {
                statement = method_proxy->applyValues(statement, values);
                statement->execute();
                // Using this means about -100 ops per second but is more secure.
                statement->clearParameters();
            }
            catch (const sql::SQLException &e) {
                std::cerr << "# ERR: SQLException in " << __FILE__;
                std::cerr << "(" << __FUNCTION__ << ") on line "
                << __LINE__ << std::endl;
                std::cerr << "# ERR: " << e.what();
                std::cerr << " (MySQL error code: " << e.getErrorCode();
                std::cerr << ", SQLState: " << e.getSQLState() <<
                " )" << std::endl;
            }
            catch (const std::exception &e) {
                std::cout << "# ERR: Error in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what() << std::endl;
            }
            catch (...) {
                std::cout << "# ERR: Error in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
                std::cout << "# ERR: " << std::endl;
            }
            ++work_count;
            if (work_count % mod == 0) {
                std::cout << "\t\tProcessed " << work_count << " of " << source_result->rowsCount() << std::endl;
            }
        }
        
        delete source_result;
        delete source_statement;
        time_t end_time = time(NULL);
        
        double diff = difftime(end_time, start_time);
        double rate = work_count / diff;
        
        std::cout << "\t\tProcessed " << work_count << " rows in " << diff << " seconds. This is roughly " << rate << " ops per second." << std::endl;
        std::cout << "\tDone: run" << std::endl;
    }
}
