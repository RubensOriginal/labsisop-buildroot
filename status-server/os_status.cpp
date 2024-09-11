#include "os_status.h"

OSStatus::OSStatus()
{

}

OSStatus::~OSStatus()
{

}

int OSStatus::getColon(char* str)
{
    int i = 0;

    while(str[i] != ':') {
        i++;
    }

    return i;
}

int OSStatus::getEnd(int offset, char* str)
{
    int i = offset;
    while(str[i] != '\n') {
        i++;
    }

    return i;
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

    fclose(fp);

    std::ostringstream ss;

    ss << "<h2>Data e Hora:</h2><p>Data: " << date << "</p><p>Hora: " << time << "</p>";

    return ss.str();
}

std::string OSStatus::getUptime()
{
    char buffer[200];
    int size = sizeof(buffer);

    FILE* fp = fopen("/proc/uptime", "r");
    fgets(buffer, size, fp);

    int index = 0;

    while (buffer[index] != ' ') {
        index++;
    }

    std::string uptime(buffer, buffer + index);

    fclose(fp);

    std::ostringstream ss;
    ss << "<h2>Uptime:</h2><p>Uptime: " << uptime << "</p>";

    return ss.str();
}

std::string OSStatus::getProcessorModel()
{
    char buffer[200];
    int size = sizeof(buffer);
    FILE *fp = fopen("/proc/cpuinfo", "r");
    fgets(buffer, size, fp);
    fgets(buffer, size, fp);

    int offset = getColon(buffer) + 2;
    int end = getEnd(offset, buffer);

    std::string vendor(buffer + offset, buffer + end);

    fgets(buffer, size, fp);
    fgets(buffer, size, fp);
    fgets(buffer, size, fp);

    offset = getColon(buffer) + 2;
    end = getEnd(offset, buffer);

    std::string model(buffer + offset, buffer + end);

    fgets(buffer, size, fp);
    fgets(buffer, size, fp);
    
    offset = getColon(buffer) + 2;
    end = getEnd(offset, buffer);

    std::string frequency(buffer + offset, buffer + end);

    fclose(fp);

    std::ostringstream ss;

    ss << "<h2>Informações do CPU:</h2><p>Fabricante: " << vendor << "</p><p>Processador: " << model << "</p><p>Velocidade: " << frequency << " MHz</p>";

    return ss.str();
}