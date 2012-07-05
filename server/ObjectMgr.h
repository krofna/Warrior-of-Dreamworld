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
#ifndef OBJECT_MGR_H
#define OBJECT_MGR_H

#include "../client/Defines.h"

struct Spell;
class Player;

class ObjectMgr
{
public:
    Spell* GetSpell(Uint16 ID);
    Player* GetPlayer(std::string Name);

    void LoadSpells();
    void LoadPlayers();

private:
    std::vector<Spell*> Spells;
    std::map<std::string, Player*> Players;
};

extern ObjectMgr* sObjectMgr;

#endif