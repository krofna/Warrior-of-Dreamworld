#include "../shared/Log.hpp"
#include "ObjectMgr.hpp"
#include <boost/archive/binary_iarchive.hpp>

ObjectMgr::ObjectMgr(std::string const& TilesetPath, std::string const& DisplayInfoPath, std::string const& ItemDataFilePath) : m_TilesetPath(TilesetPath),
                                                                                                                                m_DisplayInfoPath(DisplayInfoPath),
                                                                                                                                m_ItemDataFilePath(ItemDataFilePath)
{

}

void ObjectMgr::Initialize()
{
    LoadItemsInfo();
    LoadDisplayIDsInformations();
}

void ObjectMgr::Cleanup()
{
    for (auto it = m_ItemsData.begin() ; it != m_ItemsData.end() ; ++it)
    {
        delete it->second;
    }

    for (auto it = m_DisplayInfoIDs.begin() ; it != m_DisplayInfoIDs.end() ; ++it)
    {
        delete it->second;
    }

    for (auto it = m_Tilesets.begin() ; it != m_Tilesets.end() ; ++it)
    {
        delete it->second;
    }
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

void ObjectMgr::LoadDisplayID(uint16 DisplayInfoID)
{
    if (!SearchDisplayID(DisplayInfoID))
    {
        sf::Texture* displayIDTexture = new sf::Texture;
        if (FindDisplayID_Information(DisplayInfoID))
        {
            std::string FileNameDisplayID_Texture = FindDisplayID_Filename(DisplayInfoID);
            if (!displayIDTexture->loadFromFile(m_DisplayInfoPath + '/' + FileNameDisplayID_Texture))
            {
                sLog.Write("Failed to load display id texture: %s !", FileNameDisplayID_Texture);
                throw std::runtime_error("Failed to load a texture !");
            }
        }
        else
        {
            sLog.Write("Failed to find information about this display id: %u !", DisplayInfoID);
            sLog.Write("Maybe a unknown display info id...");
            throw std::runtime_error("Failed to find information about something.");
        }
    }
}

ItemTemplate* ObjectMgr::GetItemData(uint64 ItemID)
{
    if (!FindItemData(ItemID))
    {
        sLog.Write("Failed to find data about this item ID: %u !", ItemID);
        throw std::runtime_error("Failed to find data about something.");
    }

    return m_ItemsData[ItemID];
}

sf::Texture* ObjectMgr::GetDisplayInfoID(uint16 DisplayInfoID)
{
    if (SearchDisplayID(DisplayInfoID))
        return m_DisplayInfoIDs[DisplayInfoID];
    else
    {
        LoadDisplayID(DisplayInfoID);
        return m_DisplayInfoIDs[DisplayInfoID];
    }
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

bool ObjectMgr::FindItemData(uint64 ItemID) const
{
    return m_ItemsData.find(ItemID) != m_ItemsData.end();
}

bool ObjectMgr::SearchDisplayID(uint16 DisplayInfoID) const
{
    return m_DisplayInfoIDs.find(DisplayInfoID) != m_DisplayInfoIDs.end();
}

bool ObjectMgr::SearchTileset(std::string const& FileName) const
{
    return m_Tilesets.find(FileName) != m_Tilesets.end();
}

std::string ObjectMgr::FindDisplayID_Filename(uint16 DisplayInfoID)
{
    return m_DisplayID_Filenames_Table[DisplayInfoID];
}

bool ObjectMgr::FindDisplayID_Information(uint16 DisplayInfoID) const
{
    return m_DisplayID_Filenames_Table.find(DisplayInfoID) != m_DisplayID_Filenames_Table.end();
}

void ObjectMgr::LoadItemsInfo()
{
    std::ifstream fileData (m_ItemDataFilePath);

    if (fileData)
    {
        boost::archive::binary_iarchive in(fileData);
        
        // If C++ was something for noob :D
/*        ItemsInfo i;
        in >> i;

        while (i)
        {
            m_ItemsData[i.GiveMeA()] = i.GiveMeB();
        }
        // I really need to sleep :) (Did at 01:21 a.m) */

        // Reality:

        int nItems;
        in >> nItems;

        for (int i = 0 ; i < nItems ; ++i)
        {
            uint64 itemID;
            in >> itemID;

            ItemTemplate* itemProto = new ItemTemplate;
            in >> *itemProto;

            m_ItemsData[itemID] = itemProto;
        }
    }
    else
    {
        sLog.Write("Failed to load items' data (FileName: %s)", m_ItemDataFilePath);
        throw std::runtime_error("Failed to load data.");
    }
}

void ObjectMgr::LoadDisplayIDsInformations()
{
    std::ifstream fileMasterInformation (m_DisplayInfoPath + "/master_data.data");

    if (fileMasterInformation)
    {
        boost::archive::binary_iarchive in(fileMasterInformation);

        int nFiles;
        in >> nFiles;

        for (int i = 0 ; i < nFiles ; ++i)
        {
            uint16 DisplayInfoID;
            in >> DisplayInfoID;

            std::string fileDataInformationName;
            in >> fileDataInformationName;

            m_DisplayID_Filenames_Table[DisplayInfoID] = fileDataInformationName;
        }
    }
    else
    {
        sLog.Write("Failed to load DisplayIDs' Informations (FileName: %s", m_DisplayInfoPath + "/master_data.data");
        throw std::runtime_error("Failed to load informations.");
    }
}
