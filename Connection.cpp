#include "Connection.h"
using namespace std;

int Connection::connect(string IP,int Port)
{
    sock* ps=new sock();
    if(ps==nullptr) return -1;
    sp.reset(ps);
    return ps->connect(IP.c_str(),Port);
}

int Connection::send(string text)
{
    int ret=send(text.size());
    if(ret<=0) return ret;
    ret=send(text.c_str(),text.size());
    return ret;
}

int Connection::recv(string& text)
{
    int len;
    int ret=recv(len);
    if(ret<=0) return ret;
    char buff[1032];
    string ans;

    int recved=0;
    while(recved<len)
    {
        memset(buff,0,1032);
        ret=recv(buff,min(len-recved,1024));
        /// FIXME: if ret==0, what should we do?
        if(ret<0) return ret;
        recved+=ret;
        ans.append(string(buff));
    }

    text=ans;

    return len;
}

int Connection::send(int val)
{
    int nval=htonl(val);
    return send(&nval,sizeof(nval));
}

int Connection::recv(int& val)
{
    int nval=0;
    int ret=recv(&nval,sizeof(nval));
    val=ntohl(nval);
    return ret;
}

int Connection::relaxsend(const void* ptr,int len)
{
    return sp.get()->send((const char*)ptr,len);
}

int Connection::send(const void* ptr,int len)
{
    if(sp.get()==nullptr) return -2;
    int sent=0;
    int total=len;
    int ret;
    while(sent<total)
    {
        ret=relaxsend((const char*)ptr+sent,total-sent);
        /// FIXME: How to return both sent bytes and error code?
        if(ret<=0) return ret;
        else sent+=ret;
    }
    return sent;
}

int Connection::relaxrecv(void* ptr,int len)
{
    return sp.get()->recv((char*)ptr,len);
}

int Connection::recv(void* ptr,int len)
{
    if(sp.get()==nullptr) return -2;
    int recved=0;
    int total=len;
    int ret;
    while(recved<total)
    {
        ret=relaxrecv((char*)ptr+recved,total-recved);
        /// FIXME: How to return both sent bytes and error code?
        if(ret<=0) return ret;
        else recved+=ret;
    }
    return recved;
}

bool Connection::ready()
{
    return sp.get()!=nullptr;
}



ServerListener::ServerListener()
{
    _status=-1;
}

ServerListener::ServerListener(int Port,int ListenQueueLength)
{
    _status=-1;
    serversock* ss=new serversock;
    if(ss==nullptr) return;
    shared_ptr<serversock> tmp;
    tmp.reset(ss);

    _status=0;
    int ret=ss->bind(Port);
    if(ret<0) return;
    _status=1;
    ret=ss->listen(ListenQueueLength);
    if(ret<0) return;
    _status=2;

    sp=tmp;
}

bool ServerListener::ready()
{
    return (sp.get()!=nullptr)&&(_status==2);
}

Connection ServerListener::accept()
{
    sock* s=new sock(sp.get()->accept());
    Connection c;
    c.sp.reset(s);
    return c;
}
