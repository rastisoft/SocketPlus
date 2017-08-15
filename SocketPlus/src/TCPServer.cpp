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