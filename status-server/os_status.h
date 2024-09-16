#ifndef INCLUDE_OS_STATUS
#define INCLUDE_OS_STATUS

#include <string>
#include <sstream>
#include <iostream>
#include <regex>

#include <unistd.h>
#include <dirent.h>

class OSStatus {
    private:
        int getColon(char* str);
        int getEnd(int offset, char* str);
        void calculateCPUUse(std::string buffer, long int* total, long int* idle);

    public:
        OSStatus();
        ~OSStatus();

        std::string getSystemTime();
        std::string getUptime();
        std::string getProcessorModel();
        std::string getProcessorStatus();
        std::string getMemoryStatus();
        std::string getSystemVersion();
        std::string getProcesses();
        std::string getDiskSize();
        std::string getNetwork();
};

#endif