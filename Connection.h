#pragma once
#include "gsock.h"
#include <string>
#include <memory>

class Connection
{
public:
    int connect(std::string IP,int Port);

    int send(std::string text);
    int recv(std::string& text);
    int send(int val);
    int recv(int& val);
    int send(const void* ptr,int len);
    int recv(void* ptr,int len);

    int relaxsend(const void* ptr,int len);
    int relaxrecv(void* ptr,int len);

    bool ready();
private:
    friend class ServerListener;
    std::shared_ptr<sock> sp;
};

class ServerListener
{
public:
    ServerListener();
    ServerListener(int Port,int ListenQueueLength);
    bool ready();
    Connection accept();
private:
    std::shared_ptr<serversock> sp;
    int _status;
};
