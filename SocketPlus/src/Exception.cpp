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

#include "../include/Exception.h"

namespace RS
{
    Exception::Exception(const std::string& message, i32 errorCode, const std::string& file, i32 line, const std::string& module) 
        : mMessage(message), mErrorCode(errorCode), mLine(line), mFile(file), mModule(module)
    {
        mFullMessage = "Error(" + std::to_string(mErrorCode) + ") in file '" + mFile + "' in line " + std::to_string(mLine) + " : " + mMessage;
    }

    Exception::~Exception() throw()
    {
    }

    const std::string& Exception::getFile()
    {
        return mFile;
    }

    ulong Exception::getLine()
    {
        return mLine;
    }

    i32 Exception::getErrorCode()
    {
        return mErrorCode;
    }

    const std::string& Exception::getModule()
    {
        return mModule;
    }

    const std::string& Exception::getMessage()
    {
        return mMessage;
    }

    const char* Exception::what() const throw()
    {        
        return mFullMessage.c_str();
    }
}