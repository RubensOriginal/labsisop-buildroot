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

    ss << "<h2>Uso da Memória:</h2><p>Memória Total: " << (float)memTotal / 1000.0f << " MB</p><p>Memória em Uso: " << (float)(memTotal - memFree) / 1000.0f << " MB</p>";

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

std::string OSStatus::getProcesses()
{

    DIR *dir;
    struct dirent *dp;

    if ((dir = opendir ("/proc")) == NULL) {
        perror ("Cannot open .");
        exit (1);
    }

    std::regex r("([0-9])*");

    std::ostringstream ss;

    ss << "<h2>Processos em Execução:</h2>";

    while ((dp = readdir (dir)) != NULL) {
        std::string path(dp->d_name);

        if (regex_match(path, r)) {
            std::string proc_path = "/proc/";
            proc_path.append(path);
            proc_path.append("/status");

            char buffer[500];
            int size = sizeof(buffer);
            FILE *fp = fopen(proc_path.c_str(), "r");

            fgets(buffer, size, fp);
            int offset = getColon(buffer) + 2;
            int end = getEnd(offset, buffer);

            std::string process_name(buffer + offset, buffer + end);

            ss << "<p>PID: "<< dp->d_name << " | Nome do processo: " << process_name << "</p>";

            fclose(fp);

        }
    }

    closedir(dir);

    return ss.str();
}

std::string OSStatus::getDiskSize()
{
    DIR *dir;
    struct dirent *dp;
    char buffer[500];
    int size = sizeof(buffer);

    if ((dir = opendir ("/sys/block/")) == NULL) {
        perror("Cannot open .");
        exit(1);
    }

    std::ostringstream ss;

    ss << "<h2>Unidades de Disco:</h2>";

    while ((dp = readdir (dir)) != NULL) {
        std::string disk(dp->d_name);

        if (dp->d_name[0] == '.' || dp->d_name[0] == 'l' || dp->d_name[0] == 'd')
            continue;

        std::string path = "/sys/block/";
        path.append(disk);
        path.append("/queue/physical_block_size");
        FILE *fp = fopen(path.c_str(), "r");

        fgets(buffer, size, fp);
        int block_size = atoi(buffer);
        fclose(fp);

        path = "/sys/block/";
        path.append(disk);
        path.append("/size");
        fopen(path.c_str(), "r");

        fgets(buffer, size, fp);
        unsigned long int disk_size = atoi(buffer) * block_size;
        fclose(fp);

        ss << "<p>Disco: " << disk << " | Tamanho: " << (float) disk_size / 1000000.0f << " MB</p>";
    }

    return ss.str();
}

std::string OSStatus::getNetwork()
{
    char buffer[500];
    int size = sizeof(buffer);

    std::ostringstream ss;

    ss << "<h2>Adaptadores de Rede:</h2>";

    FILE *fp = fopen("/proc/net/route", "r");

    fgets(buffer, size, fp);

    while(!feof(fp)) {
        fgets(buffer, size, fp);
        
        std:: stringstream sbuffer(buffer);

        std::string interface;
        std::string hex_addr;
        sbuffer >> interface;
        sbuffer >> hex_addr;

        std::string address = std::to_string(std::stol("0" + hex_addr.substr(6,2), nullptr, 16)) + "." + std::to_string(std::stol("0" + hex_addr.substr(4,2), nullptr, 16)) + "." + std::to_string(std::stol("0" + hex_addr.substr(2,2), nullptr, 16)) + "." + std::to_string(std::stol("0" + hex_addr.substr(0,2), nullptr, 16));

        ss << "<p> Rede: " << interface << " | IP: " << address << "</p>"; 
    }

    fclose(fp);

    return ss.str();
}