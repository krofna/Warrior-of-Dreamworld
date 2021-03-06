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
#include "SpellBox.hpp"
#include "Unit.hpp"

SpellBox::SpellBox(SpellTemplate* pSpell, Unit* pCaster, FloatRect Hitbox, float Angle, uint32 SpellBoxID) :
pSpellTemplate    (pSpell),
pCaster           (pCaster),
Hitbox            (Hitbox),
Angle             (Angle),
SpellBoxID        (SpellBoxID),
_Diff             (0)
{
}

bool SpellBox::CollidesWith(Unit* pObject)
{
    // TODO: Adjust hitbox, dont construct new objects too much
    FloatRect Obj(pObject->GetX(), pObject->GetY(), 1.0f, 1.0f);
    return Hitbox.intersects(Obj);
}

void SpellBox::Update(int64 diff)
{
    // TODO: This is completly random...
    diff += _Diff;
    int64 Multiplier = diff / 15;
    _Diff = diff - Multiplier * 15;

    Hitbox.left += (std::sin(this->Angle) / TILE_SIZE) * Multiplier;
    Hitbox.top += (std::cos(this->Angle) / TILE_SIZE) * Multiplier;
}
