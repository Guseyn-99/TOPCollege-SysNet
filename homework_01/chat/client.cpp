#include "chat.hpp"
#include "tcp_client.hpp"

#include <exception>
#include <iostream>
#include <string>

int main()
{
    try
    {
        std::string name;
        do
        {
            std::cout << "Enter your nickname:" << std::endl;
            if (!std::getline(std::cin, name))
                return 0;
        } while (name.empty());

        TcpClient client;
        client.Connect("127.0.0.1", ChatPort);
        client.Send(name + '\n');
        std::cout << "Connected. Type a message or .exit to quit." << std::endl;

        std::string message;
        while (std::getline(std::cin, message) && message != ".exit")
        {
            client.Send(message + '\n');
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Chat client error: " << error.what() << std::endl;
        return 1;
    }
}
