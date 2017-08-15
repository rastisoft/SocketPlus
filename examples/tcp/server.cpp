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
#include <string>
#include <array>
#include <random>
#include "../common/common.h"
#include "../../SocketPlus/include/TCPServer.h"

using namespace RS;
using namespace RS::Network::SocketPlus;

int main(int argc, char* argv[])
{
    auto printErrorAndUsage = [argv](const char* errorMessage = NULL) -> void 
    {
        if(errorMessage)
            std::cerr << "\033[1;31mError: " << errorMessage << "\033[0m\n";
        std::cerr << "\n Usage: " << argv[0] << " <Port number>\n";
        std::cerr << "     Port number : between 1024 to 65535.\n";
        std::cerr << " example: " << argv[0] << " 55555\n";
        exit(0);
    };

    //Checks if number of command line arguments is correct.
    if (argc != 2)
        printErrorAndUsage();
    
    //Gets port number.
    const ui32 portNumber = [&]()
    {
        if(isNumber(argv[1]))
            return std::stoi(argv[1]);

        printErrorAndUsage("Port number shoud be a number!");
    }();

    //Checks if port number is in the required range.
    if(portNumber < 1024 || portNumber > 65535)
        printErrorAndUsage("Invalid port number!");
    
    //Some meaningless answers that the server replies to the client.
    const std::array<std::string, 6> meaninglessAnswers {
        "Ok.", "Interesting :|", 
        "Who cares?!", "Nice!", 
        "No.", "Yea."
    };

    //Creates random number generator.
    std::random_device random;
    std::default_random_engine randomEngine(random());
    std::uniform_int_distribution<i32> uniformDistribution(0, meaninglessAnswers.size() - 1);

    std::cout << "\n\033[1;36m****************Server****************\033[0m\n\n";

    try
    {
        //Creates tcp server object with IPv4 address domain. (SocketDomain::INET6 for IPv6)
        TCPServer server(SocketDomain::INET);

        std::cout << "\033[1;34m I am waitting for a connection on port: \033[0m" << portNumber << std::endl;

        //Starts the server with specified port number. 
        server.start(portNumber, 5);
        
        std::cout << "\033[1;34m A client with address \033[0m" << server.getClientAddress() << "\033[1;34m has been connected to me.\033[0m\n";
        
        for(;;)
        {
            std::cout << "\033[1;33m Listening... \033[0m\n";
            std::string receivedString;
            
            //Receives a simple text from client.
            server.receive(receivedString);

            //Exits the loop if the client says bye.
            if(receivedString == "bye")
                break;
          
            std::cout << "\033[1;34m The client(\033[0m" << server.getClientAddress() << "\033[1;34m) said : \033[0m" << receivedString << "\n";
            
            //Sends a random text to the client.
            server.send(meaninglessAnswers[uniformDistribution(randomEngine)]);
        }

        std::cout << "\033[1;34m Good bye! \033[0m\n";

    }
    catch(const Exception& exception)
    {
        std::cerr << exception.what();
    }
    
    return 0;
}