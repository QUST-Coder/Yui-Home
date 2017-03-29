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

    bool ready();
private:
    std::shared_ptr<sock> sp;
};

class ServerListener
{
public:
    ServerListener();
    ServerListener(int Port,int ListenQueueLength);
    bool ready();
private:
    std::shared_ptr<serversock> sp;
    int _status;
};
