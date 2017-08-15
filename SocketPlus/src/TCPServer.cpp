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

#include "../include/TCPServer.h"

namespace RS::Network::SocketPlus
{
    
    TCPServer::TCPServer(SocketDomain domain, i32 protocol) : 
        TCPClientServerBase(domain, protocol),
        mClientSocketFileDescriptor(-1)
    {            
    }

    TCPServer::~TCPServer(void)
    {
    }    

    void TCPServer::bind(const sockaddr* address, socklen_t addressLength)
    {
        CHECK_FOR_ERROR(::bind(mSocketFileDescriptor, address, addressLength), "bind() : ");
    }

    void TCPServer::bind(ui32 portNumber)
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

    void TCPServer::listen(ui32 backlog)
    {
        CHECK_FOR_ERROR(::listen(mSocketFileDescriptor, backlog), "listen() : ");
    }

    i32 TCPServer::accept(sockaddr* address, socklen_t* addressLength)
    {
        const int result = ::accept(mSocketFileDescriptor, address, addressLength);
        CHECK_FOR_ERROR(result, "accept() : ");
        
        return result;
    }

    i32  TCPServer::accept(void)
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

    void TCPServer::start(i32 portNumber, i32 backLogSize)
    {
        if(mDomain == SocketDomain::INET6)
        {
            constexpr i32 optionOn = 1;
            setSocketOption(IPPROTO_IPV6, IPV6_V6ONLY, (char*)&optionOn, sizeof(optionOn));
        }

        bind(portNumber);                
        
        listen(backLogSize);
        
        mClientSocketFileDescriptor = accept();

        mTargetSocketFileDescriptor = mClientSocketFileDescriptor;
        mClientAddress = getPeerAddress(mClientSocketFileDescriptor);        
    }

    const std::string& TCPServer::getClientAddress()
    {
        return mClientAddress;
    }
}