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
#ifndef SPELL_BOX_H
#define SPELL_BOX_H

#include "../shared/Defines.hpp"

struct Spell;
class Unit;

class SpellBox
{
public:
    SpellBox(Spell* pSpell, Unit* pCaster, sf::FloatRect Hitbox, float Angle, Uint32 SpellBoxID);

    bool CollidesWith(Unit* pObject);
    void Update(Int32 diff);

    Spell* pSpell;
    Unit* pCaster;
    Uint32 SpellBoxID;

private:
    sf::FloatRect Hitbox;
    float Angle;
    Int32 _Diff;
};

#endif