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
#ifndef INVENTORY_DEFINED
#define INVENTORY_DEFINED

#include "shared/Defines.hpp"

namespace sf { class Event; }
class Bag;

enum BagBlockStatus
{
    BAG_USED,
    BAG_UNUSED,
    BAG_ERROR
};
enum ItemBlockStatus
{
    ITEM_USED,
    ITEM_UNUSED,
    ITEM_ERROR
};

class Inventory
{
public:
    Inventory(std::string const& FileNameIcon);
    ~Inventory();

    bool HandleEvent(sf::Event& e);
    void Draw();

    void Swap(uint8 SrcBag, uint8 DestBag, uint8 SrcSlot, uint8 DestSlot);
    void Destroy(uint8 SrcBag, uint8 SrcSlot);
    void Create(uint8 DestBag, uint8 DestSlot, uint64 Entry);
    void Create(uint8 DestBag, uint64 Entry);

    bool AutoCreateBag(uint64 Entry);

    uint8 GetNumSlots(uint8 BagPos) const;

protected:
    void DrawBagOpen();
    void DrawBagIcon();

private:
    Bag* m_Bags[4]; 
    bool m_IsBagOpen[4];
    bool m_IsAllOpen;
};

#endif
