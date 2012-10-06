/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012 Ryan Lahfa

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
#ifndef OBJECT_MGR_DEFINED
#define OBJECT_MGR_DEFINED

#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include "../shared/Defines.hpp"
#include "shared/Templates.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

// Data are stored in
/**
 *
 * Templates: template_info.dbc
 * Tilesets: in a folder
 * DisplayIDs data: displayid_info.dbc
 *
**/

class ObjectMgr
{
    public:
    ObjectMgr(std::string const& TilesetDataFolder, std::string const& TemplatesFilePath);

    void Initialize();

    // Use this function, if you not using anymore data from ObjectMgr
    // Else, the Client might crash !
    void Cleanup();

    void LoadTileset(std::string const& TilesetName);

    ItemTemplate* GetItemTemplate(uint64 ItemID);
    CreatureTemplate* GetCreatureTemplate(uint64 CreatureID);

    sf::Texture* GetTileset(std::string const& TilesetName);
    sf::Texture* GetTileset(std::string const& TilesetName, sf::Color const& MaskColor, uint8 alpha = 0); // Very slow function because it copy Texture in Image and copy Image in Texture

    bool SearchItemTemplate(uint64 ItemID) const;
    bool SearchCreatureTemplate(uint64 CreatureID) const;

    bool SearchTileset(std::string const& FileName) const;

    sf::Texture* GetTexture(std::string const& TextureName, std::string const& TexturePath);

    private:
    void LoadTemplates();

    void LoadItemTemplate(std::string const& lineData);
    void LoadCreatureTemplate(std::string const& lineData);

    std::map<uint64, ItemTemplate* > m_ItemTemplates;
    std::map<uint64, CreatureTemplate* > m_CreatureTemplates;

    std::map<std::string, sf::Texture* > m_Tilesets;
    std::map<std::string, sf::Texture* > m_Textures;

    std::string m_TilesetPath;
    std::string m_TemplatesFilePath;


    struct MapSecondDeleter
    {
        template<typename Type>
        void operator() (Type toDelete)
        {
            delete toDelete.second;
        }
    };
};

#endif
