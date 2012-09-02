#include "Inventory.hpp"
#include "Item.hpp"
#include "Bag.hpp"

#include <SFML/Window/Event.hpp>

Inventory::Inventory(std::string const& FileNameIcon)
{
	for (int i = 0 ; i < 4 ; ++i)
        m_Bags[i] = new Bag(i, FileNameIcon);
}

bool Inventory::HandleEvent(sf::Event& e)
{
    bool Modification = false;

    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::B && e.key.shift)
        {
            m_IsAllOpen = !m_IsAllOpen;
            Modification = true;
        }
    }

    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);

        for (int iBag = 0 ; iBag < 4 ; ++iBag)
        {
            // TODO: Check if function is correct
            if (m_Bags[iBag]->IsInArea(mousePos))
            {
                m_IsBagOpen[iBag] = true;
                Modification = true;
                break;
            }
        }
    }

    return Modification;
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
    for (int iBag = 0 ; iBag < 4 ; ++iBag)
    {
        m_Bags[iBag]->DrawIcon();
    }
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

void Inventory::Create(uint8 DestBag, uint64 Entry)
{
    m_Bags[DestBag]->Create(Entry);
}

bool Inventory::AutoCreateBag(uint64 Entry)
{
    for (int i = 0 ; i < 4 ; ++i)
    {
        if (!m_Bags[i]->IsCreated())
        {
            m_Bags[i]->Create(Entry);
            return true;
        }
    }
    return false;
}
