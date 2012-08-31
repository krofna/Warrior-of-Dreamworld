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
}

void Item::Draw()
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
