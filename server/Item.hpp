#ifndef ITEM_DEFINED
#define ITEM_DEFINED

class Item
{
    enum ItemUpdateState
    {
        ITEM_CREATED,
        ITEM_CHANGED,
        ITEM_DELETED,
        ITEM_UNCHANGED
    };
    enum ItemChangedState
    {
        SLOT_MOVED,
        BAG_MOVED,
        NONE
    };

    public:
    Item();

    static Item* CreateItem(uint64 ItemID, Player const* Owner = nullptr);

    virtual bool Create(uint64 ItemID, Player const* Owner = nullptr);

    virtual void SaveToDB();
    virtual void LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 ItemID);

    void SetContainer(Bag* pContainer);
    Bag* GetContainer() const;

    void SetSlot(uint8 Slot);
    uint8 GetSlot() const;

    std::string GetText() const;
    void SetText(std::string const& Text);

    uint64 GetGUID() const;
    uint64 GetOwnerGUID() const;
    Player const* GetOwner() const;

    ItemTemplate const* GetTemplate() const;
    
    void SetItemID(uint64 ItemID);
    uint64 GetItemID() const;

    private:
    uint64 m_GUID;
    std::string m_Text;
    Bag* m_Container;
    uint8 m_Slot;
    uint64 m_OwnerGUID;

    ItemUpdateState m_uState;
    ItemChangedState m_cState;

    ItemTemplate* m_Template;
};

#endif
