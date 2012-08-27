#ifndef BAG_DEFINED
#define BAG_DEFINED

class Bag : public Item
{
    public:
    Bag();

    virtual void LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID);
    virtual void SaveToDB();

    Item* GetItemBySlot(uint8 Slot) const;
    bool IsValidSlot(uint8 Slot) const;
    uint8 FindFreeSlot() const;

    void Store(Item* pItem, uint8 Slot);
    void Destroy(uint8 Slot);

    private:
    
};

#endif
