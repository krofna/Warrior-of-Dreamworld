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
#include "GameObject.hpp"
#include "GameObjectAI.hpp"
#include "World.hpp"
#include "AIFactory.hpp"
#include "shared/Opcodes.hpp"
#include "shared/Templates.hpp"
#include "Map.hpp"

GameObject::GameObject(uint64 ObjID, Map* pMap, uint16 x, uint16 y, GameObjectTemplate* pTemplate) :
WorldObject           (ObjID),
pTemplate             (pTemplate)
{
    Position = Vector2i(x, y);
    pAI = CreateAI(pTemplate->ScriptName, this);
    this->pMap = pMap;
    pMap->AddToTileGrid(this);
}

GameObject::~GameObject()
{
    delete pAI;
}

void GameObject::Update(int64 diff)
{
    pAI->UpdateAI(diff);
}

void GameObject::OnInteract(Player* pWho)
{
}

WorldPacket GameObject::PackData()
{
    WorldPacket Packet((uint16)MSG_ADD_OBJECT);
    Packet << ObjID << pTemplate->Tileset << pTemplate->Name << GetX() << GetY() << pTemplate->tx << pTemplate->ty;
    return Packet;
}
