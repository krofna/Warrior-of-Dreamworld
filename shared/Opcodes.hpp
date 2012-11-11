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

#include "shared/Config.hpp"
#include "WorldSession.hpp"

// MSG = Client & Server side message
// CMSG = Client side message
// SMSG = Server side message

enum Opcodes
{
    MSG_NULL,

    /** Connection stuff **/
    MSG_LOGIN,
    MSG_LOG_OUT,

    /** Movement & Object Management stuff **/
    SMSG_ADD_OBJECT,
    SMSG_REMOVE_OBJECT,
    MSG_MOVE_OBJECT,

    /** Chat Stuff  **/
    SMSG_SYSTEM_MSG, // Debug System Message
    SMSG_NOTIFICATION_MSG, // Notification System Message
    SMSG_COMMAND_REPONSE_MSG, // Command reponse message
    MSG_CHAT_MSG, // Normal chat message

    /** Attack **/
    CMSG_MELEE_ATTACK,
    MSG_CAST_SPELL,
    SMSG_REMOVE_SPELL,

    /** Items **/
    CMSG_AUTO_EQUIP_ITEM,
    MSG_USE_ITEM,
    MSG_EQUIP_ITEM,
    MSG_SWAP_ITEM,
    MSG_DELETE_ITEM,
    SMSG_CREATE_ITEM,
    SMSG_INVENTORY_DATA,

    /** NPC Interact Stuff **/
    SMSG_NPC_INTERACT,
    CMSG_BUY_ITEM,
    CMSG_SELL_ITEM,
    CMSG_ACCEPT_QUEST,

    MSG_COUNT
};

enum LoginReponse
{
    LOGIN_SUCCESS = 0x0,
    LOGIN_FAIL_BAD_USERNAME,
    LOGIN_FAIL_BAD_PASSWORD,
    LOGIN_FAIL_SERVER_OFFLINE // Comment by OzVessalius: How can the server answer that if it is offline xD ? Or, there will be a authserver & worldserver ?
};

struct OpcodeHandler
{
    char const* name;
    void (WorldSession::*Handler)();
};

extern OpcodeHandler OpcodeTable[MSG_COUNT];

#endif
