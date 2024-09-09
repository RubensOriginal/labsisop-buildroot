#include "tcp_server.h"


int main() {

    // TcpServer* server = new TcpServer("10.0.1.14", 8080);
    TcpServer* server = new TcpServer("192.168.1.10", 8080);
    server->startListen();

    return 0;
}