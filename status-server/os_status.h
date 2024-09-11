#ifndef INCLUDE_OS_STATUS
#define INCLUDE_OS_STATUS

#include <string>
#include <sstream>
#include <iostream>

class OSStatus {
    private:
        int getColon(char* str);
        int getEnd(int offset, char* str);

    public:
        OSStatus();
        ~OSStatus();

        std::string getSystemTime();
        std::string getUptime();
        std::string getProcessorModel();
};

#endif