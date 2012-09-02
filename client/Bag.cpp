#include "Bag.hpp"
#include "Item.hpp"

// TODO: Write the good positions :D
// And please, not voodoo again :)
const sf::Vector2f Bag::PositionsBag[4] =
{
    sf::Vector2f(WindowWidth - 50, WindowHeight - 10),
    sf::Vector2f(WindowWidth - 40, WindowHeight - 10),
    sf::Vector2f(WindowWidth - 30, WindowHeight - 10),
    sf::Vector2f(WindowWidth - 20, WindowHeight - 10)
};

Bag::Bag(int idxBag, std::string const& FileNameIcon) : m_Capacity(0), m_IsCreated(false)
{
    for (int i = 0 ; i < MAX_BAG_SIZE ; ++i)
        m_Slots[i] = nullptr;

    m_IconSprite = new sf::Sprite(*sObjectMgr->GetTexture("BagIcon", FileNameIcon));
    m_IconSprite->setPosition(PositionsBag[idxBag]);
}

void Bag::Create(uint64 Entry)
{
    m_IsCreated = true;
}

void Bag::Draw()
{
    for (int i = 0 ; i < m_Capacity ; ++i)
    {
        if (m_Slots[i])
            m_Slots[i]->Draw();
        else
            DrawEmptySlot(i);
    }
}

void Bag::DrawIcon()
{
    Window->draw(*m_IconSprite);
}

void Bag::DrawEmptySlot(int idx)
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

Item* Bag::GetItem(uint8 SrcSlot) const
{
    return m_Slots.at(SrcSlot);
}

bool Bag::IsCreated() const
{
    return m_IsCreated;
}
