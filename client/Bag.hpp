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
#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "shared/Defines.hpp"
#include "shared/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <boost/utility.hpp>

#define MAX_BAG_SIZE 36

#define BASE_SLOT_POS(x, y) sf::Vector2f(x, y) // ??
#define SLOT_SHAPE sf::Vector2f(15, 15)

class Item;

class Bag : private boost::nocopyable
{
public:
    Bag(int idxBag, std::string const& FileNameIcon);
    ~Bag();

    void Create(uint64 Entry);

    void Draw();
    void DrawIcon();
    void DrawEmptySlot(sf::Vector2f const& baseSlotPos, int idx);

    Item* GetItem(uint8 SrcSlot) const;

    void Store(uint8 DestSlot, Item* pItem);
    void Destroy(uint8 SrcSlot);
    void Create(uint8 DestSlot, uint64 Entry);

    bool IsCreated() const;

    bool IsInArea(sf::Vector2i const& point) const;
    
    uint8 GetNumSlots() const; 

    static void InitializePositionsBag();

private:
    bool m_IsCreated;
    uint8 m_Capacity;
    static_template_array<Item*, MAX_BAG_SIZE> m_Slots;

    // GUI Stuff
    sf::Sprite* m_IconSprite;

    static sf::Vector2f PositionsBag[4];
};

#endif
