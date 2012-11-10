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

#include <stack>

#include <boost/asio/io_service.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFGUI/SFGUI.hpp>

#include "GameState.hpp"
#include "shared/Singleton.hpp"

class Game : public Singleton<Game>
{
    friend class Singleton<Game>;
public:
    virtual ~Game();

    static void Initialize(boost::asio::io_service* io);

    boost::asio::io_service* GetIO();
    sf::RenderWindow* GetRenderWindow();
    sfg::SFGUI* GetSFGUI();
    sfg::Desktop* GetDesktop();

    void Update();

    void PushState(GameState* pState);
    void PopState();
    void PopAllStates();

private:
    sf::Event Event;
    std::stack<std::unique_ptr<GameState>> StateStack;

    std::unique_ptr<sf::RenderWindow> Window;
    std::unique_ptr<sfg::SFGUI> SFGUI;
    std::unique_ptr<sfg::Desktop> Desktop;

    static boost::asio::io_service* sIO;
};

#endif
