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
    Packet << ItemID;
    // Bag slots used
    Packet << m_Capacity - NumberFreeSlots();

    for (uint8 iSlot = 0 ; iSlot < m_Capacity ; ++iSlot)
    {
        if (m_Items[iSlot])
            m_Items[iSlot]->BuildPacketData(Packet);
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
