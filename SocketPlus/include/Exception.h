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

#include "CommonTypes.h"
#include <exception>

namespace RS
{
    class Exception : public std::exception
    {
    protected:
        
        std::string mMessage;
        ulong       mLine;
        std::string mFile;
        std::string mModule;
        std::string mFullMessage;
        i32         mErrorCode;
    public:
                                    Exception(const std::string& message = "", int errorCode = 0, const std::string& file = "", i32 line = 0, const std::string& module = "");                         
        virtual                     ~Exception(void) throw();

        virtual const std::string&  getFile();
        virtual ulong               getLine();
        virtual i32                 getErrorCode();
        virtual const std::string&  getModule();
        virtual const std::string&  getMessage();         
        
        const char*                 what() const throw();
    };

    //TODO: try inline function instead of macro.
    #define THROW_EXCEPTION(message, errorCode) throw RS::Exception(message, errorCode, __FILE__, __LINE__)    
}