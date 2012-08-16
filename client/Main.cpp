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

int main()
{
    using namespace std;

    ofstream ErrorLog("Error Log.txt");
    cerr.rdbuf(ErrorLog.rdbuf());

    Window = new sf::RenderWindow;

    try
    {
        cerr << "Guessing screen resolution [FIXME]: Select configuration in game and save it in Config.conf" << endl;
        WindowWidth = (*sf::VideoMode::getFullscreenModes().begin()).width;
        WindowHeight = (*sf::VideoMode::getFullscreenModes().begin()).height;
        cerr << "My guess is: " << WindowWidth << "x" << WindowHeight << endl;

        boost::asio::io_service io;
        tcp::resolver Resolver(io);
        tcp::resolver::query Query("127.0.0.1", "48879");
        tcp::resolver::iterator Iterator = Resolver.resolve(Query);

        sGame = new Game(true);

        Session = new WorldSession(io, Iterator, sGame);
        {
            // TODO: this code is useless. see above
            ifstream ConfigFile("Config.conf");

            if(!ConfigFile)
                throw std::exception("Cannot open Config.conf");

            std::string Ip;
            ConfigFile >> Ip;

            sGame->AddToLoadQueue(new Login(), WorldPacket(0));
        }
        boost::thread NetworkThread(boost::bind(&boost::asio::io_service::run, &io));
        sGame->Run();
    }
    catch(std::exception& e)
    {
        cerr << e.what();
    }
    catch(...)
    {
        cerr << "Unhandled exception";
    }

    delete sGame;
    delete Session;
    delete Window;

    return 0;
}