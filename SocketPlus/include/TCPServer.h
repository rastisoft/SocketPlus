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
        i32                         mClientSocketFileDescriptor; //Keeps the socketFileDescriptor of the connected client.
        std::string                 mClientAddress; //Keeps the address of the connected client.

        virtual void                bind(const sockaddr* address, socklen_t addressLength); 
        virtual void                bind(ui32 portNumber);       
        virtual void                listen(ui32 backlog);
        virtual i32                 accept(sockaddr* address, socklen_t* addressLength);
        virtual i32                 accept(void);
    public:
        /**
            @description: TCPServer class constructor.
            @param domain : the communication domain which can be IPv4 or IPv6.
            @param protocol : the socket protocol. (will be removed in future updates.)
            @return
        **/
                                    TCPServer(SocketDomain domain = SocketDomain::IPv4, i32 protocol = 0);
        virtual                     ~TCPServer(void);       

        /**
            @description: Sends entire string to the connected client.
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of sent byte.
        **/
        virtual i32                 send(const std::string& message, i32 flags = 0);

        /**
            @description: Sends all data to the connected client.
            @param data: data that should be sent.
            @param length: length of the data.
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of sent byte.
        **/
        virtual i32                 send(const char* data, i32 length, i32 flags = 0);

        /**
            @description: receives string from the connected client.
            @param outString: keeps received string.
            @param length: length of the received data.(default = 256)
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of received byte.
        **/
        virtual i32                 receive(std::string& outString, i32 length = 256, i32 flags = 0);

        /**
            @description: starts the server. The server start listening on the specified port number.
            @param portNumber : the port number that socket should use.
            @param backLogSize: maximum number of waiting connection in the queue before accepted by server(default = 64) 
            @return: void.
        **/
        virtual void                start(i32 portNumber, i32 backLogSize = 64);
        
        /**
            @description: Returns the connected client address.
            @param portNumber : the port number that socket should use.
            @param backLogSize: maximum number of waiting connection in the queue before accepted by server(default = 64) 
            @return: void.
        **/
        virtual const std::string&  getClientAddress(void);
    };
}