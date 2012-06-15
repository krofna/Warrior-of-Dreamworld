#include "Game.h"
#include "Globals.h"

Game::Game()
{
    Window.create(sf::VideoMode(5*TILE_SIZE, 2*TILE_SIZE, 32), "[PH]", sf::Style::Close);
}

void Game::Run()
{
    sf::Event Event;

    while(Window.isOpen())
    {
        while(Window.pollEvent(Event))
        {
            if(Event.type == sf::Event::KeyPressed)
            {
                CurrentState->HandleEvent(Event);
            }
        }
        Window.clear();
        CurrentState->Draw();
        Window.display();
    }
}