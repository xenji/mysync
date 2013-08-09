//
//  insert_method.cpp
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#include "insert_method.h"
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

    std::string InsertMethod::getMethodName() {
        return "INSERT";
    }
    
    std::string InsertMethod::enhanceStatement(std::string key, std::string statement) {
        
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
            return statement;
        }
        
        std::string joined = boost::algorithm::join(list, "','");
        ss << key << " NOT IN ('" << joined << "')";
        statement = ss.str();
        return statement;
    }
    
    sql::PreparedStatement* InsertMethod::generateStatement(const std::vector<std::string> values) {

        std::stringstream ss;
        std::stringstream vv;
        sql::PreparedStatement *prep_st;
        
        ss << "INSERT INTO " << table_name << " (";
        
        for(std::vector<int>::size_type i = 0; i < values.size(); i++) {

            ss << fields[i];
            vv << "?";
            if (i != values.size()-1) {
                ss << ",";
                vv << ",";
            }
        }
        ss << ") VALUES (" << vv.str() << ")";
        std::string test = ss.str();
        prep_st = this->target_conn->prepareStatement(ss.str());
        
        for(std::vector<int>::size_type i = 0; i < values.size(); i++) {
            prep_st->setString(i+1, values[i]);
        }
        
        return prep_st;

    }
}