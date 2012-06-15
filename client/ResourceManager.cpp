#include "ResourceManager.h"

std::map<std::string, sf::Texture*> ResourceManager::Tilesets;

sf::Texture* ResourceManager::GetTileset(const std::string& FileName)
{
    if(Tilesets.find(FileName) == Tilesets.end())
    {
        sf::Texture* TilesetTexture = new sf::Texture();
        TilesetTexture->loadFromFile("data/tileset/" + FileName);
        Tilesets[FileName] = TilesetTexture;
        return TilesetTexture;
    }

    return Tilesets[FileName];
}

void ResourceManager::RemoveTileset(const std::string& FileName)
{
    Tilesets.erase(FileName);
}