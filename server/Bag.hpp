#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "Item.hpp"

#define MAX_BAG_SIZE 36

class Bag : public Item
{
    public:
    Bag();

    virtual bool Create(uint64 ItemID, Player const* Owner = nullptr);

    virtual void LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID);
    virtual void SaveToDB();

    Item* GetItemBySlot(uint8 Slot) const;
    bool IsValidSlot(uint8 Slot) const;
    uint8 FindFreeSlot() const;

    void Store(Item* pItem, uint8 Slot);
    void Destroy(uint8 Slot);

    private:
    uint8 m_Capacity;
    array<Item*, MAX_BAG_SIZE> m_Items;
};

#endif
