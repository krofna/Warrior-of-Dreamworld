#ifndef ITEM_DEFINED
#define ITEM_DEFINED

#include "Templates.hpp"

class Player;
class Item
{
    enum ItemState
    {
        ITEM_CREATED,
        ITEM_MODIFIED,
        ITEM_DELETED
    };
    public:
    static Item* CreateItem(uint64 Entry, Player* pPlayer);

    Item();

    void SaveToDB();
    void LoadFromDB(uint64 GUID, uint64 OwnerGUID, uint64 Entry);
    void DeleteFromDB();

    ItemTemplate* GetTemplate() const;

    bool IsBag() const;

    uint64 GetOwnerGUID() const;
    void SetOwnerGUID(uint64 OwnerGUID);
    Player* GetOwner() const;

    private:
    uint64 m_GUID;
    uint64 m_GUIDOwner;
    ItemState m_State;
    ItemTemplate* m_Template;
};

#endif
