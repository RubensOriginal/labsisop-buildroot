#ifndef INCLUDE_TCP_SERVER
#define INCLUDE_TCP_SERVER

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>


class TcpServer {
    private:
        std::string m_ip_address;
        int m_port;
        int m_socket;
        sockaddr_in m_socket_address;
        unsigned int m_socket_address_len;
        const int BUFFER_SIZE = 30720;

    public:
        TcpServer(std::string ip_address, int port);
        ~TcpServer();
        int startServer();
        void closeServer();
        void startListen();
        void acceptConnection(int &new_socket);
        std::string buildResponse();
        void sendResponse(int &new_socket, std::string &response);
};

#endif