#ifndef ITEM_DEFINED
#define ITEM_DEFINED

#include "../shared/Defines.hpp"
#include <SFML/System.hpp>

struct ItemTemplate;

class Item
{
    public:
    Item();
    static Item* Create(uint64 Entry);

    void Draw(sf::Vector2f const& baseSlotPos, int idx);
    void DrawInformation();

    void Use();

    private:
    uint64 m_GUID;
    ItemTemplate const* m_Template;
};

#endif
