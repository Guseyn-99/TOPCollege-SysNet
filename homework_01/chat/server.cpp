#include "chat.hpp"
#include "line_protocol.hpp"
#include "tcp_server.hpp"

#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <system_error>
#include <thread>
#include <utility>

namespace
{
std::mutex outputMutex;

void HandleClient(TcpClient client)
{
    try
    {
        std::string pending;
        const auto name = ReadLine(client, pending);
        if (!name || name->empty())
            return;

        while (const auto message = ReadLine(client, pending))
        {
            const std::lock_guard lock(outputMutex);
            std::cout << *name << ": " << *message << std::endl;
        }
    }
    catch (const std::exception& error)
    {
        const std::lock_guard lock(outputMutex);
        std::cerr << "Chat connection error: " << error.what() << std::endl;
    }
}
}

int main()
{
    try
    {
        TcpServer server;
        server.Listen("127.0.0.1", ChatPort);
        std::cout << "Chat server listening on 127.0.0.1:" << ChatPort << std::endl;

        while (true)
        {
            auto client = server.AcceptClient();
            try
            {
                std::thread(HandleClient, std::move(client)).detach();
            }
            catch (const std::system_error& error)
            {
                const std::lock_guard lock(outputMutex);
                std::cerr << "Could not start chat thread: " << error.what() << std::endl;
            }
        }
    }
    catch (const std::exception& error)
    {
        const std::lock_guard lock(outputMutex);
        std::cerr << "Chat server error: " << error.what() << std::endl;
        return 1;
    }
}
