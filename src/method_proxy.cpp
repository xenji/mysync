//
//  method_proxy.cpp
//  mysync
//
//  Created by Mario Mueller on 08.08.13.
//
//
#include <iostream>
#include "method_proxy.h"

namespace MySync {

    void MethodProxy::setFields(const std::vector<std::string> _fields) {
        fields = _fields;
    }
    
    void MethodProxy::setTable(const std::string _table) {
        table_name = _table;
    }
    
    void MethodProxy::setBatchSize(int _size) {
        size = _size;
    }
    
    void MethodProxy::setKeyField(const std::string _key) {
        key = _key;
    }
}