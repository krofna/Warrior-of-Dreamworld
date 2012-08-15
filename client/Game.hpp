/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012  Mislav Blazevic

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef GAME_H
#define GAME_H

#include "GameState.hpp"
#include "Globals.hpp"
#include "boost/thread.hpp"
#include "Loadable.h"
#include <queue>

class Game
{
public:
    Game(bool FullScreen);
    ~Game();
    void Run();

    void AddToLoadQueue(Loadable* pLoadable, char* Argv);
    void ChangeState(GameState* pState) { this->CurrentState = pState; }

private:
    GameState* CurrentState;

    std::queue<std::pair<Loadable*, char*> > LoadQueue;
    boost::mutex LoadQueueMutex;
};

#endif