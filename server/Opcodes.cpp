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
#include "../shared/Opcodes.hpp"

OpcodeHandler OpcodeTable[MSG_COUNT] = 
{
    { "MSG_NULL", &WorldSession::HandleNULL },
    { "MSG_LOGIN", &WorldSession::HandleLoginOpcode },
    { "MSG_ADD_OBJECT", &WorldSession::HandleNULL }, // SMSG
    { "MSG_REMOVE_OBJECT", &WorldSession::HandleNULL }, // SMSG
    { "MSG_MOVE_OBJECT", &WorldSession::HandleMoveObjectOpcode },
    { "MSG_CAST_SPELL", &WorldSession::HandleCastSpellOpcode },
    { "MSG_LOG_OUT", &WorldSession::HandleLogOutOpcode },
    { "MSG_SPELL_HIT", &WorldSession::HandleNULL } // SMSG
};
