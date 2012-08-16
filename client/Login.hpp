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
#ifndef LOGIN_H
#define LOGIN_H

#include "GameState.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>

class Login : public GameState
{
public:
    Login();
    void HandleEvent(sf::Event Event);
    void Draw();
    virtual void Load(WorldPacket Argv) { sGame->PopAllStates(); sGame->PushState(this); }

private:
    sf::Text UsernameText;

    std::string Username;
    std::string Password;
    bool InputFlag;
};

#endif
