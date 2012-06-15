#ifndef MAP_H
#define MAP_H

#include "Defines.h"
#include "GameState.h"
#include <cstdint>

struct World : GameState
{
    World();
    void LoadTileMap(uint8_t MapID);
    virtual void Draw();
    virtual void HandleEvent(sf::Event Event);

//private:
    sf::RenderStates MapStates;
    sf::VertexArray TileMap;
    sf::View MapView;
};

#endif