#ifndef GAME_H
#define GAME_H

#include "World.h"

class Game
{
public:
    Game();
    void Run();
    void Foo();

//private:
    std::vector<GameState*> States;
    GameState* CurrentState;
};

#endif