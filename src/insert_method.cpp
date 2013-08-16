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
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <sstream>
#include <boost/algorithm/string/join.hpp>
#include "abort_exception.h"

namespace MySync {

    std::string InsertMethod::getMethodName() {
        return "INSERT";
    }
    
    std::string InsertMethod::enhanceStatement(const std::string key, const std::string statement) {
        
        sql::Statement *stmt = target_conn->createStatement();
        std::stringstream query;
        std::vector<std::string> list;
        std::stringstream ss;
        
        query << "SELECT " << key << " FROM " << table_name;
        sql::ResultSet *res;
        try {
            res = stmt->executeQuery(query.str());
        }
        catch (const sql::SQLException &e) {
            std::cerr << "# ERR: SQLException in " << __FILE__;
            std::cerr << "(" << __FUNCTION__ << ") on line "
            << __LINE__ << std::endl;
            std::cerr << "# ERR: " << e.what();
            std::cerr << " (MySQL error code: " << e.getErrorCode();
            std::cerr << ", SQLState: " << e.getSQLState() <<
            " )" << std::endl;
            res = NULL;
        }

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

        ss << key << " NOT IN ('" << boost::algorithm::join(list, "','") << "')";
        return ss.str();
    }
    
    sql::PreparedStatement* InsertMethod::applyValues(sql::PreparedStatement* statement, const std::vector<std::string> values) {
        for(std::vector<int>::size_type i = 0; i < values.size(); i++) {
            statement->setString(i+1, values[i]);
        }
        return statement;
    }
    
    sql::PreparedStatement* InsertMethod::generateStatement(sql::ResultSetMetaData* md) {
        std::stringstream ss;
        std::stringstream vv;
        ss << "INSERT INTO " << this->table_name << " (";
        
        for(std::vector<int>::size_type i = 0; i < md->getColumnCount(); i++) {
            ss << fields[i];
            vv << "?";
            if (i != md->getColumnCount()-1) {
                ss << ",";
                vv << ",";
            }
        }
        ss << ") VALUES (" << vv.str() << ")";
        return this->target_conn->prepareStatement(ss.str());
    }
}
