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
#include "Map.hpp"
#include "shared/Opcodes.hpp"
#include "shared/Log.hpp"
#include <boost/bind.hpp>
#include "Player.hpp"
#include "Database.hpp"
#include "Pathfinder.hpp"
#include "ObjectMgr.hpp"
#include "Creature.hpp"
#include "GameObject.hpp"
#include "MapScript.hpp"
#include "AIFactory.hpp"

Map::Map     (MapTemplate* pTemplate, uint32 MapID) : 
NewSpellBoxID(0),
pTemplate    (pTemplate),
MapID        (MapID)
{
    pMapScript = CreateScript(pTemplate->ScriptName, this);
    TileGrid.resize(pTemplate->SizeY, std::vector<WorldObject*>(pTemplate->SizeX, nullptr));
}

Map::~Map()
{
    // Notify everyone that server is going down
    if(!(pTemplate->mapflag & MAP_INSTANCED))
        std::for_each(Players.begin(), Players.end(), boost::bind(&Player::OnServerShutdown, _1));

    // Cleanup entities
    std::for_each(Creatures.begin(), Creatures.end(), Deleter());
    std::for_each(GameObjects.begin(), GameObjects.end(), Deleter());
    std::for_each(Spells.begin(), Spells.end(), Deleter());
    
    delete pMapScript;

    sLog.Write("Map %u destroyed.", MapID);
}

uint16 Map::GetID() const
{
    return MapID;
}

uint32 Map::GetNewSpellBoxID() const
{
    return NewSpellBoxID;
}

void Map::Load()
{
    QueryResult Result(sDatabase.PQuery("SELECT * FROM `creature` WHERE map='%u'", MapID));
    Creature* pCreature;

    while(Result->next())
    {
        pCreature = new Creature(Result->getUInt64(1), this, Result->getUInt(4), Result->getUInt(5), sObjectMgr.GetCreatureTemplate(Result->getUInt(2)));
        Creatures.push_back(pCreature);
    }

    QueryResult GoResult(sDatabase.PQuery("SELECT * FROM `gameobject` WHERE map='%u'", MapID));
    GameObject* pGo;

    while(GoResult->next())
    {
        pGo = new GameObject(GoResult->getUInt64(1), this, GoResult->getUInt(4), GoResult->getUInt(5), sObjectMgr.GetGameObjectTemplate(GoResult->getUInt(2)));
        GameObjects.push_back(pGo);
    }
}

void Map::Update(int64 diff)
{
    pMapScript->Update(diff);
    std::for_each(Spells.begin(), Spells.end(), boost::bind(&SpellBox::Update, _1, diff));
    std::for_each(Players.begin(), Players.end(), boost::bind(&Map::UnitUpdate, this, _1, diff));
    std::for_each(Creatures.begin(), Creatures.end(), boost::bind(&Map::UnitUpdate, this, _1, diff));
    std::for_each(GameObjects.begin(), GameObjects.end(), boost::bind(&GameObject::Update, _1, diff));
}

void Map::UnitUpdate(Unit* pUnit, int64 diff)
{
    // Check if unit got hit by spell
    for(auto SpellBoxIter = Spells.begin(); SpellBoxIter != Spells.end();)
    {
        if((*SpellBoxIter)->pCaster != pUnit/*Is friendly? Healing?*/ && (*SpellBoxIter)->CollidesWith(pUnit))
        {
            pUnit->SpellHit(*SpellBoxIter);
            delete *SpellBoxIter;
            SpellBoxIter = Spells.erase(SpellBoxIter);
        }
        else
        {
            ++SpellBoxIter;
        }
    }
    pUnit->Update(diff);
}

void Map::AddToTileGrid(WorldObject* pWho)
{
    sLog.Write("Object guid %ull has moved to X: %u, Y: %u", pWho->GetObjectID(), pWho->GetX(), pWho->GetY());
    TileGrid[pWho->GetY()][pWho->GetX()] = pWho;
}

Creature* Map::SummonCreature(uint64 Entry, Unit* pSummoner)
{
    Creature* pCreature = new Creature(sDatabase.Generate64BitsGUID(), this, pSummoner->GetX(), pSummoner->GetY(), sObjectMgr.GetCreatureTemplate(Entry));
    Creatures.push_back(pCreature);
    return pCreature;
}

void Map::AddPlayer(Player* pPlayer)
{
    // Pack & send all data about creatures to new player
    for(auto CreatureIterator = Creatures.begin(); CreatureIterator != Creatures.end(); ++CreatureIterator)
    {
        WorldPacket CreatureData = (*CreatureIterator)->PackData();
        pPlayer->SendPacket(CreatureData);
    }
    
    // Pack & send all data about game objects to new player
    for(auto GoIterator = GameObjects.begin(); GoIterator != GameObjects.end(); ++GoIterator)
    {
        WorldPacket GoData = (*GoIterator)->PackData();
        pPlayer->SendPacket(GoData);
    }

    // Pack & send all data about players in map to new player
    // and send data about the new player to all other players
    
    WorldPacket NewPlayerData = pPlayer->PackData();
    
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        WorldPacket PlayerData = (*PlayerIterator)->PackData();
        pPlayer->SendPacket(PlayerData);
        (*PlayerIterator)->SendPacket(NewPlayerData);
    }

    // Send the new player his data
    pPlayer->SendPacket(NewPlayerData);

    // Finally, we add the new player to the map
    Players.push_back(pPlayer);
}

void Map::AddSpell(Unit* pCaster, SpellTemplate* pSpellTemplate, float Angle)
{
    // PLACEHOLDER
    Spells.push_back(new SpellBox(pSpellTemplate, pCaster, FloatRect((float)pCaster->GetX()+(5/32), (float)pCaster->GetY()+(3/32), 1.0f-float(9/32), 1.f-float(8/32)), Angle, NewSpellBoxID));
    ++NewSpellBoxID;
}

void Map::SendToPlayers(WorldPacket& Packet)
{
    std::for_each(Players.begin(), Players.end(), boost::bind(&Player::SendPacket, _1, Packet));
}

void Map::RemovePlayer(Player* pPlayer)
{
    WorldPacket Packet((uint16)MSG_REMOVE_OBJECT);
    Packet << pPlayer->GetObjectID();
    
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end();)
    {
        if((*PlayerIterator) == pPlayer)
        {
            PlayerIterator = Players.erase(PlayerIterator);
        }
        else
        {
            (*PlayerIterator)->SendPacket(Packet);
            ++PlayerIterator;
        }
    }
}

bool Map::TryInteract(Player* pWho, uint16 x, uint16 y)
{
    if(!TileGrid[y][x])
        return false;

    TileGrid[y][x]->OnInteract(pWho);
    return true;
}

void Map::RemoveFromTileGrid(uint16 x, uint16 y)
{
    TileGrid[y][x] = nullptr;
}

WorldObject* Map::GetObjectAt(uint16 x, uint16 y)
{
    return TileGrid[y][x];
}

WorldObjectGrid* Map::GetWorldObjectGrid()
{
    return &TileGrid;
}

bool Map::CheckOutside(uint16 PosX, uint16 PosY, uint8 Direction) const
{
    switch(Direction)
    {
        case MOVE_UP: return ((PosY - 1) < 0);
        case MOVE_DOWN: return ((PosY + 1) >= pTemplate->SizeY);
        case MOVE_LEFT: return ((PosX - 1) < 0);
        case MOVE_RIGHT: return ((PosX + 1) >= pTemplate->SizeX);
        default: return true;
    }
}
