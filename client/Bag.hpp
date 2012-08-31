#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "../shared/Defines.hpp"

#define MAX_BAG_SIZE 36

class Item;
class Bag
{
    public:
    Bag();

    void Draw();
    void DrawEmptySlot();

    Item* GetItem(uint8 SrcSlot) const;

    void Store(uint8 DestSlot, Item* pItem);
    void Destroy(uint8 SrcSlot);
    void Create(uint8 DestSlot, uint64 Entry);

    private:
    uint8 m_Capacity;
    static_template_array<Item*, MAX_BAG_SIZE> m_Slots;
};

#endif
