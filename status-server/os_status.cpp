#include "os_status.h"

OSStatus::OSStatus()
{

}

OSStatus::~OSStatus()
{

}

std::string OSStatus::getSystemTime()
{
    char buffer[200];
    int size = sizeof(buffer);

    FILE* fp = fopen("/proc/driver/rtc", "r");
    fgets(buffer, size, fp);
    std::string time(buffer + 11, buffer + 19);
    fgets(buffer, size, fp);
    std::string date(buffer + 11, buffer + 21);

    std::ostringstream ss;

    ss << "<h2>Data e Hora:</h2><p>Data: " << date << "</p><p>Hora: " << time << "</p>";

    return ss.str();
}