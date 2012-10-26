/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012 Ryan Lahfa

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
#ifndef ITEM_DEFINED
#define ITEM_DEFINED

#include "shared/Defines.hpp"
#include "shared/Vector2.hpp"

struct ItemTemplate;

class Item
{
public:
    Item();
    static Item* Create(uint64 Entry);

    void Draw(Vector2f const& baseSlotPos, int idx);
    void DrawInformation();

    void Use();

private:
    uint64 m_GUID;
    ItemTemplate const* m_Template;
};

#endif
