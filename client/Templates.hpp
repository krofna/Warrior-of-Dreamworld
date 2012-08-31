#ifndef TEMPLATE_DEFINED
#define TEMPLATE_DEFINED

#include "../shared/Defines.hpp"
#include <boost/serialization/string.hpp>

enum InventoryTypeEnumeration
{
    INVTYPE_NON_EQUIP,
    INVTYPE_BAG
};

struct ItemTemplate
{
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & ItemID & Class & SubClass & Name & DisplayInfoID & InventoryType & ContainerSlots & Description;
    }
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
