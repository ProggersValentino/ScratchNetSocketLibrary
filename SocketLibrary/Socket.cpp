#include "pch.h"
#include "Socket.h"
#include <iostream>
#include <winsock.h>
#include <ws2tcpip.h>



Socket::Socket()
{
    handle = 0;
}

Socket::~Socket()
{
}

SOCKET Socket::GetSocket()
{
    return handle;
}

bool Socket::OpenSock(unsigned short port, bool bindSock)
{
    /*socket creation*/
    handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (handle <= 0)
    {
        std::cout << "Socket creation failed." << std::endl;
        return false;
    }

    //do not have this as a pointer otherwise it will fail to bind
    sockaddr_in inetAddr;
    int len = sizeof(inetAddr);

    inetAddr.sin_family = AF_INET;
    inetAddr.sin_addr.s_addr = htonl(INADDR_ANY); /*loopback ip address*/
    inetAddr.sin_port = htons(port);


    /*
        set socket to non block as by default its turned which means that recvfrom function will not return until a packet is available to read
         * which ofc is not suitable when we need to simulate 60 frames per second therefore it would be to slow
         /*#1#*/
    DWORD nonBlocking = 1;

    if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
    {
        std::cout << "ioctlsocket failed. failed to set socket to non-blocking" << std::endl;
        return false;
    }

    if (!bindSock) //for the client if we dont want to bind the socket
    {
        return true;
    }

    /*if (!bindSock) return true; //early exit as we dont need to make not blocking*/
    int bounded = bind(handle, (SOCKADDR*)&inetAddr, len);

    /*binding the address to the socket we created making the socket's destination implicite in future calls*/
    if (bounded == SOCKET_ERROR)
    {
        std::cout << "Socket bind failed." << std::endl;
        std::cout << "Error: " << WSAGetLastError() << std::endl;
        return false;
    }
    else
    {
        printf("socket bound");
    }

   


    return true;
}

void Socket::Close()
{
    /*clos the socket*/
#if PLATFORM == PLATFORM_WINDOWS
    closesocket(handle);
#endif
}

bool Socket::IsOpen()
{
    return handle != 0;
}

bool Socket::Send(const Address& destination, const void* data, int size)
{

    sockaddr_in address;
    /*settings for the address we want to send to*/
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(destination.getAddress()); /*loopback ip address*/
    address.sin_port = htons(destination.getPort());

    /*sending a packet to a specific address*/
    int sent_bytes = sendto(handle, (const char*)data, size, 0, (SOCKADDR*)&address, sizeof(address));

    if (sent_bytes != size)
    {
        int error = WSAGetLastError();
        printf("failed to send data to socket, error: %d\n", error);

        return false;
    }
    else
    {
        printf("successfully sent data to socket\n");
    }

    return true;
}

int Socket::Receive(Address& sender, void* data, int size)
{
    sockaddr_in& from = sender.sockAddr; //we dont wnat to create a copy of sockadd_in as we need the information later
    int from_len = sizeof(from);

    /*printf("receiving data from socket\n");*/

    //recieve data from sendto function
    int bytes = recvfrom(handle, (char*)data, size, 0, (SOCKADDR*)&from, &from_len);
    int error = WSAGetLastError();

    if (bytes == SOCKET_ERROR && error != 10054 && error != 10035) //we ignore 10054 as its normal in UDP connections as well as 10035 as we have set it as nonblocking and are waiting for data
    {
        printf("Failed to recieve message under this error: %d", error);
        return 0;
    }

    if (bytes > 0)
    {


        /*unsigned int from_address =
                ntohl( from->sin_addr.s_addr );

        unsigned int from_port =
            ntohs( from->sin_port);*/

            /*printf("we got message from port: %d and address: %d", from_port, from_address);*/


    }

    return bytes;
}


extern "C" {
    Socket* RetrieveSocket()
    {
        return new Socket();
    }

    bool OpenSock(Socket* s, unsigned short port, bool bindSock)
    {
        return s->OpenSock(port, bindSock);
    }

    void Close(Socket* s)
    {
        s->Close();
    } /*close the socket*/

    bool IsOpen(Socket* s)
    {
        return s->IsOpen();
    }

    bool SendToFromSocket(Socket* s, const Address& destination, const void* data, int size)
    {
        return s->Send(destination, data, size);
    }

    int Receive(Socket* s, Address& sender, void* data, int size)
    {
        return s->Receive(sender, data, size);
    }


}


