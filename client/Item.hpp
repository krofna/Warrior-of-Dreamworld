#ifndef ITEM_DEFINED
#define ITEM_DEFINED

class ItemTemplate;
class Item
{
    public:
    static Item* Create(uint64 Entry);

    void Draw();
    void DrawInformation();

    void Use();

    private:
    uint64 m_GUID;
    ItemTemplate const* m_Template;
};

#endif
