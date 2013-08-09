//
//  update_method.h
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#ifndef __mysync__update_method__
#define __mysync__update_method__

#include <iostream>
#include "method_proxy.h"

namespace MySync {
    class UpdateMethod : public MethodProxy {
        std::string getMethodName();
        sql::PreparedStatement* generateStatement(sql::ResultSetMetaData* md);
        sql::PreparedStatement* applyValues(sql::PreparedStatement* statement, const std::vector<std::string> values);
        std::string enhanceStatement(std::string key, std::string statement);
    };
}

#endif /* defined(__mysync__update_method__) */
