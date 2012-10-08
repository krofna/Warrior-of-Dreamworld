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

class Bag;
class WorldPacket;
class Player;
struct ItemTemplate;

class Item
{
    enum ItemUpdateState
    {
        ITEM_CREATED,
        ITEM_CHANGED,
        ITEM_DELETED,
        ITEM_UNCHANGED
    };
    enum ItemChangedState
    {
        SLOT_MOVED,
        BAG_MOVED,
        NONE
    };

public:
    enum ItemBlockStatus
    {
        ITEM_USED,
        ITEM_UNUSED,
        ITEM_ERROR
    };

    Item();

    static Item* CreateItem(uint64 ItemID, Player const* Owner = nullptr);

    virtual bool Create(uint64 ItemID, Player const* Owner = nullptr);

    virtual void SaveToDB();
    virtual void LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID);

    virtual void BuildPacketData(WorldPacket& Packet);

    void SetContainer(Bag* pContainer);
    Bag* GetContainer() const;

    void SetSlot(uint8 Slot);
    uint8 GetSlot() const;

    std::string GetText() const;
    void SetText(std::string const& Text);

    uint64 GetGUID() const;
    uint64 GetOwnerGUID() const;
    Player const* GetOwner() const;

    ItemTemplate const* GetTemplate() const;
    
    void SetItemID(uint64 ItemID);
    uint64 GetItemID() const;

	bool IsBag() const;
	Bag* ToBag();

private:
    uint64 m_GUID;
    std::string m_Text;
    Bag* m_Container;
    uint8 m_Slot;
    uint64 m_OwnerGUID;

    ItemUpdateState m_uState;
    ItemChangedState m_cState;

    ItemTemplate const* m_Template;
};

#endif
