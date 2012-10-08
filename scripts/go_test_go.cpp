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
#include "GameObject.hpp"
#include "GameObjectAI.hpp"

class WOD_DLL_DECL go_test_goAI : public GameObjectAI
{
public:
    go_test_goAI(GameObject* pGo) : GameObjectAI(pGo)
    {
        Reset();
    }

    void Reset()
    {
    }
};

GameObjectAI* GetAI_go_test_go(GameObject* pGo)
{
    return new go_test_goAI(pGo);
}

void LoadScript_npc_krofnica()
{
    RegisterAI("go_test_go", &GetAI_go_test_go);
}
