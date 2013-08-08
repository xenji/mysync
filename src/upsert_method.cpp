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
        return "INSERT";
    }
    
    std::string UpsertMethod::enhanceStatement(std::string key, std::string statement) {
        
        return statement;
    }
    
    std::string UpsertMethod::generateStatement(const std::vector<std::string> values) {
        return "";
    }
}