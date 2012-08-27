#ifndef TEMPLATE_DEFINED
#define TEMPLATE_DEFINED

struct ItemTemplate
{
    uint64 ItemID;
    uint32 Class;
    uint32 SubClass;
    std::string Name;
    uint32 DisplayInfoID;
    uint32 InventoryType;
    uint8 ContainerSlots;
    std::string Description;
};

#endif
