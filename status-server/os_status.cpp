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

void OSStatus::calculateCPUUse(std::string buffer, long int* total, long int* idle)
{
    std::string data(buffer);
    std::istringstream ss(data);

    std::string temp;
    int sum = 0;
    ss >> temp;
    ss >> temp;
    sum += atoi(temp.c_str());
    ss >> temp;
    sum += atoi(temp.c_str());
    ss >> temp;
    sum += atoi(temp.c_str());

    *idle = atoi(temp.c_str());

    ss >> temp;
    sum += atoi(temp.c_str());
    ss >> temp;
    sum += atoi(temp.c_str());
    ss >> temp;
    sum += atoi(temp.c_str());
    ss >> temp;
    sum += atoi(temp.c_str());

    *total = sum;
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
    ss << "<h2>Uptime:</h2><p>Uptime: " << uptime << " segundos</p>";

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

std::string OSStatus::getProcessorStatus()
{
    char buffer[200];
    int size = sizeof(buffer);
    FILE *fp = fopen("/proc/stat", "r");
    fgets(buffer, size, fp);
    fclose(fp);
    
    long int previousSum = 0;
    long int previousIdle = 0;

    calculateCPUUse(buffer, &previousSum, &previousIdle);

    sleep(1);

    fp = fopen("/proc/stat", "r");
    fgets(buffer, size, fp);
    fclose(fp);

    long int sum = 0;
    long int idle = 0;

    calculateCPUUse(buffer, &sum, &idle);

    float result = (1-(float)(idle-previousIdle)/(float)(sum - previousSum)) * 100;

    std::ostringstream ss;

    ss << "<h2>Uso do Processador:</h2><p>Utilização: " << result << " %</p>";

    return ss.str();
}

std::string OSStatus::getMemoryStatus()
{
    char buffer[200];
    int size = sizeof(buffer);
    FILE *fp = fopen("/proc/meminfo", "r");

    fgets(buffer, size, fp);
    int offset = getColon(buffer) + 2;
    int end = getEnd(offset, buffer);

    std::string total(buffer + offset, buffer + end - 3);
    long int memTotal = atoi(total.c_str());

    fgets(buffer, size, fp);
    fgets(buffer, size, fp);
    offset = getColon(buffer) + 2;
    end = getEnd(offset, buffer);

    std::string free(buffer + offset, buffer + end - 3);
    long int memFree = atoi(free.c_str());

    fclose(fp);

    std::ostringstream ss;

    ss << "<h2>Uso da Memória:</h2><p>Memória Total: " << (float)memTotal / 1000.0f << " MB</p><p>Memória Usada: " << (float)(memTotal - memFree) / 1000.0f << " MB</p>";

    return ss.str();
}

std::string OSStatus::getSystemVersion()
{
    char buffer[500];
    int size = sizeof(buffer);
    FILE *fp = fopen("/proc/version", "r");

    fgets(buffer, size, fp);
    int end = getEnd(0, buffer);

    std::string osVersion(buffer, buffer + end);

    fclose(fp);

    std::ostringstream ss;

    ss << "<h2>Versão do Sistema: </h2><p>" << osVersion << "</p>";
    return ss.str();
}