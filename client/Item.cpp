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
#include "Item.hpp"
#include "Globals.hpp"
#include "Templates.hpp"

Item::Item()
{

}

Item* Item::Create(uint64 Entry)
{
    Item* newItem = new Item;
    ItemTemplate const* itemProto = sObjectMgr->GetItemData(Entry);

    if (!itemProto)
        return nullptr;

    newItem->m_Template = itemProto;
    return newItem;
}

void Item::Draw(sf::Vector2f const& baseSlotPos, int idx)
{
    // TODO: Draw icon display ID.
}

void Item::DrawInformation()
{
    // TODO: Draw name and description, and extra informations.
}

void Item::Use()
{
    if (m_Template->InventoryType == INVTYPE_BAG)
    {
        // TODO: Session->SendEquipRequest(m_GUID);
    }
    else if (m_Template->InventoryType == INVTYPE_NON_EQUIP)
    {
        // TODO: Session->SendUseRequest(m_GUID);
    }
}
