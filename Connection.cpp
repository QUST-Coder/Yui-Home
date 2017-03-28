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

int Connection::send(const void* ptr,int len)
{
    if(sp.get()==nullptr) return -2;
    int sent=0;
    int total=len;
    int ret;
    while(sent<total)
    {
        ret=sp.get()->send((const char*)ptr+sent,total-sent);
        /// FIXME: How to return both sent bytes and error code?
        if(ret<=0) return ret;
        else sent+=ret;
    }
    return sent;
}

int Connection::recv(void* ptr,int len)
{
    if(sp.get()==nullptr) return -2;
    int recved=0;
    int total=len;
    int ret;
    while(recved<total)
    {
        ret=sp.get()->recv((char*)ptr+recved,total-recved);
        /// FIXME: How to return both sent bytes and error code?
        if(ret<=0) return ret;
        else recved+=ret;
    }
    return recved;
}
