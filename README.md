# **SocketPlus**
|        |          |
|---------|:---------:|
|**Version:**  | 0.5.0 | 
|**Platform:** | Linux |
|**Language:** | C++ 17 |
|**License:**  | MIT |
|**Status:**   | under development |

**SocketPlus** provides a clean OOP framework for socket programming in C++.

For one of our courses at university, we had to implement a communication protocol using Unix Socket. Later, based on our experiences, we decided to create the SocketPlus that makes OOP socket programming in C++ easier.

We are working on this project in our spare time and add more features to it gradually. Currently, TCP Client/Server feature(for both IPv4 and IPv6) has been implemented.



## Required Tools and OS:
 * C++ 17 compiler.
 * CMake 3.0 or above.
 * Linux operating system.
 
 We have used *GCC-7.1.0* compiler on *Ubuntu 16.04* to develop and test the SocketPlus.

## How to build:
 In order to build SocketPlus, run following commands:
```
mkdir build
cd build
cmake ..
make
```
## How to run examples:
The executable file will be built in the *examples/bin* folder.
At the moment just one TCP example is available.

### TCP:
This example shows how the SocketPlus can be used to create a simple client/server application.
 
First you need to run the server application and specify a port number (between 1024 - 65535).

```
        ./tcpserver 55555
```

Now run the client on another terminal and tell it what address/IP and port number it should use to connect to the server. If the client and server are on the same machine use localhost or 127.0.0.1 as host address.

```
        ./tcpclient localhost 55555
```
If the connection is established successfully, the client can get the user input and send it to the server. when the server gets the message, it will replies to the server by random words.