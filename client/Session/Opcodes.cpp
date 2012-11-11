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
#include "shared/Opcodes.hpp"

OpcodeHandler OpcodeTable[MSG_COUNT] = 
{
    { "MSG_NULL",              &WorldSession::HandleNULL },

    { "MSG_LOGIN",             &WorldSession::HandleLoginOpcode },
    { "MSG_LOG_OUT",           &WorldSession::HandleLogOutOpcode },

    { "SMSG_ADD_OBJECT",       &WorldSession::HandleAddObjectOpcode },
    { "SMSG_REMOVE_OBJECT",    &WorldSession::HandleRemoveObjectOpcode },
    { "MSG_MOVE_OBJECT",       &WorldSession::HandleMoveObjectOpcode },

    { "SMSG_SYSTEM_MSG",   &WorldSession::HandleSystemMessageOpcode },
    { "SMSG_NOTIFICATION_MSG",  &WorldSession::HandleNotificationMessageOpcode },
    { "SMSG_COMMAND_REPONSE_MSG",   &WorldSession::HandleCommandReponseOpcode },
    { "MSG_CHAT_MSG",      &WorldSession::HandleChatMessageOpcode },

    { "CMSG_MELEE_ATTACK",     &WorldSession::HandleNULL },
    { "MSG_CAST_SPELL",        &WorldSession::HandleCastSpellOpcode }, 
    { "SMSG_REMOVE_SPELL",     &WorldSession::HandleRemoveSpellOpcode },

    { "CMSG_AUTO_EQUIP_ITEM",  &WorldSession::HandleNULL },
    { "MSG_USE_ITEM",          &WorldSession::HandleUseItemOpcode },
    { "MSG_EQUIP_ITEM",        &WorldSession::HandleEquipItemOpcode },
    { "MSG_SWAP_ITEM",         &WorldSession::HandleSwapItemOpcode },
    { "MSG_DELETE_ITEM",       &WorldSession::HandleDeleteItemOpcode },
    { "SMSG_CREATE_ITEM",      &WorldSession::HandleCreateItemOpcode },
    { "SMSG_INVENTORY_DATA",   &WorldSession::HandleInventoryDataOpcode },

    { "SMSG_NPC_INTERACT",     &WorldSession::HandleNpcInteractOpcode },
    { "CMSG_BUY_ITEM",         &WorldSession::HandleNULL },
    { "CMSG_SELL_ITEM",        &WorldSession::HandleNULL },
    { "CMSG_ACCEPT_QUEST",     &WorldSession::HandleNULL }
};
