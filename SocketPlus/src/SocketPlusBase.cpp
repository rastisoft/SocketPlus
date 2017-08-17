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
        CHECK_FOR_ERROR((mSocketFileDescriptor = socket(static_cast<i32>(mDomain), static_cast<i32>(mType), mProtocol)), "socket() : ");
    }

    SocketPlusBase::~SocketPlusBase(void)
    {
        close();
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

    std::string SocketPlusBase::getPeerAddress(i32 socketFileDescriptor)
    {
        if(mDomain == SocketDomain::IPv6)
        {
            sockaddr_in6 addressIP6;
            socklen_t addressLength = sizeof(addressIP6);
            getPeerName(socketFileDescriptor, reinterpret_cast<sockaddr *>(&addressIP6), &addressLength);
            return netToPresentation(reinterpret_cast<sockaddr *>(&addressIP6));
        }
        else
        {
            sockaddr_in address;
            socklen_t addressLength = sizeof(address);
            getPeerName(socketFileDescriptor, reinterpret_cast<sockaddr *>(&address), &addressLength);
            return netToPresentation(reinterpret_cast<sockaddr *>(&address));
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
                    return inet_ntop(AF_INET6, &(reinterpret_cast<const sockaddr_in6*>(address)->sin6_addr), buffer, bufferSize);
                case AF_INET:
                    return inet_ntop(AF_INET, &(reinterpret_cast<const sockaddr_in*>(address)->sin_addr), buffer, bufferSize);
                default:
                    THROW_EXCEPTION("netToPresentation() : Only INET and INET6 are supported.", -1);
            };
        }();

        if(ntopResult == NULL)
            THROW_SOCKET_EXCEPTION("netToPresentation() :");

        return std::string(buffer);
    }
}