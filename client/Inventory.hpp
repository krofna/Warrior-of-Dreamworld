#ifndef INVENTORY_DEFINED
#define INVENTORY_DEFINED

#include "../shared/Defines.hpp"

namespace sf { struct Event; }
class Bag;
class Inventory
{
    public:
    Inventory();

    void HandleEvent(sf::Event& e);
    void Draw();

    void Swap(uint8 SrcBag, uint8 DestBag, uint8 SrcSlot, uint8 DestSlot);
    void Destroy(uint8 SrcBag, uint8 SrcSlot);
    void Create(uint8 DestBag, uint8 DestSlot, uint64 Entry);

    protected:
    void DrawBagOpen();
    void DrawBagIcon();

    private:
    Bag* m_Bags[4]; 
    bool m_IsBagOpen[4];
    bool m_IsAllOpen;
};

#endif
