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

namespace MySync {
    class MethodProxy {
    public:
        virtual std::string getMethodName() = 0;
        virtual std::string generateStatement() = 0;
        virtual ~MethodProxy() {};
    };
}

#endif /* defined(__mysync__method_proxy__) */
