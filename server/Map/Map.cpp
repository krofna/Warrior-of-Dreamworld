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
#include "World.hpp"
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

Map::Map     (MapTemplate* pTemplate, uint16 MapID, uint16 SizeX, uint16 SizeY) : 
NewSpellBoxID(0),
pTemplate    (pTemplate),
MapID        (MapID),
TileGrid     (SizeX, std::vector<WorldObject*>(SizeY, nullptr))
{
    pMapScript = sWorld->GetAIFactory()->CreateMapScript(pTemplate->ScriptName, this);
}

Map::~Map()
{
    // Notify everyone that server is going down
    std::for_each(Players.begin(), Players.end(), boost::bind(&Player::OnServerShutdown, _1));

    // Cleanup entities
    std::for_each(Creatures.begin(), Creatures.end(), boost::bind(&operator delete, _1));
    std::for_each(GameObjects.begin(), GameObjects.end(), boost::bind(&operator delete, _1));
    std::for_each(Spells.begin(), Spells.end(), boost::bind(&operator delete, _1));
    
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

    while(Result->next())
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
    for(auto PlayerIterator = Players.begin(); PlayerIterator != Players.end(); ++PlayerIterator)
    {
        (*PlayerIterator)->SendPacket(Packet);
    }
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


bool Map::CheckOutside(int PosX, int PosY, uint8 Direction) const
{
    switch(Direction)
    {
        case MOVE_UP: return ((PosY - 1) < 0);
        case MOVE_DOWN: return ((PosY + 1) >= TileGrid.size()-1);
        case MOVE_LEFT: return ((PosX - 1) < 0);
        case MOVE_RIGHT: return ((PosX + 1) >= TileGrid[0].size()-1);
        default: return false;
    }
}
