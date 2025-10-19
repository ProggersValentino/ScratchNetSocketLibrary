#include "pch.h"
#include "Address.h"
#include <winsock2.h>

Address::Address() : sockAddr{} //zero out the socket address
{


    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(port);

    address = sockAddr.sin_addr.s_addr;
}

Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short p) : sockAddr{} //zero out the socket address
{
    unsigned int addressConverted = (a << 24) | (b << 16) | (c << 8) | d; /*creating an unsigned 32 bit int which is the format of the address*/

    address = addressConverted;
    port = p;

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(address);
    sockAddr.sin_port = htons(port);
}

Address::Address(unsigned int address, unsigned short port) : sockAddr{} //zero out the socket address
{
    this->address = address;
    this->port = port;

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(address);
    sockAddr.sin_port = htons(port);
}

Address::Address(unsigned short port) : sockAddr{} //zero out the socket address
{

    this->port = port;

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(port);

    address = sockAddr.sin_addr.s_addr;

}

unsigned int Address::getAddress() const
{
    return address;
}

sockaddr_in Address::GetSockAddrIn() const
{
    return sockAddr;
}

u_long Address::GetAddressFromSockAddrIn() const
{
    return ntohl(sockAddr.sin_addr.s_addr);
}

u_short Address::GetPortFromSockAddrIn() const
{
    return ntohs(sockAddr.sin_port);
}

unsigned char Address::GetA() const
{
    return address >> 24 & 0xFF; /*shifts the data by 24 bits and isolates the result*/
}

unsigned char Address::GetB() const
{
    return address >> 16 & 0xFF;
}

unsigned char Address::GetC() const
{
    return address >> 8 & 0xFF;
}

unsigned char Address::GetD() const
{
    return address & 0xFF;
}

unsigned short Address::getPort() const
{
    return port;
}

void Address::SetAddress(unsigned int value)
{
    address = value;
}

extern "C" {

    ADDR_API Address* CreateAddress()
    {
        return new Address();
    }

    ADDR_API Address* CreateAddressIndiIPPort(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short p)
    {
        return new Address(a, b, c, d, p);
    }

    ADDR_API Address* CreateAddressIPPort(unsigned int address, unsigned short port)
    {
        return new Address(address, port);
    }

    ADDR_API Address* CreateAddressPort(unsigned short port)
    {
        return new Address(port);
    }

    ADDR_API sockaddr_in GetSockAddrIn(Address* a)
    {
        return a->GetSockAddrIn();
    }

}

