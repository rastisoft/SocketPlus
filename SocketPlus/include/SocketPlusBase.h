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
        INET = AF_INET,
        INET6 = AF_INET6
    };

    enum class SocketType
    {
        STREAM = SOCK_STREAM,
        DGRAM = SOCK_DGRAM,
        SEQPACKET = SOCK_SEQPACKET,
        RAW = SOCK_RAW,
        RDM = SOCK_RDM,
        PACKET = SOCK_PACKET,
        NONBLOCK = SOCK_NONBLOCK,
        CLOEXEC = SOCK_CLOEXEC
    };

    /**
    @description: This a base class that provides main common functions which 
    are needed by all derived classes.
    **/

    class SocketPlusBase
    {
    protected:
        
        SocketDomain        mDomain;
        SocketType          mType;
        i32                 mProtocol;
        i32                 mSocketFileDescriptor;        
        
        virtual void        getPeerName(i32 socketFileDescriptor, sockaddr* address, socklen_t* addressLength);
        virtual void        setSocketOption(i32 level, i32 optionName, const char* optionValue, socklen_t optionLength);
        virtual std::string getPeerAddress(i32 socketFileDescriptor);
        virtual std::string netToPresentation(const sockaddr* address);
        
    public:
                            SocketPlusBase(SocketType type, SocketDomain domain = SocketDomain::INET, i32 protocol = 0);
        virtual             ~SocketPlusBase(void);       

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