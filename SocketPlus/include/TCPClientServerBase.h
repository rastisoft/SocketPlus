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

#include "SocketPlusBase.h"

namespace RS::Network::SocketPlus
{
    /**
    @description: This class provides main common functions which 
    are needed by TCP classes.
    **/

    class TCPClientServerBase : public SocketPlusBase
    {
    protected:

        /**
            @description: Sends all data to the another socket.
            @param socketFileDescriptor: file descriptor of receiver socket.
            @param data: data that should be sent.
            @param length: length of the data.
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of sent byte.
        **/
        virtual i32         send(i32 socketFileDescriptor, const char* data, i32 length, i32 flags = 0);

        /**
            @description: Sends entire string to the another socket.
            @param socketFileDescriptor: file descriptor of receiver socket.
            @param message: string that should be sent.
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of sent byte.
        **/
        virtual i32         send(i32 socketFileDescriptor, const std::string& message, i32 flags = 0);

        /**
            @description: receives data from the another socket.
            @param socketFileDescriptor: file descriptor of the sender socket.
            @param buffer: keeps received data.
            @param length: length of the received data.
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of received byte.
        **/
        virtual i32         receive(i32 socketFileDescriptor, char* buffer, i32 length, i32 flags = 0);

        /**
            @description: receives string from the another socket.
            @param socketFileDescriptor: socketFileDescriptor of the sender socket.
            @param outString: keeps received string.
            @param length: length of the received data.(default = 255)
            @param flags: specifies the behaviour of the function.(default = 0).
            @return: number of received byte.
        **/
        virtual i32         receive(i32 socketFileDescriptor, std::string& outString, i32 length = 256, i32 flags = 0);
    public:
         /**
            @description: TCPClientServerBase class constructor.
            @param domain : the communication domain which can be IPv4 or IPv6.
            @param protocol : the socket protocol. (will be removed in the future updates.)
            @return
        **/
                            TCPClientServerBase(SocketDomain domain = SocketDomain::IPv4, i32 protocol = 0);
        virtual             ~TCPClientServerBase(void);        
    };
}