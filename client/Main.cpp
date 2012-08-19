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
#include "boost/scoped_ptr.hpp"

#include <stdexcept>

int main()
{
    using namespace std;

    Window = new sf::RenderWindow;
    boost::asio::io_service io;

    try
    {
        sLog.Write("Guessing screen resolution [FIXME]: Select configuration in game and save it in Config.conf");
        WindowWidth = (*sf::VideoMode::getFullscreenModes().begin()).width;
        WindowHeight = (*sf::VideoMode::getFullscreenModes().begin()).height;
        sLog.Write("My guess is: %ux%u", WindowWidth, WindowHeight);

        std::string Ip;
        
        tcp::resolver Resolver(io);
        tcp::resolver::query Query(Ip.c_str(), "48879");
        tcp::resolver::iterator Iterator = Resolver.resolve(Query);

        sGame = new Game(true);
        Session = new WorldSession(io, Iterator, sGame);
        sGame->PushState(new Login());

        boost::thread NetworkThread(boost::bind(&boost::asio::io_service::run, &io));
        sGame->Run();
        NetworkThread.join();
    }
    catch(std::exception const
          & e)
    {
        sLog.Write("%s", e.what());
    }
    catch(...)
    {
        sLog.Write("Unhandled exception");
    }

    sLog.Flush();

    delete sGame;
    delete Session;
    delete Window;

    return 0;
}
