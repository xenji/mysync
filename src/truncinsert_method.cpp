//
//  truncinsert_method.cpp
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#include "truncinsert_method.h"
namespace MySync {
    
    std::string TruncInsertMethod::getMethodName() {
        return "INSERT";
    }
    
    std::string TruncInsertMethod::enhanceStatement(std::string key, std::string statement) {
        
        return statement;
    }
    
    sql::PreparedStatement* TruncInsertMethod::generateStatement(sql::ResultSetMetaData* md) {
        sql::PreparedStatement *stmt = NULL;
        return stmt;
    }
    
    sql::PreparedStatement* TruncInsertMethod::applyValues(sql::PreparedStatement* statement, const std::vector<std::string> values) {
        sql::PreparedStatement *stmt = NULL;
        return stmt;
    }
}