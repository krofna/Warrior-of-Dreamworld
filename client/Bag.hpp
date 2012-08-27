#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "../shared/Defines.hpp"

class Item;
class Bag
{
    public:
    Bag();

    void Draw();

    void Store(uint8 DestSlot, Item* pItem);
    void Destroy(uint8 SrcSlot, uint8 Count);
    void Create(uint8 DestSlot, uint64 Entry, uint8 Count);

    private:
    array<Item*, MAX_CAPACITY> m_Slots;
};

#endif
