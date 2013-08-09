//
//  upsert_method.cpp
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#include "upsert_method.h"
namespace MySync {
    
    std::string UpsertMethod::getMethodName() {
        return "UPSERT";
    }
    
    std::string UpsertMethod::enhanceStatement(std::string key, std::string statement) {
        
        return statement;
    }
    
    sql::PreparedStatement* UpsertMethod::generateStatement(sql::ResultSetMetaData* md) {
        sql::PreparedStatement *stmt = NULL;
        return stmt;
    }
    
    sql::PreparedStatement* UpsertMethod::applyValues(sql::PreparedStatement* statement, const std::vector<std::string> values) {
        sql::PreparedStatement *stmt = NULL;
        return stmt;
    }
}