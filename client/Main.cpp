/*#include <SFML/Network.hpp>
#include <iostream>

int main()
{
    // ----- The client -----

    // Create a socket and connect it to 192.168.1.50 on port 55001
    sf::TcpSocket socket;
    socket.connect("127.0.0.1", 55001);

    // Send a message to the connected host
    std::string message = "Hi, I am a client";
    socket.send(message.c_str(), message.size() + 1);

    // Receive an answer from the server
    char buffer[1024];
    std::size_t received = 0;
    socket.receive(buffer, sizeof(buffer), received);
    std::cout << "The server said: " << buffer << std::endl;
}*/

#include "Game.h"
#include <fstream>
#include <iostream>

int main()
{
    std::ofstream ErrorLog("Error Log.txt");
    std::cerr.rdbuf(ErrorLog.rdbuf());

    Game* instance = new Game;
    World* world = new World;
    world->LoadTileMap(1);
    instance->CurrentState = world;
    instance->Run();
}