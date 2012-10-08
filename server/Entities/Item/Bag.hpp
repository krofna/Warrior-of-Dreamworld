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
#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "Item.hpp"

#define MAX_BAG_SIZE 36
#define BAG_FULL_SLOT MAX_BAG_SIZE + 1

class Bag : public Item
{
    public:
    enum BagBlockStatus
    {
        BAG_USED,
        BAG_UNUSED,
        BAG_ERROR
    };
    Bag();

    virtual bool Create(uint64 ItemID, Player const* Owner = nullptr);

    virtual void LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID);
    virtual void SaveToDB();

    Item* GetItemBySlot(uint8 Slot) const;
    bool IsValidSlot(uint8 Slot) const;
    uint8 FindFreeSlot() const;
	uint8 NumberFreeSlots() const;

    void Store(Item* pItem, uint8 Slot);
    void Destroy(uint8 Slot);

    void BuildPacketData(WorldPacket& Packet);

    private:
    uint8 m_Capacity;
    array<Item*, MAX_BAG_SIZE> m_Items;
};

#endif
