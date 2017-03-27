/** CPPLIB#GSOCK
General Socket (CPPLIB)
Created On: 2016.5.18

Change Log:
[20160529] sockbuffer added. Now sock::send() needs only 1 parameter.
[20160630] class ksock added. K-Sock is designed to avoid problems caused by TCP.
[20160902] Generality Support Interface - Not implemented.
[20161102] Now send/recv has time-out-control.
*/

/** GSock For Yui Project
This is a special version of CPPLIB#GSOCK.
*/

#ifdef cpplib_socket
#error "CPPLIB#GSOCK is incompatible with CPPLIB#SOCK"
#endif

#ifdef cpplib_gsock
#error "This Version of GSock is incompatible with CPPLIB#GSOCK"
#endif

#ifndef _yui_project_gsock_h
#define _yui_project_gsock_h

#ifdef _WIN32 /// Windows
#include <winsock2.h>
typedef int socklen_t;
#else /// Linux
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define closesocket close
using BYTE = unsigned char;
#endif

#include <thread>
#include <cstdio>
#include <cstring>
using namespace std;

#if defined(DEBUG) && !defined(dprintf)
#define dprintf(format,args...) printf("Debug Output: ");printf(format,##args)
#elif !defined(dprintf)
#define dprintf(format,args...)
#endif // DEBUG

class sock
{
public:
	sock();
	~sock();
	sock(sock&& tmp);
	sock& operator = (sock&& tmp);
	sock(const sock&) = delete;
	sock& operator = (const sock&) = delete;
	int connect(const char* IPStr, int Port);

	/**Generality Support Interface*/
	template<typename T>
	int send(T& inc);
	template<typename T>
	int recv(T& inc);

	int send(const char* Buffer, int BufferLen);
	int recv(char* Buffer, int BufferMax);

	/**[20161102] Now send/recv has time-out-control. */
	int setsendtime(int Second);
	int setrecvtime(int Second);

	int getsendtime(int& _out_Second, int& _out_uSecond);
	int getrecvtime(int& _out_Second, int& _out_uSecond);

	sockaddr_in getaddr();

protected:
	int sfd;
	sockaddr_in saddr;
	friend class serversock;
};

class serversock
{
public:
	serversock();
	~serversock();
	int bind(int Port);
	int set_reuse();
	int listen(int MaxCount);
	sock accept();
protected:
	int sfd;
	sockaddr_in saddr;
};



#endif // End of _yui_project_gsock_h
