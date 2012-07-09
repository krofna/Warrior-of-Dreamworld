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

#include "WorldSession.h"

#define MSG_COUNT 8

enum Opcodes
{
    MSG_NULL,
    MSG_LOGIN,
    MSG_ADD_OBJECT,
    MSG_REMOVE_OBJECT,
    MSG_MOVE_OBJECT,
    MSG_CAST_SPELL,
    MSG_SEND_TEXT,
    MSG_LOG_OUT
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

#endif