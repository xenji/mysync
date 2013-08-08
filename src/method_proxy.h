//
//  method_proxy.h
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//

#ifndef __mysync__method_proxy__
#define __mysync__method_proxy__

#include <iostream>
#include <vector>

namespace MySync {
    class MethodProxy {
    public:
        virtual std::string getMethodName() = 0;
        virtual std::string generateStatement(const std::vector<std::string> values) = 0;
        virtual ~MethodProxy() {};
    protected:
        std::vector<std::string> fields;
        std::string table_name;
        void setTable(const std::string _table_name);
        void setFields(const std::vector<std::string> _fields);
    };
}

#endif /* defined(__mysync__method_proxy__) */
