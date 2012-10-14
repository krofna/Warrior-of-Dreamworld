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
#include "WorldAcceptor.hpp"
#include "World.hpp"
#include "shared/Log.hpp"
#include "WorldSession.hpp"
#include "Database.hpp"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#define DEBUG_CONNECTION

WorldAcceptor::WorldAcceptor(boost::asio::io_service& io) :
Endpoint                    (boost::asio::ip::tcp::v4(), 0xBEEF)
{
    pAcceptor = new TCPAcceptor(io, Endpoint);
}

WorldAcceptor::~WorldAcceptor()
{
    delete pAcceptor;
}

void WorldAcceptor::HandleAccept(WorldSessionPtr Session, const boost::system::error_code& error)
{
    #if defined(DEBUG_CONNECTION)
    sLog.Write("Accepting a connection...");
    #endif

    if(!error)
    {
        Session->Start();
    }
    #if defined(DEBUG_CONNECTION)
    else
    {
        sLog.Write("Connection refused (%s)", error.message());
    }
    #endif
    Accept();
}

void WorldAcceptor::Accept()
{
    NewSession = make_shared<WorldSession>(pAcceptor->get_io_service());
    pAcceptor->async_accept(NewSession->Socket,
        boost::bind(&WorldAcceptor::HandleAccept, this, NewSession,
        boost::asio::placeholders::error));
}

