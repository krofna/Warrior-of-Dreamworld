#include "Bag.hpp"
#include "Item.hpp"

Bag::Bag()
{

}

void Bag::Draw()
{
    for (int i = 0 ; i < m_Capacity ; ++i)
    {
        if (m_Slots[i])
            m_Slots[i]->Draw();
        else
            DrawEmptySlot();
    }
}

void Bag::DrawEmptySlot()
{
    // TODO: What should we draw ?
}

void Bag::Store(uint8 DestSlot, Item* pItem)
{
    m_Slots[DestSlot] = pItem;
}

void Bag::Destroy(uint8 SrcSlot)
{
    delete m_Slots[SrcSlot];
    m_Slots[SrcSlot] = nullptr;
}

void Bag::Create(uint8 DestSlot, uint64 Entry)
{
    m_Slots[DestSlot] = Item::Create(Entry);
}
