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
#include "Game.hpp"
#include "WorldSession.hpp"
#include "ObjectMgr.hpp"
#include <stdexcept>

int main()
{
    using namespace std;

    boost::asio::io_service io;
    Game::Create(io);
    WorldSession::Create(io);

    sObjectMgr = new ObjectMgr("data/tileset", "data/database/templates_info.dbc");
    sObjectMgr->Initialize();

    sSFGUI = new sfg::SFGUI();

    try
    {
        sLog.Write("Guessing screen resolution [FIXME]: Select configuration in game and save it in Config.conf");
        WindowWidth = (*sf::VideoMode::getFullscreenModes().begin()).width;
        WindowHeight = (*sf::VideoMode::getFullscreenModes().begin()).height;
        sLog.Write("My guess is: %ux%u", WindowWidth, WindowHeight);

        Game::GetInstance().PushState(new Login());

        io.post(boost::bind(&Game::Update, Game::GetInstance()));
        io.run();
    }
    catch(std::exception const& e)
    {
        sLog.Write("EXCEPTION occured: %s", e.what());
    }
    catch(...)
    {
        sLog.Write("Unhandled exception");
    }

    sLog.Flush();

    sObjectMgr->Cleanup();

    delete sObjectMgr;
    delete sSFGUI;

    return 0;
}
