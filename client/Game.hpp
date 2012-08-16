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
#include <queue>
#include <stack>

class Game
{
public:
    Game(bool FullScreen);
    ~Game();
    void Run();

    void AddToLoadQueue(Loadable* pLoadable, WorldPacket Argv);
    void PushState(GameState* pState) { StateStack.push(pState); }
    void PopState() { delete StateStack.top(); StateStack.pop(); }
    void PopAllStates() { while(!StateStack.empty()) { delete StateStack.top(); StateStack.pop(); }}


private:
    std::stack<GameState*> StateStack;

    std::queue<std::pair<Loadable*, WorldPacket> > LoadQueue;
    boost::mutex LoadQueueMutex;
};

#endif