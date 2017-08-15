/*
MIT License

Copyright (c) 2017 Davood Rasti & Alireza Rasti - rastisoft

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

#include <iostream>
#include "../common/common.h"
#include "../../SocketPlus/include/TCPClient.h"


using namespace RS;
using namespace RS::Network::SocketPlus;

int main(int argc, char* argv[])
{
    auto printErrorAndUsage = [argv](const char* errorMessage = NULL) -> void 
    {
        if(errorMessage)
            std::cerr << "\033[1;31mError: " << errorMessage << "\033[0m\n";
        std::cerr << "\n Usage: " << argv[0] << " <host> <port number>\n";
        std::cerr << "     host : host name or IP (e.g localhost or 127.0.0.1).\n";
        std::cerr << "     port number : between 1024 to 65535.\n";
        std::cerr << " example: " << argv[0] << " 55555\n";
        exit(0);
    };

    //Checks if number of command line arguments is correct.
    if (argc != 3)
        printErrorAndUsage();

    const std::string serverAddress(argv[1]);

    //Gets port number.
    const ui32 portNumber = [&]()
    {
        if(isNumber(argv[2]))
            return std::stoi(argv[2]);

        printErrorAndUsage("Port number should be a number!");
    }();

    //Checks if port number is in the required range.
    if(portNumber < 1024 || portNumber > 65535)
        printErrorAndUsage("Invalid port number!");

    std::cout << "\n\033[1;36m****************Client****************\033[0m\n\n";

    try
    {
        //Creates tcp client object with IPv4 address domain. (SocketDomain::INET6 for IPv6)
        TCPClient client(SocketDomain::INET6);
    
        //Connects to the server.
        client.connectTo(serverAddress, portNumber);
        std::cout << "\033[1;34m I have connected to the server\033[0m.\n";

        std::string message;
        std::string receivedString;
        for(;;)
        {
            //Gets the user message.
            std::cout << "\033[1;34m Say something (bye to exit)> \033[0m";
            std::getline (std::cin, message);
            
            //Sends the user message to the server.
            client.send(message);

            //Exits the loop if the user says bye.
            if(message == "bye")
                break;
            
            //Receives the server reply.
            client.receive(receivedString);
            std::cout << "\033[1;33m The server replied : \033[0m" << receivedString << "\n";
        }
    }
    catch(Exception& exception)
    {
        std::cout << exception.what();
    }

    return 0;
}