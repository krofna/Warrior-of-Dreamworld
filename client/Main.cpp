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
#include "Login.h"
#include "Game.h"
#include "WorldSession.h"
#include "Config.h"
#include "boost/scoped_ptr.hpp"

int main()
{
    using namespace std;

    ofstream ErrorLog("Error Log.txt");
    cerr.rdbuf(ErrorLog.rdbuf());

    try
    {
        cerr << "Guessing screen resolution [FIXME]: Select configuration in game and save it in Config.conf" << endl;
        WindowWidth = (*sf::VideoMode::getFullscreenModes().begin()).width;
        WindowHeight = (*sf::VideoMode::getFullscreenModes().begin()).height;
        cerr << "My guess is: " << WindowWidth << "x" << WindowHeight << endl;

        boost::scoped_ptr<Game> pGame(new Game(true));
        Session = new WorldSession(pGame.get());
        {
            ifstream ConfigFile("Config.conf");

            if(!ConfigFile)
                throw std::exception("Cannot open Config.conf");

            std::string Ip;
            ConfigFile >> Ip;

            if(!Session->ConnectToServer(Ip.c_str()))
                throw std::exception("Cannot connect to server");

            pGame->ChangeState(new Login());
        }
        pGame->Run();
    }
    catch(std::exception& e)
    {
        cerr << e.what();
    }
    catch(...)
    {
        cerr << "Unhandled exception";
    }

    delete Session;

    return 0;
}