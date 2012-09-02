#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "../shared/Defines.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>

#define MAX_BAG_SIZE 36

#define BASE_SLOT_POS(x, y) sf::Vector2f(x, y) // ??
#define SLOT_SHAPE sf::Vector2f(15, 15)

class Item;
class Bag
{
    public:
    Bag(int idxBag, std::string const& FileNameIcon);

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
