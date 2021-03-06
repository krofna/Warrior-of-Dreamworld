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
#include "ScriptLoader.hpp"

extern void LoadScript_npc_krofnica();
extern void LoadScript_npc_the_fullmetal_alchimist();

extern void LoadScript_go_test_go();

extern void LoadScript_map_test_map();

void LoadScripts()
{
    // Load CreatureAI
    LoadScript_npc_krofnica();
    LoadScript_npc_the_fullmetal_alchimist();

    // Load GameObjectAI
    LoadScript_go_test_go();
    
    // Load MapScript
    LoadScript_map_test_map();
}
