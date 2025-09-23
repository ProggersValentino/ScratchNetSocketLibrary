#pragma once
#include <winsock2.h>

#include "Address.h"
#include <Ws2tcpip.h>
#include "pch.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#if defined(WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#define SOCKET_EXPORTS

//do we want to import a dll in or mark it for use in other dlls or applications
#ifdef SOCKET_EXPORTS
#define SOCKET_API __declspec(dllexport)
#else
#define SOCKET_API __declspec(dllimport)
#endif


class SOCKET_API Socket
{
public:
    Socket();
    ~Socket();

    SOCKET GetSocket();

    bool OpenSock(unsigned short port, bool bindSock); /*open and bind new socket to a given port*/
    void Close(); /*close the socket*/

  

    bool IsOpen();

    /*send packets to a destination*/
    bool Send(const Address& destination, const void* data, int size);

    /*retrieve packets from sender*/
    int Receive(Address& sender, void* data, int size);

private:
    SOCKET handle = 0;
};

//creating C based interface wrapper to wrap our c++ functions to ensure they can be used else where
extern "C" { 
    SOCKET_API Socket* RetrieveSocket();

    SOCKET_API bool OpenSock(Socket* s, unsigned short port, bool bindSock); /*open and bind new socket to a given port*/

    SOCKET_API void Close(Socket* s);

    SOCKET_API bool IsOpen(Socket* s);
    
    /*send packets to a destination*/
    SOCKET_API bool SendToFromSocket(Socket* s, const Address& destination, const void* data, int size);

    /*retrieve packets from sender*/
    SOCKET_API int Receive(Socket* s, Address& sender, void* data, int size);
}