#pragma once

#include <iostream>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib") /*creating a link to the winsock library to get socket functionality by telling the linker*/


#include "pch.h"
//platfrom detection
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(WIN32)
#define PLATFORM PLATFORM_WINDOWS
#endif

/*including the appropriate libraries for each OS */
#if PLATFORM == PLATFORM_WINDOWS
#endif

#define NETSOCKET_EXPORTS

//do we want to import a dll in or mark it for use in other dlls or applications
#ifdef NETSOCKET_EXPORTS
#define NETSOCKET_API __declspec(dllexport)
#else
#define NETSOCKET_API __declspec(dllimport)
#endif



/*activate the socket layer -> this is required for winsock2 to work */
extern "C" NETSOCKET_API bool InitializeSockets();

/*shuts down all sockets no matter how many threads are across it*/
extern "C" NETSOCKET_API void ShutdownSockets();

