//
// Created by IHEB on 4/2/2020.
//

#ifndef HZDR_DEMO_SERVER_SERVER_H
#define HZDR_DEMO_SERVER_SERVER_H




#include <cstdint>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <functional>

//!!!!! type byte in TDM-GCC-32/include/rpcndr.h may need to be commented because of conflict with std::byte
#include <windows.h>
#include <winsock2.h>

#include <ws2tcpip.h>

#define INPUT_BUFFER_SIZE 100 //test: 100 bytes of buffer
#define DEFAULT_PORT 9034


class Server
{
public:
    Server();
    Server(int port);
    virtual ~Server();

    int shutdown_();
    void close(int& fd);
    void init();
    void loop();

    //callback setters
    void onConnect(std::function<void(int)> ncc);
    void onDisconnect(std::function<void(int)> dc);
    void onInput(std::function<void(int fd, char *buffer)> rc); //TODO: change to onReceive
    template<typename T>
    int sendMessage(int source_fd, T messageBuffer, int len, int flags);
    void snapOut(); //used to snap out of listening to terminate the worker

private:
    //fd_set file descriptor sets for use with FD_ macros
    fd_set masterfds;
    fd_set tempfds;

    //unsigned integer to keep track of maximum fd value, required for select()
    uint16_t maxfd;

    //socket file descriptors
    int mastersocket_fd; //master socket which receives new connections
    int tempsocket_fd; //temporary socket file descriptor which holds new clients

    //client connection data
    struct sockaddr_storage client_addr;
    //server socket details
    struct sockaddr_in servaddr;
    //input buffer
    char input_buffer[INPUT_BUFFER_SIZE];

//    char remote_ip[INET6_ADDRSTRLEN];

    std::function<void(int fd, char *buffer)> receiveCallback;
    std::function<void(int)> newConnectionCallback;
    std::function<void(int)> disconnectCallback;

    //function prototypes
    void setup(int port);
    void initializeSocket();
    void bindSocket();
    void startListen();
    void handleNewConnection();
    void recvInputFromExisting(int fd);

    //void *getInetAddr(struct sockaddr *saddr);


};

Server::Server()
{
    setup(DEFAULT_PORT);
}

Server::Server(int port)
{
    setup(port);
}

Server::~Server()
{
    close(mastersocket_fd);
}

void Server::setup(int port)
{
    // initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2,2);
    auto wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0){
        perror("Can't initialize winsock! Quiting");
    }
    mastersocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (mastersocket_fd == INVALID_SOCKET) {
        perror("Socket creation failed");
    }

    FD_ZERO(&masterfds);
    FD_ZERO(&tempfds);

    memset(&servaddr, 0, sizeof (servaddr)); //bzero
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);
    memset(input_buffer, 0, INPUT_BUFFER_SIZE); //zero buffer //bzero
//    bzero(input_buffer,INPUT_BUFFER_SIZE); //zero the input buffer before use to avoid random data appearing in first receives
}

void Server::initializeSocket()
{
    int opt_value = 1;
    int ret_test = setsockopt(mastersocket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt_value, sizeof (int));
    if (ret_test < 0) {
        perror("[SERVER] [ERROR] setsockopt() failed");
        shutdown_();
    }
}

void Server::bindSocket()
{
    int bind_ret = bind(mastersocket_fd, (struct sockaddr*) &servaddr, sizeof (servaddr));
    if (bind_ret < 0) {
        perror("[SERVER] [ERROR] bind() failed");
    }
    FD_SET(mastersocket_fd, &masterfds); //insert the master socket file-descriptor into the master fd-set
    maxfd = mastersocket_fd; //set the current known maximum file descriptor count
}

void Server::startListen()
{
    int listen_ret = listen(mastersocket_fd, 3);
    if (listen_ret < 0) {
        perror("[SERVER] [ERROR] listen() failed");
    }
}



int Server::shutdown_()
{
    std::cout << "shutting down\n";
    socklen_t addrlen = sizeof (client_addr);
    connect(mastersocket_fd, (struct sockaddr*) &client_addr, addrlen);
    for (int i = 0; i <= maxfd; i++) {
        this->close(i);
    }
    closesocket(tempsocket_fd);
    closesocket(mastersocket_fd);
    return WSACleanup();;
}

void Server::close(int& fd){
    shutdown(fd, SD_BOTH);
}

void Server::handleNewConnection()
{
    socklen_t addrlen = sizeof (client_addr);
    tempsocket_fd = accept(mastersocket_fd, (struct sockaddr*) &client_addr, &addrlen);

    if (tempsocket_fd < 0) {
        perror("[SERVER] [ERROR] accept() failed");
    } else {
        FD_SET(tempsocket_fd, &masterfds);
        //increment the maximum known file descriptor (select() needs it)
        if (tempsocket_fd > maxfd) {
            maxfd = tempsocket_fd;
        }
    }
    newConnectionCallback(tempsocket_fd); //call the callback
}

void Server::recvInputFromExisting(int fd)
{
    int nbytesrecv = recv(fd, input_buffer, INPUT_BUFFER_SIZE, 0);
    if (nbytesrecv <= 0)
    {
        //problem
        if (0 == nbytesrecv)
        {
            disconnectCallback((uint16_t)fd);
            close(fd); //well then, bye bye.
            FD_CLR(fd, &masterfds);
            return;
        } else
        {
            perror("[SERVER] [ERROR] recv() failed");
        }
        close(fd); //close connection to client
        FD_CLR(fd, &masterfds); //clear the client fd from fd set
        return;
    }
    receiveCallback(fd, input_buffer);
    memset(&input_buffer, 0, INPUT_BUFFER_SIZE); //zero buffer //bzero
//    bzero(&input_buffer,INPUT_BUFFER_SIZE); //clear input buffer
}

void Server::loop()
{
    tempfds = masterfds; //copy fd_set for select()
    int sel = select(maxfd + 1, &tempfds, NULL, NULL, NULL); //blocks until activity

    if (sel < 0) {
        perror("[SERVER] [ERROR] select() failed");
        shutdown_();
    }
    //no problems, we're all set

    //loop the fd_set and check which socket has interactions available
    for (int i = 0; i <= maxfd; i++) {
        if (FD_ISSET(i, &tempfds)) { //if the socket has activity pending
            if (mastersocket_fd == i) {
                //new connection on master socket
                handleNewConnection();
            } else {
                //exisiting connection has new data
                recvInputFromExisting(i);
            }
        } //loop on to see if there is more
    }
}

void Server::init()
{
    initializeSocket();
    bindSocket();
    startListen();
}

void Server::onInput(std::function<void(int, char*)> rc)
{
    receiveCallback = rc;
}

void Server::onConnect(std::function<void(int)> ncc)
{
    newConnectionCallback = ncc;
}

void Server::onDisconnect(std::function<void(int)> dc)
{
    disconnectCallback = dc;
}

template<typename T>
int Server::sendMessage(int source_fd, T messageBuffer, int len, int flags) {
    int num_left = len;
    int num_sent;
    int err = 0;
    T cp = messageBuffer;
    while (num_left > 0) {
        num_sent = send(source_fd, cp, strlen(messageBuffer), flags);
        if (num_sent < 0) {
            err = SOCKET_ERROR;
            break;
        }
        assert(num_sent <= num_left);
        num_left -= num_sent;
        cp += num_sent;
    }
    return(err == SOCKET_ERROR ?  SOCKET_ERROR : len);
}

void Server::snapOut(){
    socklen_t addrlen = sizeof (client_addr);
    connect(mastersocket_fd, (struct sockaddr*) &client_addr, addrlen);
}
#endif //HZDR_DEMO_SERVER_SERVER_H
