#include "tcp_server.h"

TcpServer::TcpServer(std::string ip_address, int port) : m_ip_address(ip_address), m_port(port), m_socket(), m_socket_address_len(sizeof(sockaddr_in))
{
    m_socket_address.sin_family = AF_INET;
    m_socket_address.sin_port = htons(m_port);
    m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    if (startServer() != 0)
    {
        std::cout <<  "Failed to start server with Port: " << ntohs(m_socket_address.sin_port) << std::endl;
    }
}

TcpServer::~TcpServer()
{
    closeServer();
}

int TcpServer::startServer()
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (m_socket < 0) {
        std::cout << "Cannot create a socket" << std::endl;
        return 1;
    }

    if (bind(m_socket, (sockaddr *)&m_socket_address, m_socket_address_len)) {
        std::cout << "Cannot connect socket to address" << std::endl;
        return 1;
    }
    
    return 0;
}

void TcpServer::closeServer()
{
    close(m_socket);
    exit(0);
}

void TcpServer::startListen()
{
    if (listen(m_socket, 20) < 0) {
        std::cout << "Socket listen failed" << std::endl;
        exit(1);
    }

    std::cout << "*** Listening on Address: " << inet_ntoa(m_socket_address.sin_addr) << " Port: " << ntohs(m_socket_address.sin_port) << " ***\n" << std::endl;

    while (1)
    {
        std::cout << "===== Waiting for a new connection =====" << std::endl;

        int new_socket = 0;

        acceptConnection(*(&new_socket));

        char buffer[BUFFER_SIZE] = {0};
        int bytesReceived = read(new_socket, buffer, BUFFER_SIZE);

        if (bytesReceived < 0) {
            std::cout << "Failed to read bytes from client socket connection" << std::endl;
            exit(1);
        }

        std::cout << "----- Received Request from client -----" << std::endl;

        std::string response = buildResponse();

        sendResponse(*(&new_socket), *(&response));

        close(new_socket);
    }
    
}

void TcpServer::acceptConnection(int &new_socket)
{
    new_socket = accept(m_socket, (sockaddr *) &m_socket_address, &m_socket_address_len);

    if (new_socket < 0) {
        std::cout << "Server failed to accept incoming connection from Address: " << inet_ntoa(m_socket_address.sin_addr) << "; PORT: " << ntohs(m_socket_address.sin_port) << std::endl;
        exit(1);
    }
}

std::string TcpServer::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;

    return ss.str();
}

void TcpServer::sendResponse(int &new_socket, std::string &response)
{
    long bytesSent;

    bytesSent = write(new_socket, response.c_str(), response.size());

    if (bytesSent == response.size())
    {
        std::cout << "----- Server Response sent to client -----" << std::endl;
    } else
    {
        std::cout << "Error sending response to client" << std::endl;
    }
}