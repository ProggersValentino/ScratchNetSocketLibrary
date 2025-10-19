#pragma once

#include "pch.h"
#include <winsock2.h>
#include <ws2def.h>
#include <iostream>

#define ADDR_EXPORTS

#ifdef ADDR_EXPORTS
#define ADDR_API __declspec(dllexport)
#else
#define ADDR_API  __declspec(dllimport)
#endif

/*all address related actions happen here from address creation to deletion*/
class ADDR_API Address
{
public:

    /*initialize with default callback address*/
    Address();

    Address(unsigned char a,
        unsigned char b,
        unsigned char c,
        unsigned char d,
        unsigned short p);

    Address(unsigned int address,
        unsigned short port);

    Address(unsigned short port);

    /*get individual address numbers*/
    unsigned char GetA() const;
    unsigned char GetB() const;
    unsigned char GetC() const;
    unsigned char GetD() const;

    unsigned int getAddress() const;

    sockaddr_in GetSockAddrIn() const;

    u_long GetAddressFromSockAddrIn() const;
    u_short GetPortFromSockAddrIn() const;

    sockaddr_in sockAddr;

    /*get port*/
    unsigned short getPort() const;

    void SetAddress(unsigned int value);

    bool operator==(const Address& other) const
    {
        return GetAddressFromSockAddrIn() == other.GetAddressFromSockAddrIn()
            && GetPortFromSockAddrIn() == other.GetPortFromSockAddrIn();
    }

private:
    unsigned int address;
    unsigned short port = 30000;

};

//creating C based interface wrapper to wrap our c++ functions to ensure they can be used else where
extern "C" {

    ADDR_API Address* CreateAddress();

    ADDR_API Address* CreateAddressIndiIPPort(unsigned char a,
        unsigned char b,
        unsigned char c,
        unsigned char d,
        unsigned short p);

    ADDR_API Address* CreateAddressIPPort(unsigned int address,
        unsigned short port);
    
    ADDR_API Address* CreateAddressPort(unsigned short port);

    ADDR_API sockaddr_in GetSockAddrIn(Address* a);
}