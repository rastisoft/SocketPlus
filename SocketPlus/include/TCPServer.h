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

#include "TCPClientServerBase.h"

namespace RS::Network::SocketPlus
{    
    class TCPServer : public TCPClientServerBase
    {
    protected:
        i32                         mClientSocketFileDescriptor;
        std::string                 mClientAddress;
    public:
                                    TCPServer(SocketDomain domain = SocketDomain::INET, i32 protocol = 0);
        virtual                     ~TCPServer(void);

        virtual void                bind(const sockaddr* address, socklen_t addressLength); 
        virtual void                bind(ui32 portNumber);       
        virtual void                listen(ui32 backlog);
        virtual i32                 accept(sockaddr* address, socklen_t* addressLength);
        virtual i32                 accept(void);
        virtual void                start(i32 portNumber, i32 backLogSize);
        
        virtual const std::string&  getClientAddress();
    };
}