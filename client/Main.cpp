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
#include "shared/Log.hpp"
#include <stdexcept>
#include <boost/bind/bind.hpp>

int main()
{
    using namespace std;

    try
    {
        boost::asio::io_service io;

        Game::Create(io);
        WorldSession::Create(io);

        ObjectMgr::GetInstance()->Initialize("data/tileset", "data/database/templates_info.dbc");

        Game::GetInstance().PushState(new Login());

        io.post(boost::bind(&Game::Update, boost::ref(Game::GetInstance())));
        io.run();

        WorldSession::Destroy();
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

    return 0;
}
