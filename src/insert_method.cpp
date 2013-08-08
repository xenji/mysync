//
//  insert_method.cpp
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#include "insert_method.h"

namespace MySync {

    std::string InsertMethod::getMethodName() {
        return "INSERT";
    }
    
    std::string InsertMethod::enhanceStatement(std::string key, std::string statement) {
        
        return statement;
    }
    
    std::string InsertMethod::generateStatement(const std::vector<std::string> values) {
        return "";
    }
}