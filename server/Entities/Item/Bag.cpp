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
#include "Bag.hpp"

#include "Database.hpp"
#include "ObjectMgr.hpp"
#include "shared/WorldPacket.hpp"

Bag::Bag()
{
    for (uint8 iSlot = 0 ; iSlot < MAX_BAG_SIZE ; ++iSlot)
        m_Items[iSlot] = nullptr;
}

bool Bag::Create(uint64 ItemID, Player const* Owner)
{
    if (!Item::Create(ItemID, Owner))
        return false;

    ItemTemplate const* itemProto = sObjectMgr.GetItemTemplate(ItemID);
    if (!itemProto)
        return false;
    
    SetItemID(ItemID);
    m_Capacity = itemProto->ContainerSlots;

	return true;
}

void Bag::LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID)
{
    Item::LoadFromDB(GUID, OwnerGUID, ItemID);

    QueryResult Result(sDatabase.PQuery("SELECT * FROM `items` WHERE `bagid` = %llu", GUID));

    // TODO: Use only needed row.

    while (Result->next())
    {
        uint64 ItemGUID     = Result->getUInt64 (1);
        uint64 DB_OwnerGUID = Result->getUInt64 (2);
        uint64 BagGUID      = Result->getUInt64 (3);
        uint8  Slot         = Result->getUInt   (4);
        uint64 ItemID       = Result->getUInt64 (5);

        Item* pNewItem = new Item;
        pNewItem->LoadFromDB(ItemGUID, OwnerGUID, ItemID);
        m_Items[Slot] = pNewItem;
    }
}

void Bag::SaveToDB()
{
    Item::SaveToDB();
}

void Bag::BuildPacketData(WorldPacket& Packet)
{
    uint64 ItemID = GetItemID();
    // Bag item ID
    Packet << ItemID << (uint8)m_Capacity;

    for (uint8 iSlot = 0 ; iSlot < m_Capacity ; ++iSlot)
    {
        if (m_Items[iSlot])
        {
            Packet << (uint8)Item::ITEM_USED;
            m_Items[iSlot]->BuildPacketData(Packet);
        }
        else
            Packet << (uint8)Item::ITEM_UNUSED;
    }
}

Item* Bag::GetItemBySlot(uint8 Slot) const
{
    if (!IsValidSlot(Slot))
        return nullptr;

    return m_Items[Slot];
}

bool Bag::IsValidSlot(uint8 Slot) const
{
    return Slot >= 0 && Slot <= m_Capacity;
}

uint8 Bag::FindFreeSlot() const
{
    for (uint8 iSlot = 0 ; iSlot < m_Capacity ; ++iSlot)
    {
        if (!m_Items[iSlot])
            return iSlot;
    }

	return BAG_FULL_SLOT;
}

uint8 Bag::NumberFreeSlots() const
{
	uint8 Free = 0;
	for (uint8 iSlot = 0 ; iSlot < m_Capacity ; ++iSlot)
		if (!m_Items[iSlot])
			Free++;

	return Free;
}

void Bag::Store(Item* pItem, uint8 Slot)
{
    if (!IsValidSlot(Slot))
        return;

    m_Items[Slot] = pItem;
    pItem->SetContainer(this);
    pItem->SetSlot(Slot);
}

void Bag::Destroy(uint8 Slot)
{
    if (!IsValidSlot(Slot))
        return;

    if (m_Items[Slot])
    {
        delete m_Items[Slot];
        m_Items[Slot] = nullptr;
    }
}
