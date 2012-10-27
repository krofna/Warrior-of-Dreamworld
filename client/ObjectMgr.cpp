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
#include "shared/Log.hpp"
#include "ObjectMgr.hpp"
#include <boost/archive/binary_iarchive.hpp>

ObjectMgr* sObjectMgr;

ObjectMgr::ObjectMgr(std::string const& TilesetPath, std::string const& TemplatesFilePath) :
m_TilesetPath       (TilesetPath),
m_TemplatesFilePath (TemplatesFilePath)
{
}

ObjectMgr::~ObjectMgr()
{
    std::for_each(m_ItemTemplates.begin(), m_ItemTemplates.end(), MapDeleter());
    std::for_each(m_CreatureTemplates.begin(), m_CreatureTemplates.end(), MapDeleter());
    std::for_each(m_Tilesets.begin(), m_Tilesets.end(), MapDeleter());
    std::for_each(m_Textures.begin(), m_Textures.end(), MapDeleter());
}

void ObjectMgr::Initialize()
{
    LoadTemplates();
}

void ObjectMgr::LoadTileset(std::string const& FileName)
{
    if (!SearchTileset(FileName))
    {
        sf::Texture* tilesetTexture = new sf::Texture;
        if (!tilesetTexture->loadFromFile(m_TilesetPath + '/' + FileName))
        {
            sLog.Write("Failed to load tileset texture: %s !", FileName);
            throw std::runtime_error("Failed to load a texture !");
        }

        m_Tilesets[FileName] = tilesetTexture;
    }
}

ItemTemplate* ObjectMgr::GetItemTemplate(uint64 ItemID)
{
    if (!SearchItemTemplate(ItemID))
    {
        sLog.Write("Failed to find data about this item ID: %u !", ItemID);
        throw std::runtime_error("Failed to find data about something.");
    }

    return m_ItemTemplates[ItemID];
}

sf::Texture* ObjectMgr::GetTileset(std::string const& TilesetName)
{
    if (SearchTileset(TilesetName))
        return m_Tilesets[TilesetName];
    else
    {
        LoadTileset(TilesetName);
        return m_Tilesets[TilesetName];
    }
}

sf::Texture* ObjectMgr::GetTileset(std::string const& TilesetName, sf::Color const& Mask, uint8 alpha)
{
    if (SearchTileset(TilesetName))
        return m_Tilesets[TilesetName];
    else
    {
        LoadTileset(TilesetName);
        sf::Texture* tileset = m_Tilesets[TilesetName];
        sf::Image imageTexture = tileset->copyToImage();
        imageTexture.createMaskFromColor(Mask, alpha);
        tileset->update(imageTexture);
        return tileset;
    }
}

sf::Texture* ObjectMgr::GetTexture(std::string const& TextureName, std::string const& TexturePath)
{
    auto TextureIt = m_Textures.find(TextureName);
    if (TextureIt != m_Textures.end())
        return TextureIt->second;

    sf::Texture* Tex = new sf::Texture;
    if (!Tex->loadFromFile(TexturePath))
    {
        sLog.Write("Failed to load texture (texture name: %s, texture path: %s)", TextureName, TexturePath);
        throw std::runtime_error("Failed to load texture.");
    }

    m_Textures[TextureName] = Tex;

    return Tex;
}

bool ObjectMgr::SearchItemTemplate(uint64 ItemID) const
{
    return m_ItemTemplates.find(ItemID) != m_ItemTemplates.end();
}

bool ObjectMgr::SearchCreatureTemplate(uint64 CreatureEntry) const
{
    return m_CreatureTemplates.find(CreatureEntry) != m_CreatureTemplates.end();
}

bool ObjectMgr::SearchTileset(std::string const& FileName) const
{
    return m_Tilesets.find(FileName) != m_Tilesets.end();
}

void ObjectMgr::LoadItemTemplate(std::string const& lineData)
{
    ItemTemplate* Proto = new ItemTemplate;
    std::vector<std::string> data;
    boost::algorithm::split(data, lineData, boost::is_any_of(";"), boost::algorithm::token_compress_on);

    Proto->ItemID        = boost::lexical_cast<uint64>(data.at(0));
    Proto->Class         = boost::lexical_cast<uint32>(data.at(1));
    Proto->SubClass      = boost::lexical_cast<uint32>(data.at(2));
    Proto->Name          = data.at(3);
    Proto->DisplayInfoID = boost::lexical_cast<uint32>(data.at(4));
    Proto->InventoryType = boost::lexical_cast<uint8>(data.at(5));
    Proto->Description   = data.at(6);

    m_ItemTemplates[Proto->ItemID] = Proto;
}

void ObjectMgr::LoadCreatureTemplate(std::string const& lineData)
{
    CreatureTemplate* Proto = new CreatureTemplate;
    std::vector<std::string> data;
    boost::algorithm::split(data, lineData, boost::is_any_of(";"), boost::algorithm::token_compress_on);

    Proto->Entry     = boost::lexical_cast<uint64>(data.at(0));
    Proto->Name      = data.at(1);
    Proto->Tileset   = data.at(2);
    Proto->tx        = boost::lexical_cast<uint16>(data.at(3));
    Proto->ty        = boost::lexical_cast<uint16>(data.at(4));
    Proto->npcflag   = boost::lexical_cast<uint16>(data.at(5)); // Really needed by Client ?
    Proto->MaxHealth = boost::lexical_cast<uint32>(data.at(6));
    Proto->MaxPower  = boost::lexical_cast<uint32>(data.at(7));

    m_CreatureTemplates[Proto->Entry] = Proto;
}

void ObjectMgr::LoadTemplates()
{
    std::ifstream fileData(m_TemplatesFilePath);

    if (fileData)
    {
        std::string line;

        while (std::getline(fileData, line))
        {
            int idxType = line.find(";");
            std::string typeTemplate = line.substr(0, idxType);

            if (typeTemplate == "ItemTemplate")
                LoadItemTemplate(line.substr(idxType + 1));
            else if (typeTemplate == "CreatureTemplate")
                LoadCreatureTemplate(line.substr(idxType + 1));
            else if (typeTemplate == "QuestTemplate") // Skip it. Thanks to krofna!
                ;
            else
                sLog.Write("Unknown template type: %s. templates_info.dbc damaged ?", typeTemplate);
        }
    }
    else
    {
        sLog.Write("Failed to load templates data (FileName: %s)", m_TemplatesFilePath);
        throw std::runtime_error("Failed to load data.");
    }
}
