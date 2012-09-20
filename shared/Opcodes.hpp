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
#ifndef OPCODES_H
#define OPCODES_H

#include "../shared/Config.hpp"

#ifdef MSVC
    #include "WorldSession.hpp"
#else
    class WorldSession;
#endif

#define MSG_COUNT 18

enum Opcodes
{
    MSG_NULL,
    MSG_LOGIN,
    MSG_ADD_OBJECT,
    MSG_REMOVE_OBJECT,
    MSG_MOVE_OBJECT,
    MSG_CAST_SPELL,
    MSG_LOG_OUT,
    MSG_SYSTEM_MESSAGE,
    MSG_CHAT_MESSAGE,
    MSG_COMMAND_REPONSE,
    MSG_NOTIFICATION_MSG,

    /** Items **/
    MSG_AUTO_EQUIP_ITEM,
    MSG_USE_ITEM,
    MSG_EQUIP_ITEM,
    MSG_SWAP_ITEM,
    MSG_DELETE_ITEM,
    MSG_CREATE_ITEM,
	MSG_INVENTORY_DATA
};

enum
{
    LOGIN_SUCCESS = 0x0,
    LOGIN_FAIL_BAD_USERNAME,
    LOGIN_FAIL_BAD_PASSWORD,
    LOGIN_FAIL_SERVER_OFFLINE
};

struct OpcodeHandler
{
    char const* name;
    void (WorldSession::*Handler)();
};

extern OpcodeHandler OpcodeTable[MSG_COUNT];

#define MakeOpcode(NameOpcode, Name) { #NameOpcode, &WorldSession:: ## Handle ## Name ## Opcode }
#define MakeNullOpcode(NameOpcode) { #NameOpcode, &WorldSession::HandleNULL }

#endif
