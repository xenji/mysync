//
//  update_method.cpp
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#include "update_method.h"
#include <sstream>
#include <string>
#include <vector>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <sstream>
#include "abort_exception.h"

namespace MySync {
    std::string UpdateMethod::getMethodName() {
        return "UPDATE";
    }
    
    std::string UpdateMethod::enhanceStatement(std::string key, std::string statement) {
        // we do updates, so just get those we have in the target table.
        sql::Statement *stmt = target_conn->createStatement();
        std::stringstream query;
        std::vector<std::string> list;
        std::stringstream ss;
        
        query << "SELECT " << key << " FROM " << table_name;
        sql::ResultSet *res = stmt->executeQuery(query.str());
        
        if (
               (statement.find("WHERE") == std::string::npos)
            && (statement.find("where") == std::string::npos)) {
            
            ss << statement << " WHERE ";
        }
        else {
            ss << statement << " AND ";
        }

        while (res->next()) {
            list.push_back(res->getString(1));
        }
        
        if (list.size() == 0) {
            throw AbortException();
        }
        ss << key << " IN ('";
        
        for (std::size_t i = 0; i < list.size(); i++) {
            ss  << list[i];
            if (i < (list.size() - 1)) {
                ss << "','";
            }
        }
        ss << "')";
        return ss.str();
    }
    
    sql::PreparedStatement* UpdateMethod::applyValues(sql::PreparedStatement* statement, const std::vector<std::string> values) {
        // skip first one as it is probably the key. This needs to be enhanced later on.
        std::vector<int>::size_type i;
        for(i = 1; i < values.size(); i++) {
            statement->setString(i, values[i]);
        }
        statement->setString(i, values[0]);
        return statement;
    }
    
    sql::PreparedStatement* UpdateMethod::generateStatement(sql::ResultSetMetaData* md) {
        std::stringstream ss;
        std::string pkvalue;

        ss << "UPDATE " << table_name << " SET ";
        for(std::vector<int>::size_type i = 0; i < md->getColumnCount(); i++) {
            if (fields[i] == key) {
                continue;
            }
            ss << fields[i] << "=?";
            
            if (i != md->getColumnCount()-1) {
                ss << ", ";
            }
        }
        ss << " WHERE " << key << "=?";
        return this->target_conn->prepareStatement(ss.str());
    }
}
