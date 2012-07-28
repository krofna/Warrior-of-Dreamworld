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
#include "Login.hpp"
#include "Globals.hpp"
#include "../shared/Opcodes.hpp"
#include <cassert>

Login::Login() :
InputFlag(true)
{
}

void Login::HandleEvent(sf::Event Event)
{
    switch(Event.type)
    {
    case sf::Event::Closed:
        Window.close();
        break;

    case sf::Event::KeyPressed:
        if(Event.key.code == sf::Keyboard::Return)
        {
            if(InputFlag)
            {
                InputFlag = false;
            }
            else
            {
                Session->SendAuthRequest(Username, Password);
            }
        }
        else if(Event.key.code == sf::Keyboard::BackSpace)
        {
            if(InputFlag)
            {
                Username.erase(Username.end()-1);
                UsernameText.setString(Username);
            }
            else
                Password.erase(Password.end()-1);
        }
        else if(Event.key.code == sf::Keyboard::Escape)
            Window.close();
        break;

    case sf::Event::TextEntered:
        if(InputFlag)
        {
            if(Event.text.unicode > 64)
            {
                Username += tolower(static_cast<char>(Event.key.code));
                UsernameText.setString(Username);
            }
        }
        else
        {
            if(Event.text.unicode > 64)
                Password += static_cast<char>(Event.key.code);
        }
    default:
		break;
    }
}

void Login::Draw()
{
    Window.draw(UsernameText);
}
