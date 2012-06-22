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

#ifdef USE_BOOST
#include <boost/filesystem.hpp>
#endif

int main()
{
    std::ofstream ErrorLog("Error Log.txt"), DebugLog("Debug Log.txt");
    std::cerr.rdbuf(ErrorLog.rdbuf()); std::clog.rdbuf(DebugLog.rdbuf());

    Game* pGame;
#ifdef USE_BOOST
    if(boost::filesystem::exists("Config.txt"))
    {
        std::ifstream CfgFile("Config.txt");
        bool FullScreen;

        CfgFile >> WindowWidth >> WindowHeight >> FullScreen;
        pGame = new Game(FullScreen);
    }
    else
#endif
    {
        std::cerr << "Cannot find Config.txt. Taking a wild guess... [FIXME]: Select configuration in game" << std::endl;
        //pGame->ChangeState(new Config());
        WindowWidth = (*sf::VideoMode::getFullscreenModes().begin()).width;
        WindowHeight = (*sf::VideoMode::getFullscreenModes().begin()).height;
        std::cerr << "My guess is: " << WindowWidth << "x" << WindowHeight << std::endl;
        pGame = new Game(true);
    }

    Session = new WorldSession(pGame);
    pGame->ChangeState(new Login());
    pGame->Run();
}