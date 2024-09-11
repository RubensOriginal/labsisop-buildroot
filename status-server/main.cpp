#include "tcp_server.h"


int main()
{
    // TcpServer server = TcpServer("10.0.0.242", 8080);
    TcpServer server = TcpServer("192.168.1.10", 8080);
    server.startListen();

    return 0;
}