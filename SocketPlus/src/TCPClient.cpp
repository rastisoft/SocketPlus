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

#include "../include/TCPClient.h"
#include <iostream>

namespace RS::Network::SocketPlus
{
    
    TCPClient::TCPClient(SocketDomain domain, i32 protocol) : 
        TCPClientServerBase(domain, protocol)
    {
    }

    TCPClient::~TCPClient(void)
    {
    }

    void TCPClient::connectTo(const std::string& address, i32 portNumber)
    {
        mPortNumber = portNumber;

        addrinfo hints{};
        addrinfo* serverAddressInfo;      
        
        //Initializing hints address info.
        // std::memset(&hints, 0, sizeof hints);
        hints.ai_family = static_cast<i32>(SocketDomain::IPv4);
        hints.ai_socktype = SOCK_STREAM;

        //Checks if host is reachable.
        auto resultCode = getaddrinfo(address.c_str(), NULL, &hints, &serverAddressInfo);
        if(resultCode != 0)
            THROW_EXCEPTION("connectTo() : Host cannot be reached! " + std::string(gai_strerror(resultCode)), resultCode);
        
        //Connects to the server based on the IP version.
        if(mDomain == SocketDomain::IPv6)
        {
            sockaddr_in6 connectionAddressIP6{};
            // std::memset(reinterpret_cast<char *>(&connectionAddressIP6), 0, sizeof(connectionAddressIP6));
            connectionAddressIP6.sin6_family = AF_INET6;
            connectionAddressIP6.sin6_port   = htons(portNumber);
            resultCode = ::connect(mSocketFileDescriptor, reinterpret_cast<struct sockaddr *>(&connectionAddressIP6), sizeof(connectionAddressIP6));
        }
        else
        {
            sockaddr_in connectionAddress{};
            // std::memset(reinterpret_cast<char *>(&connectionAddress), 0, sizeof(connectionAddress));
            connectionAddress.sin_family = AF_INET;
            connectionAddress.sin_port   = htons(portNumber);
            resultCode = ::connect(mSocketFileDescriptor, reinterpret_cast<struct sockaddr *>(&connectionAddress), sizeof(connectionAddress));
        }

        if(resultCode)
            THROW_SOCKET_EXCEPTION("connectTo() : Could not connect to the host " + address + " via port "  + std::to_string(portNumber));
    }

    i32 TCPClient::send(const std::string& message, i32 flags)
    {
        return TCPClientServerBase::send(mSocketFileDescriptor, message, flags);
    }

    i32  TCPClient::send(const char* data, i32 length, i32 flags)
    {
        return TCPClientServerBase::send(mSocketFileDescriptor, data, length, flags);
    }

    i32 TCPClient::receive(std::string& outString, i32 length, i32 flags)
    {
       return TCPClientServerBase::receive(mSocketFileDescriptor, outString, length, flags);
    }
}