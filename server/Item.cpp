#include "Item.hpp"

Item::Item() : m_Template(nullptr)
{

}

Item* Item::CreateItem(uint64 Entry, Player* pPlayer)
{
    Item* pItem = new Item;

    pItem->m_GUID = Generate64BitsGUID();
    pItem->m_State = ITEM_CREATED;
    pItem->SetOwnerGUID(pPlayer->GetObjectID());
    pItem->m_Template = sObjectMgr.GetItemTemplate(Entry);

    return pItem;
}

void Item::SaveToDB()
{
    if (m_State == ITEM_CREATED)
    {
        sDatabase.PExecute("INSERT INTO `items` VALUES(%llu, %llu, %llu)", m_GUID, m_OwnerGUID, m_Template->Entry);
    }
    else if (m_State == ITEM_MODIFIED)
    {

    }
    else if (m_State == ITEM_DELETED)
    {
        DeleteFromDB();
    }
}

void Item::LoadToDB()
{
    
}

void Item::DeleteFromDB()
{

}

ItemTemplate* Item::GetTemplate() const
{
    return m_Template;
}

bool Item::IsBag() const
{
    return m_Template->Type == INV_TYPE_BAG;
}

uint64 Item::GetOwnerGUID() const
{
    return m_OwnerGUID;
}

void Item::SetOwnerGUID(uint64 OwnerGUID)
{
    m_OwnerGUID = OwnerGUID;
}

Player* Item::GetOwner() const
{
    return sObjectMgr.GetPlayer(m_OwnerGUID);
}
