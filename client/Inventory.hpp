#ifndef INVENTORY_DEFINED
#define INVENTORY_DEFINED

#include "../shared/Defines.hpp"

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
