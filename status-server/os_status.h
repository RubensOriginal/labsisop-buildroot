#ifndef INCLUDE_OS_STATUS
#define INCLUDE_OS_STATUS

#include <string>
#include <sstream>
#include <iostream>

class OSStatus {
    public:
        OSStatus();
        ~OSStatus();

        std::string getSystemTime();
};

#endif