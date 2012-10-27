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
#include "Bag.hpp"
#include "Item.hpp"
#include "ObjectMgr.hpp"
#include "GameState.hpp"

// TODO: Write the good positions :D
// Comment by krofna: What the hell is this?
#define BAG_POS_Y ((Window->getSize().y / 6) * 5)

sf::Vector2f Bag::PositionsBag[4];

void Bag::InitializePositionsBag()
{
    PositionsBag[0] = sf::Vector2f(Window->getSize().x - 200.f, (float)BAG_POS_Y + 50.f);
    PositionsBag[1] = sf::Vector2f(Window->getSize().x - 150.f, (float)BAG_POS_Y + 50.f);
    PositionsBag[2] = sf::Vector2f(Window->getSize().x - 100.f, (float)BAG_POS_Y + 50.f);
    PositionsBag[3] = sf::Vector2f(Window->getSize().x - 50.f, (float)BAG_POS_Y + 50.f);
}

Bag::Bag(int idxBag, std::string const& FileNameIcon) : m_Capacity(0), m_IsCreated(false)
{
    for (int i = 0 ; i < MAX_BAG_SIZE ; ++i)
        m_Slots[i] = nullptr;

    m_IconSprite = new sf::Sprite(*sObjectMgr->GetTexture("BagIcon", FileNameIcon));
    m_IconSprite->setPosition(PositionsBag[idxBag]);
}

Bag::~Bag()
{
    delete m_IconSprite;
}

void Bag::Create(uint64 Entry)
{
    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(Entry);
    m_Capacity = proto->ContainerSlots;
    m_IsCreated = true;
}

void Bag::Draw()
{
    sf::Vector2f baseSlotPos(BASE_SLOT_POS(m_IconSprite->getPosition().x, m_IconSprite->getPosition().y));
    for (int i = 0 ; i < m_Capacity ; ++i)
    {
        if (m_Slots[i])
            m_Slots[i]->Draw(baseSlotPos, i);
        else
            DrawEmptySlot(baseSlotPos, i);
    }
}

void Bag::DrawIcon()
{
    Window->draw(*m_IconSprite);
}

void Bag::DrawEmptySlot(sf::Vector2f const& baseSlotPos, int idx)
{
    sf::RectangleShape shape(SLOT_SHAPE);
    shape.setTexture(sObjectMgr->GetTexture("EmptySlotIcon", "data/icon/empty_slot.png"));
    // TODO: Where should we draw ?
    
    /*
    sf::Vector2f SlotPos (baseSlotPos);

    Hahahaha... Where?

    shape.setPosition(SlotPos);
    Window->Draw(shape);
       */
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

uint8 Bag::GetNumSlots() const
{
    return m_Capacity;
}

bool Bag::IsInArea(sf::Vector2i const& point) const
{
    sf::FloatRect rect = sf::FloatRect(m_IconSprite->getPosition(), m_IconSprite->getScale());

    return rect.contains(sf::Vector2f(point));
}
