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
#include <boost/algorithm/string/join.hpp>
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
        
        std::string joined = boost::algorithm::join(list, "','"); 
        ss << key << " IN ('" << joined << "')";
        return statement;
    }
    
    sql::PreparedStatement* UpdateMethod::generateStatement(const std::vector<std::string> values) {
        std::stringstream ss;
        std::string pkvalue;

        ss << "UPDATE " << table_name << " SET ";

        for(std::vector<int>::size_type i = 0; i < values.size(); i++) {
            if (fields[i] == key) {
                pkvalue = values[i];
                continue;
            }
            ss << fields[i] << "=?";
            
            if (i != values.size()-1) {
                ss << ", ";
            }
        }
        ss << " WHERE " << key << "=?";
        sql::PreparedStatement *prep_st = this->target_conn->prepareStatement(ss.str());

        std::vector<int>::size_type i;
        for(i = 1; i < values.size(); i++) {
            prep_st->setString(i, values[i]);
        }
        prep_st->setString(i, pkvalue);
        
        return prep_st;
    }
}