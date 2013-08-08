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
    
    std::string TruncInsertMethod::generateStatement(const std::vector<std::string> values) {
        return "";
    }
}