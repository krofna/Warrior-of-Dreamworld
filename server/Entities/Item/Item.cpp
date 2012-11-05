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
#include "Database.hpp"
#include "ObjectMgr.hpp"
#include "Player.hpp"
#include "shared/Log.hpp"

Item::Item() : m_Container(nullptr), m_uState(ITEM_UNCHANGED), m_cState(NONE), m_Template(nullptr)
{

}

Item* Item::CreateItem(uint64 ItemID, Player const* Owner)
{
    Item* pNewItem = new Item;
    return pNewItem->Create(ItemID, Owner) ? pNewItem : nullptr;
}

bool Item::Create(uint64 ItemID, Player const* Owner)
{
    ItemTemplate const* itemProto = sObjectMgr.GetItemTemplate(ItemID);
    if (!itemProto)
        return false;

    m_GUID      = sDatabase.Generate64BitsGUID();
    m_Text      = itemProto->Name;
    m_Container = nullptr;
    m_Slot      = 0;
    m_OwnerGUID = Owner ? Owner->GetObjectID() : 0;
    m_Template  = itemProto;

    return true;
}

void Item::SaveToDB()
{
    if (m_uState == ITEM_CREATED)
    {
        sDatabase.PExecute("INSERT INTO `items` VALUES(%llu, %llu, %llu, %llu)", m_GUID, m_OwnerGUID, m_Container ? m_Container->GetGUID() : 0, m_Slot, GetItemID());
    }
    else if (m_uState == ITEM_CHANGED)
    {
        // TODO: Make update flag and others thing.

        // sDatabase.PExecute("UPDATE %s WHERE `guid` = %llu", ModificationQuery);
    }
    else if (m_uState == ITEM_DELETED)
    {
        sDatabase.PExecute("DELETE FROM `items` WHERE `guid` = %llu", m_GUID);
    }

    m_uState = ITEM_UNCHANGED;
}

void Item::LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID)
{
    QueryResult Result(sDatabase.PQuery("SELECT * FROM `items` WHERE `guid` = %llu", GUID));

    uint64 DB_GUID          = Result->getUInt64(1);
    uint64 DB_OwnerGUID     = Result->getUInt64(2);
    uint64 DB_ContainerGUID = Result->getUInt64(3);
    uint8  DB_Slot          = Result->getUInt  (4);
    uint64 DB_ItemID        = Result->getUInt64(5);

    if (GUID != DB_GUID)
    {
        sLog.Write("Corrumpted database with %llu and %llu item guid.", GUID, DB_GUID);
        throw std::runtime_error("Corrumpted database detected, can't continue (More information Log.txt)");
    }

    if (OwnerGUID != DB_OwnerGUID)
    {
        sLog.Write("Corrumpted database with %llu and %llu item owner guid.", OwnerGUID, DB_OwnerGUID);
        throw std::runtime_error("Corrumpted database detected, can't continue (More information Log.txt)");
    }

    if (ItemID != DB_ItemID)
    {
        sLog.Write("Corrumpted database with %llu and %llu item id.", ItemID, DB_ItemID);
        throw std::runtime_error("Corrumpted database detected, can't continue (More information Log.txt)");
    }
    
    m_Template = sObjectMgr.GetItemTemplate(ItemID);
    m_Slot     = DB_Slot;
    m_Text     = GetTemplate()->Name;

    m_uState   = ITEM_UNCHANGED;
    m_cState   = NONE;
}

void Item::BuildPacketData(WorldPacket& Packet)
{
    uint64 ItemID = GetItemID();
    Packet << ItemID;
}

void Item::SetContainer(Bag* pContainer)
{
    m_Container = pContainer;
}

void Item::SetSlot(uint8 Slot)
{
    m_Slot = Slot;
}

void Item::SetText(std::string const& Text)
{
    m_Text = Text;
}

void Item::SetItemID(uint64 ItemID)
{
    m_Template = sObjectMgr.GetItemTemplate(ItemID);
}

Bag* Item::GetContainer() const
{
    return m_Container;
}

uint8 Item::GetSlot() const
{
    return m_Slot;
}

std::string Item::GetText() const
{
    return m_Text;
}

uint64 Item::GetGUID() const
{
    return m_GUID;
}

uint64 Item::GetOwnerGUID() const
{
    return m_OwnerGUID;
}

Player const* Item::GetOwner() const
{
    return sObjectMgr.GetPlayer(m_OwnerGUID);
}

ItemTemplate const* Item::GetTemplate() const
{
    return m_Template;
}

uint64 Item::GetItemID() const
{
    return m_Template->ItemID;
}

bool Item::IsBag() const
{
    return m_Template->InventoryType == INVTYPE_BAG;
}

Bag* Item::ToBag()
{
    return (IsBag() ? dynamic_cast<Bag* >(this) : nullptr);
}
