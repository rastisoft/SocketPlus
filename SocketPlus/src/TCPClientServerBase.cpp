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

namespace RS::Network::SocketPlus
{
    TCPClientServerBase::TCPClientServerBase(SocketDomain domain, i32 protocol) : 
    SocketPlusBase(SocketType::STREAM, domain, protocol),
        mTargetSocketFileDescriptor(-1)
    {            
    }

    TCPClientServerBase::~TCPClientServerBase(void)
    {
    }

    i32 TCPClientServerBase::send(const std::string& message)
    {
        return SocketPlusBase::send(mTargetSocketFileDescriptor, message);
    }

    i32  TCPClientServerBase::send(const char* data, i32 length)
    {
        return SocketPlusBase::send(mTargetSocketFileDescriptor, data, length);
    }

    i32 TCPClientServerBase::receive(std::string& outString, i32 length)
    {
       return SocketPlusBase::receive(mTargetSocketFileDescriptor, outString, length);
    }
}