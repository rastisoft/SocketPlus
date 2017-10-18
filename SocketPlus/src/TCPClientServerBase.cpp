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

#include "../include/TCPClientServerBase.h"
#include <cstring>

namespace RS::Network::SocketPlus
{
    TCPClientServerBase::TCPClientServerBase(SocketDomain domain, i32 protocol) : 
    SocketPlusBase(SocketType::Stream, domain, protocol)
    {            
    }

    TCPClientServerBase::~TCPClientServerBase(void)
    {
    }

    i32 TCPClientServerBase::send(i32 socketFileDescriptor, const char* data, i32 length, i32 flags)
    {
        i32 totalSentSize{0};
        i32 sentSize{0};
        while(totalSentSize < length)
        {
            sentSize = ::send(socketFileDescriptor, data + sentSize, length - sentSize, flags);
                            
            if(sentSize < 0)
                THROW_SOCKET_EXCEPTION("send() : ");
                
            totalSentSize += sentSize;
        } 
        
        return totalSentSize;
    }

    i32 TCPClientServerBase::send(i32 socketFileDescriptor, const std::string& message, i32 flags)
    {
        return send(socketFileDescriptor, (message + '\0').c_str(), message.length() + 1, flags);
    }

    i32 TCPClientServerBase::receive(i32 socketFileDescriptor, char* buffer, i32 length, i32 flags)
    {
        i32 totalReceivedSize{0};
        i32 receiveSize{0};
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

    i32 TCPClientServerBase::receive(i32 socketFileDescriptor, std::string& outString, i32 length, i32 flags)
    {
        char buffer[length] = {};
        const i32 receivedSize{receive(socketFileDescriptor, buffer, length, flags)};
        outString.assign(buffer);
        return receivedSize;
    }    
}