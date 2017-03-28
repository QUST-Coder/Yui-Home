#include "gsock.h"

#include <cstdio>
#include <cstring>
#include <exception>
using namespace std;

namespace _internal_on_start
{
    class _init_winsock2_2_class
    {
    public:
        _init_winsock2_2_class()
        {
            /// Windows Platform need WinSock2.DLL initialization.
            #if (defined _WINSOCK2_H || defined _WINSOCK_H)
            WORD wd;
            WSAData wdt;
            wd=MAKEWORD(2,2);
            int ret=WSAStartup(wd,&wdt);
            if(ret<0)
            {
                printf("Unable to load winsock2.dll. Terminate Called.\n");
                std::terminate();
            }
            #endif
        }
        ~_init_winsock2_2_class()
        {
            /// Windows Platform need WinSock2.DLL clean up.
            #if (defined _WINSOCK2_H || defined _WINSOCK_H)
            WSACleanup();
            #endif
        }
    }_init_winsock2_2_obj;
}/// End of namespace _internal_on_start

serversock::serversock()
{
	sfd = socket(AF_INET, SOCK_STREAM, 0);
}

serversock::~serversock()
{
	closesocket(sfd);
}

int serversock::bind(int Port)
{
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(Port);
	saddr.sin_family = AF_INET;
	return ::bind(sfd, (sockaddr*)&saddr, sizeof(saddr));
}

int serversock::set_reuse()
{
	int opt = 1;
	return setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
}

int serversock::listen(int MaxCount)
{
	return ::listen(sfd, MaxCount);
}

sock serversock::accept()
{
	sock s;
	/// Linux: use socklen_t instead of int
	socklen_t tmp = sizeof(s.saddr);
	int ret = ::accept(sfd, (sockaddr*)&s.saddr, &tmp);
	if (ret<0)
	{
		s.sfd = -1;/// Bad Socket
	}
	else
	{
		s.sfd = ret;
	}
	return s;
}

sock::sock()
{
	sfd = socket(AF_INET, SOCK_STREAM, 0);
}

sock::~sock()
{
	if(sfd>=0)
    {
        closesocket(sfd);
    }
}

sock::sock(sock && tmp)
{
	sfd = tmp.sfd;
	saddr = tmp.saddr;
	tmp.sfd = -1;
}

int sock::connect(const char * IPStr, int Port)
{
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_addr.s_addr = inet_addr(IPStr);
	saddr.sin_port = htons(Port);
	saddr.sin_family = AF_INET;
	return ::connect(sfd, (sockaddr*)&saddr, sizeof(saddr));
}

int sock::send(const char * Buffer, int BufferLen)
{
	return ::send(sfd, (const char*)Buffer, BufferLen, 0);
}

int sock::recv(char * Buffer, int BufferMax)
{
	return ::recv(sfd, (char*)Buffer, BufferMax, 0);
}

/**[20161102] Now send/recv has time-out-control. */

int sock::setsendtime(int Second)
{
	struct timeval outtime;
	/// We don't know why, but on Windows, 1 Second means 1000.
#if (defined _WINSOCK2_H || defined _WINSOCK_H)
	outtime.tv_sec = Second * 1000;
	outtime.tv_usec = 0;
#else
	outtime.tv_sec = Second;
	outtime.tv_usec = 0;
#endif

	return setsockopt(sfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&outtime, sizeof(outtime));
}

int sock::setrecvtime(int Second)
{
	struct timeval outtime;
	/// We don't know why, but on Windows, 1 Second means 1000.
#if (defined _WINSOCK2_H || defined _WINSOCK_H)
	outtime.tv_sec = Second * 1000;
	outtime.tv_usec = 0;
#else
	outtime.tv_sec = Second;
	outtime.tv_usec = 0;
#endif

	return setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&outtime, sizeof(outtime));
}

int sock::getsendtime(int & _out_Second, int & _out_uSecond)
{
	struct timeval outtime;
	/// Linux: use socklen_t instead of int
	socklen_t _not_used_t;
	int ret = getsockopt(sfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&outtime, &_not_used_t);
	if (ret<0) return ret;
	/// We don't know why, but on Windows, 1 Second means 1000.
#if (defined _WINSOCK2_H || defined _WINSOCK_H)
	_out_Second = outtime.tv_sec / 1000;
	_out_uSecond = outtime.tv_usec;
#else
	_out_Second = outtime.tv_sec;
	_out_uSecond = outtime.tv_usec;
#endif

	return ret;
}

int sock::getrecvtime(int & _out_Second, int & _out_uSecond)
{
	struct timeval outtime;
	/// Linux: use socklen_t instead of int
	socklen_t _not_used_t;
	int ret = getsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&outtime, &_not_used_t);
	if (ret<0) return ret;
	/// We don't know why, but on Windows, 1 Second means 1000.
#if (defined _WINSOCK2_H || defined _WINSOCK_H)
	_out_Second = outtime.tv_sec / 1000;
	_out_uSecond = outtime.tv_usec;
#else
	_out_Second = outtime.tv_sec;
	_out_uSecond = outtime.tv_usec;
#endif

	return ret;
}

sockaddr_in sock::getaddr()
{
	return saddr;
}

sock& sock::operator = (sock&& tmp)
{
	closesocket(sfd);
	sfd = tmp.sfd;
	saddr = tmp.saddr;
	tmp.sfd = -1;
	return *this;
}
