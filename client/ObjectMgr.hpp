#ifndef OBJECT_MGR_DEFINED
#define OBJECT_MGR_DEFINED

#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include "Templates.hpp"

class ObjectMgr
{
    public:
    ObjectMgr(std::string const& TilesetDataFolder, std::string const& DisplayInfoIDFolder, std::string const& ItemDataFilePath);

    void Initialize();

    // Use this function, if you not using anymore item data or display info texture or tileset texture !
    // Else, the Client might crash !
    void Cleanup();

    void LoadTileset(std::string const& TilesetName);
    void LoadDisplayID(uint16 DisplayInfoID);

    ItemTemplate* GetItemData(uint64 ItemID);
    sf::Texture* GetDisplayInfoID(uint16 DisplayInfoID);
    sf::Texture* GetTileset(std::string const& TilesetName);

    bool FindItemData(uint64 ItemID) const;
    bool SearchDisplayID(uint16 DisplayInfoID) const;
    bool SearchTileset(std::string const& FileName) const;

    sf::Texture* GetTexture(std::string const& TextureName, std::string const& TexturePath);

    private:
    std::string FindDisplayID_Filename(uint16 DisplayInfoID);
    bool FindDisplayID_Information(uint16 DisplayInfoID) const;

    void LoadItemsInfo();
    void LoadDisplayIDsInformations();

    std::map<uint64, ItemTemplate* > m_ItemsData;
    std::map<uint16, sf::Texture* > m_DisplayInfoIDs;
    std::map<std::string, sf::Texture* > m_Tilesets;
    std::map<std::string, sf::Texture* > m_Textures;

    std::map<uint16, std::string> m_DisplayID_Filenames_Table;

    std::string m_TilesetPath;
    std::string m_DisplayInfoPath;
    std::string m_ItemDataFilePath;
};

#endif
