/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012  Mislav Blazevic

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>

#include <boost/serialization/string.hpp>
#include <boost/archive/binary_oarchive.hpp>

#define WARN() std::cout << "BUG." << std::endl;

std::string ToString(int n)
{
    std::ostringstream out;
    out << n;
    return out.str();
}

void EditMap()
{
    int MapID;
    std::cout << "Map ID ?" << std::endl;
    std::cin >> MapID;

    std::string fileName = "map" + ToString(MapID) + ".map";
    std::ofstream File(fileName);

    if (File)
    {
        uint16_t SizeX, SizeY;
        std::cout << "Size (x, y) ?" << std::endl;
        std::cin >> SizeX >> SizeY;
        File << SizeX << " " << SizeY;

        std::string fileNameTileset;
        std::cout << "Tileset Filename: ";
        std::cin >> fileNameTileset;
        File << " " << fileNameTileset << " ";

        for (float y = 0 ; y < SizeY ; ++y)
            for (float x = 0 ; x < SizeX ; ++x)
                File << y << " " << x << " 1 15 ";
    }
}

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
    uint64_t ItemID;
    uint32_t Class;
    uint32_t SubClass;
    std::string Name;
    uint32_t DisplayInfoID;
    uint32_t InventoryType;
    uint8_t ContainerSlots;
    std::string Description;
};

void EditItemType(ItemTemplate& itemProto)
{
    if (itemProto.InventoryType == INVTYPE_BAG)
    {
        std::cout << "ContainerSlots ?" << std::endl;
        std::cin >> itemProto.ContainerSlots;
    }
}

static std::map<std::string, uint32_t> invtype_text =
{
    { "Non equipable", INVTYPE_NON_EQUIP },
    { "Bag", INVTYPE_BAG }
};

void EditItems()
{
    std::string fileNameData;
    std::cout << "Filename data ?" << std::endl;
    std::cin >> fileNameData;

    std::ofstream fileData (fileNameData, std::ios::ate);
    if (!fileData)
    {
        std::cerr << "Failed to open file !";
        throw std::runtime_error("Failed to open file: " + fileNameData + " !");
    }

    boost::archive::binary_oarchive out(fileData);

    uint64_t nItems;
    std::cout << "How many item do you want to create ?" << std::endl;
    std::cin >> nItems;

    for (uint64_t i = 0 ; i < nItems ; ++i)
    {
        ItemTemplate itemProto;

        std::cout << "Name ?" << std::endl;
        std::cin.ignore();
        std::getline(std::cin, itemProto.Name);

        std::cout << "Description ?" << std::endl;
        std::cin.ignore();
        std::getline(std::cin, itemProto.Description);

        std::cout << "Class ?" << std::endl;
        std::cin >> itemProto.Class;

        std::cout << "SubClass ?" << std::endl;
        std::cin >> itemProto.SubClass;

        std::cout << "Display Info ID ?" << std::endl;
        std::cin >> itemProto.DisplayInfoID;

        std::cout << "Inventory Type ?" << std::endl;
        for (auto it = invtype_text.begin() ; it != invtype_text.end() ; ++it)
            std::cout << it->first << " --> " << it->second << std::endl;
        std::cin >> itemProto.InventoryType;
        
        EditItemType(itemProto);

        out << i;
        itemProto.ItemID = i;
        out << itemProto;
    }
}

void EditDisplayIDMasterFile()
{
    std::string masterpath;
    std::cout << "Master path: ";
    std::cin >> masterpath;
    
    if (masterpath[masterpath.size()] == '/')
        masterpath.erase(masterpath.end() - 1);

    std::ofstream fileMaster(masterpath + '/' + "master_data.data");
    
    if (!fileMaster)
    {
        std::cerr << "Failed to open file master !" << std::endl;
        throw std::runtime_error("Failed to open a file!");
    }

    boost::archive::binary_oarchive out(fileMaster);

    int nFilenames;
    std::cout << "How many display ID texture have you ?" << std::endl;
    std::cin >> nFilenames;

    for (int i = 0 ; i < nFilenames ; ++i)
    {
        uint16_t DisplayInfoID;
        std::string FileName;

        std::cout << "DisplayInfoID ?" << std::endl;
        std::cin >> DisplayInfoID;

        std::cout << "FileName ?" << std::endl;
        std::cin >> FileName;

        out << DisplayInfoID << FileName;
    }
}

int main()
{
    int choice = 0;

    while (choice < 1 || choice > 3)
    {
        std::cout << "1. Create a map" << std::endl;
        std::cout << "2. Create items data for client" << std::endl;
        std::cout << "3. Create a display id master file for client" << std::endl;

        std::cin >> choice;
    }

    switch(choice)
    {
        case 1:
            EditMap();
            break;
        case 2:
            EditItems();
            break;
        case 3:
            EditDisplayIDMasterFile();
            break;
        default:
            WARN()
            break;
    }
    
    return 0;
}
