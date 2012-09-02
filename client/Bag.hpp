#ifndef BAG_DEFINED
#define BAG_DEFINED

#include "../shared/Defines.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>

#define MAX_BAG_SIZE 36


class Item;
class Bag
{
    public:
    Bag(int idxBag, std::string const& FileNameIcon);

    void Draw();
    void DrawIcon();
    void DrawEmptySlot();

    Item* GetItem(uint8 SrcSlot) const;

    void Store(uint8 DestSlot, Item* pItem);
    void Destroy(uint8 SrcSlot);
    void Create(uint8 DestSlot, uint64 Entry);

    private:
    uint8 m_Capacity;
    static_template_array<Item*, MAX_BAG_SIZE> m_Slots;

    // GUI Stuff
    sf::Sprite* m_IconSprite;

    static const sf::Vector2f PositionsBag[4];
};

#endif
