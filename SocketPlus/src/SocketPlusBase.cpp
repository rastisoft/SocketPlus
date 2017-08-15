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

#include "../include/SocketPlusBase.h"
#include <iostream>

namespace RS::Network::SocketPlus
{   
    SocketPlusBase::SocketPlusBase(SocketType type, SocketDomain domain, i32 protocol) : 
        mType(type),
        mDomain(domain),
        mProtocol(protocol)
    {
        CHECK_FOR_ERROR((mSocketFileDescriptor = socket(int(mDomain), int(mType), mProtocol)), "socket() : ");
    }

    SocketPlusBase::~SocketPlusBase(void)
    {
        close();
    }

    void SocketPlusBase::connect(const sockaddr* address, socklen_t addressLength)
    {
        CHECK_FOR_ERROR(::connect(mSocketFileDescriptor, address, addressLength), "connect() : ");            
    }

    void SocketPlusBase::bind(const sockaddr* address, socklen_t addressLength)
    {
        CHECK_FOR_ERROR(::bind(mSocketFileDescriptor, address, addressLength), "bind() : ");
    }

    void SocketPlusBase::bind(ui32 portNumber)
    {
        if(mDomain == SocketDomain::INET6)
        {
            sockaddr_in6 serverAddressIP6;
            memset(reinterpret_cast<char *>(&serverAddressIP6), 0, sizeof(serverAddressIP6));
            serverAddressIP6.sin6_family      = AF_INET6;
            serverAddressIP6.sin6_addr        = in6addr_any;
            serverAddressIP6.sin6_port        = htons(portNumber);
            bind(reinterpret_cast<struct sockaddr *>(&serverAddressIP6), sizeof(serverAddressIP6));
        }
        else
        {
            sockaddr_in serverAddress;
            memset(reinterpret_cast<char *>(&serverAddress), 0, sizeof(serverAddress));
            serverAddress.sin_family      = AF_INET;
            serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
            serverAddress.sin_port        = htons(portNumber);
            bind(reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress));
        }
    }

    void SocketPlusBase::listen(ui32 backlog)
    {
        CHECK_FOR_ERROR(::listen(mSocketFileDescriptor, backlog), "listen() : ");
    }

    i32 SocketPlusBase::accept(sockaddr* address, socklen_t* addressLength)
    {
        const int result = ::accept(mSocketFileDescriptor, address, addressLength);
        CHECK_FOR_ERROR(result, "accept() : ");
        
        return result;
    }

    i32  SocketPlusBase::accept(void)
    {
        const i32 result = [&]()
        {
            switch(mDomain)
            {
                case SocketDomain::INET6:
                {
                    sockaddr_in6 clientAddressIP6;
                    socklen_t clientLength = sizeof(clientAddressIP6);
                    return ::accept(mSocketFileDescriptor, reinterpret_cast<struct sockaddr *>(&clientAddressIP6), &clientLength);
                }
                case SocketDomain::INET:
                {
                    sockaddr_in clientAddress;
                    socklen_t clientLength = sizeof(clientAddress);
                    return ::accept(mSocketFileDescriptor, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientLength);
                }
                default:
                    THROW_EXCEPTION("accept() : Only INET and INET6 are supported.", -1);
            };
        }();

        if(result < 0)
            THROW_SOCKET_EXCEPTION("accept() :");

        return result;
    }

    i32 SocketPlusBase::send(i32 socketFileDescriptor, const char* data, i32 length, i32 flags)
    {
        i32 totalSentSize = 0;
        i32 sentSize = 0;
        while(totalSentSize < length)
        {
            sentSize = ::send(socketFileDescriptor, data + sentSize, length - sentSize, flags);
                            
            if(sentSize < 0)
                THROW_SOCKET_EXCEPTION("send() : ");
                
            totalSentSize += sentSize;
        } 
        
        return totalSentSize;
    }

    i32 SocketPlusBase::send(i32 socketFileDescriptor, const std::string& message, i32 flags)
    {
        return send(socketFileDescriptor, (message + '\0').c_str(), message.length() + 1, flags);
    }

    i32 SocketPlusBase::receive(i32 socketFileDescriptor, char* buffer, i32 length, i32 flags)
    {
        i32 totalReceivedSize = 0;
        i32 receiveSize = 0;
        while(totalReceivedSize < length)
        {
            receiveSize = recv(socketFileDescriptor, buffer + receiveSize, length - receiveSize, flags);
            
            if(receiveSize == 0 || buffer[receiveSize - 1] == '\0')
                return totalReceivedSize;
            
            if(receiveSize == -1)
            {
                if(errno != EINTR)
                    THROW_SOCKET_EXCEPTION("receive() : ");
                else
                    continue;
            }
                
            totalReceivedSize += receiveSize;
        } 
        
        return totalReceivedSize;
    }

    i32 SocketPlusBase::receive(i32 socketFileDescriptor, std::string& outString, i32 length)
    {
        char buffer[length];
        memset(buffer, 0, sizeof(buffer));
        const i32 receivedSize = receive(socketFileDescriptor, buffer, length);
        outString = std::move(std::string(buffer));
        return receivedSize;
    }

    void  SocketPlusBase::close(void)
    {
        if(mSocketFileDescriptor >= 0)
        {
            ::close(mSocketFileDescriptor);
            mSocketFileDescriptor = -1;
        }
    }

    void SocketPlusBase::setSocketOption(i32 level, i32 optionName, const char* optionValue, socklen_t optionLength)
    {
        CHECK_FOR_ERROR(setsockopt(mSocketFileDescriptor, level, optionName, optionValue, optionLength), "setSocketOption() : ");
    }

    void SocketPlusBase::getPeerName(i32 socketFileDescriptor, sockaddr* address, socklen_t* addressLength)
    {
        CHECK_FOR_ERROR(getpeername(socketFileDescriptor, address, addressLength), "getPeerName() : ");
    }    

    void SocketPlusBase::getAddressInfo(const char* node, const char* service, const addrinfo* hints, addrinfo** result)
    {
        const i32 errorCode = getaddrinfo(node, service, hints, result);
        if (errorCode != 0)
            THROW_EXCEPTION("getAddressInfo() : " + std::string(gai_strerror(errorCode)), errorCode);
    }

    std::string SocketPlusBase::getPeerAddress(i32 socketFileDescriptor)
    {
        if(mDomain == SocketDomain::INET6)
        {
            struct sockaddr_in6 addressIP6;
            socklen_t addressLength = sizeof(addressIP6);
            getPeerName(socketFileDescriptor, reinterpret_cast<struct sockaddr *>(&addressIP6), &addressLength);
            return netToPresentation(reinterpret_cast<struct sockaddr *>(&addressIP6));
        }
        else
        {
            struct sockaddr_in address;
            socklen_t addressLength = sizeof(address);
            getPeerName(socketFileDescriptor, reinterpret_cast<struct sockaddr *>(&address), &addressLength);
            return netToPresentation(reinterpret_cast<struct sockaddr *>(&address));
        }
    }

    std::string SocketPlusBase::netToPresentation(const sockaddr* address)
    {
        
        constexpr ui8 bufferSize = INET_ADDRSTRLEN;
        char buffer[bufferSize];
        
        const char* ntopResult = [&]()
        {
            switch(address->sa_family)
            {
                case AF_INET6:
                    return inet_ntop(AF_INET6, &(reinterpret_cast<const struct sockaddr_in6*>(address)->sin6_addr), buffer, bufferSize);
                case AF_INET:
                    return inet_ntop(AF_INET, &(reinterpret_cast<const struct sockaddr_in*>(address)->sin_addr), buffer, bufferSize);
                default:
                    THROW_EXCEPTION("netToPresentation() : Only INET and INET6 are supported.", -1);
            };
        }();

        if(ntopResult == NULL)
            THROW_SOCKET_EXCEPTION("netToPresentation() :");

        return std::string(buffer);
    }
}