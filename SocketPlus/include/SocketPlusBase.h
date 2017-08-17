/*
MIT License

Copyright (c) 2017 Davood Rasti & Alireza Rasti - Rastisoft

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "CommonTypes.h"
#include "../include/Exception.h"

namespace RS::Network::SocketPlus
{   
    enum class SocketDomain
    {
        IPv4 = AF_INET,
        IPv6 = AF_INET6
    };

    enum class SocketType
    {
        Stream = SOCK_STREAM,
        Datagram = SOCK_DGRAM
    };

    /**
    @description: This a base class that provides main common functions which 
    are needed by all derived classes.
    **/

    class SocketPlusBase
    {
    protected:
        
        SocketDomain        mDomain; //Stores the socket domain.
        SocketType          mType; //Stores the socket type.
        i32                 mProtocol; //Stores the socket protocol.
        i32                 mSocketFileDescriptor; //Stores the socketFileDescriptor created by socket().
        
        virtual void        getPeerName(i32 socketFileDescriptor, sockaddr* address, socklen_t* addressLength);
        virtual void        setSocketOption(i32 level, i32 optionName, const char* optionValue, socklen_t optionLength);
        
        /**
            @description: Converts an network address to readable format.
            @param address: network address.
            @return: address in readable form.
        **/
        virtual std::string netToPresentation(const sockaddr* address);

        /**
            @description: Returns the peer connected address.
            @param socketFileDescriptor: the socketFileDescriptor of the peer socket.
            @return: peer address.
        **/
        virtual std::string getPeerAddress(i32 socketFileDescriptor);

    public:
        /**
            @description: SocketPlusBase class constructor.
            @param type: the type of the socket which can be SocketType::Stream (for connection-based) or 
                         SocketType::Stream (for connectionless).
            @param domain : the communication domain which can be IPv4 or IPv6.
            @param protocol : the socket protocol. (will be removed in the future updates.)
            @return
        **/
                            SocketPlusBase(SocketType type = SocketType::Stream, SocketDomain domain = SocketDomain::IPv4, i32 protocol = 0);
        virtual             ~SocketPlusBase(void);       

         /**
            @description: Closes the socket.
            @return void.
        **/
        virtual void        close(void);        
    };

    static RS_INLINE void checkForError(i32 resultCode, const std::string commandName, const std::string file, ui32 line)
    {
        if(resultCode < 0)
            throw RS::Exception(commandName + std::string(strerror(errno)), errno, file, line);
    };

    #define THROW_SOCKET_EXCEPTION(message) throw RS::Exception(message + std::string(strerror(errno)), errno, __FILE__, __LINE__)
    #define CHECK_FOR_ERROR(resultCode, message) if(resultCode < 0) THROW_SOCKET_EXCEPTION(message);
}