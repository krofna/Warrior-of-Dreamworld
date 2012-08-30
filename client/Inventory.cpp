#include "Inventory.hpp"
#include "Item.hpp"
#include "Bag.hpp"

#include <SFML/Window/Event.hpp>

Inventory::Inventory()
{

}

void Inventory::HandleEvent(sf::Event& e)
{
    // TODO: Check if click on Inventory menu or check if Inventory menu is already open and clicked on item
    // and do something with...
}

void Inventory::Draw()
{
    DrawBagOpen();
    DrawBagIcon();
}

void Inventory::DrawBagOpen()
{
    if (m_IsAllOpen)
    {
        for (int iBag = 0 ; iBag < 4 ; ++iBag)
        {
            m_Bags[iBag]->Draw();
        }
    }
    else
    {
        for (int iBag = 0 ; iBag < 4 ; ++iBag)
        {
            if (m_IsBagOpen[iBag])
                m_Bags[iBag]->Draw();
        }
    }
}

void Inventory::DrawBagIcon()
{
    // TODO: Draw a GUI for Inventory
}

void Inventory::Swap(uint8 SrcBag, uint8 DestBag, uint8 SrcSlot, uint8 DestSlot)
{
    Item* pSrcItem = m_Bags[SrcBag]->GetItem(SrcSlot);
    m_Bags[DestBag]->Store(DestSlot, pSrcItem);
}

void Inventory::Destroy(uint8 SrcBag, uint8 SrcSlot)
{
    m_Bags[SrcBag]->Destroy(SrcSlot);
}

void Inventory::Create(uint8 DestBag, uint8 DestSlot, uint64 Entry)
{
    m_Bags[DestBag]->Create(DestSlot, Entry);
}
