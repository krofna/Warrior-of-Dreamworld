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
#include "boost/scoped_ptr.hpp"

#include <stdexcept>

#ifdef GPROF

struct wrapper_profiling_multithread
{
    itimerval* itimer;
};

void wrapper_profiler_multithread(boost::asio::io_service* io, wrapper_profiling_multithread wrapper)
{
    setitimer(ITIMER_PROF, wrapper.itimer, NULL);
    io->run();
}

#endif

int main()
{
    using namespace std;

    sObjectMgr = new ObjectMgr("data/tileset", "data/database/templates_info.dbc");
    sObjectMgr->Initialize();

    Window = new sf::RenderWindow();
    sSFGUI = new sfg::SFGUI();
    boost::asio::io_service io;

    try
    {
        sLog.Write("Guessing screen resolution [FIXME]: Select configuration in game and save it in Config.conf");
        WindowWidth = (*sf::VideoMode::getFullscreenModes().begin()).width;
        WindowHeight = (*sf::VideoMode::getFullscreenModes().begin()).height;
        sLog.Write("My guess is: %ux%u", WindowWidth, WindowHeight);

        sGame = new Game(true);
        Session = new WorldSession(io, sGame);
        sGame->PushState(new Login());

#ifdef GPROF

        wrapper_profiling_multithread profiling_wrapper;
        getitimer(ITIMER_PROF, profiling_wrapper.itimer);
        boost::thread NetworkThread(boost::bind(&wrapper_profiler_multithread, &io, profiling_wrapper));

#endif
        boost::thread NetworkThread(boost::bind(&boost::asio::io_service::run, &io));
        sGame->Run();
        io.stop();
        NetworkThread.join();
    }
    catch(std::exception const
          & e)
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
    delete sGame;
    delete Session;
    delete sSFGUI;
    delete Window;

    return 0;
}
