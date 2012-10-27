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
#include <boost/asio/io_service.hpp>
#include <stack>

class Game
{
public:
    static Game& GetInstance();
    static void Create(boost::asio::io_service& io);
    ~Game();

    void Update();

    void PushState(GameState* pState);
    void PopState();
    void PopAllStates();

private:
    sf::Event Event;
    std::stack<GameState*> StateStack;
    boost::asio::io_service* io;
};

#endif
