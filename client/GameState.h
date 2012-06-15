#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SFML/Window.hpp>

struct GameState
{
    virtual void HandleEvent(sf::Event Event) = 0;
    virtual void Draw() = 0;
};

#endif