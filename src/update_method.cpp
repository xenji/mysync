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

namespace MySync {
    std::string UpdateMethod::getMethodName() {
        return "UPDATE";
    }
    
    std::string UpdateMethod::generateStatement(const std::vector<std::string> values) {
        std::stringstream ss;
        
        ss << "UPDATE " << table_name << " SET ";
        
        for(std::vector<int>::size_type i = 0; i < values.size(); i++) {
            ss << fields[i] << "='" << values[i] << "'";
            if ((values.size() - 1) == i) {
                ss << std::endl;
            }
        }
        
        std::string statement = ss.str();
        std::cout << statement << std::endl;
        
        return statement;
    }
}