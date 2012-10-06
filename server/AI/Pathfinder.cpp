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
#include "Pathfinder.hpp"
#include "Map.hpp"
#include "Unit.hpp"

#include "shared/Math.hpp"
#include "shared/Log.hpp"

#define MAX_MAP_WIDTH 50
#define MAX_MAP_HEIGHT 50

/*
TODO:
- Bounds checking map specific
- Diagonal movement ( no cutting )
*/

boost::shared_array<PathfinderNode> Pathfinder::PathfindingGrid;
boost::shared_array<uint8> Pathfinder::PathfindingStatusGrid;
my_priority_queue Pathfinder::OpenList;

Pathfinder::Pathfinder(Unit* pOrigin) :
pOrigin               (pOrigin),
pMap                  (pOrigin->GetMap()),
pTarget               (nullptr),
Target                (-1, -1),
Home                  (pOrigin->GetPosition())
{
    pTileGrid =  &pMap->TileGrid;
    MovementCooldown = 1000;
}

void Pathfinder::Init()
{
    PathfindingGrid.reset(new PathfinderNode[MAX_MAP_HEIGHT * MAX_MAP_WIDTH]);
    std::memset(PathfindingGrid.get(), 0, MAX_MAP_HEIGHT * MAX_MAP_WIDTH);
    
    // Allocate some memory (completly random)
    OpenList.reserve(100);

    for(int y = 0; y < MAX_MAP_HEIGHT; ++y)
    {
        for(int x = 0; x < MAX_MAP_WIDTH; ++x)
        {
            PathfindingGrid[MAX_MAP_HEIGHT * y + x].Position = Vector2i(x, y);
        }
    }

    PathfindingStatusGrid.reset(new uint8[MAX_MAP_HEIGHT * MAX_MAP_WIDTH]);
}

void Pathfinder::Update(int64 diff)
{
    // Go home if target is either dead or not on this map
    if(pTarget && (pTarget->IsDead() || pTarget->GetMap() != pMap || !pTarget->IsInWorld()))
    {
        pTarget = nullptr;
        Target = Home;
        GeneratePath();
        pOrigin->ClearVictim();
    }
    // If target moved since last path generate OR something is blocking
    // our current path, generate new path
    else if((pTarget && pTarget->GetPosition() != Target) || (!Path.empty() && (*pTileGrid)[Path.top().y][Path.top().x]))
    {
        this->Target = pTarget->GetPosition();
        GeneratePath();
    }

    // If not arrived to destination and can move, move
    if(!Path.empty() && (MovementCooldown <= diff))
    {
        pOrigin->UpdatePosition(Path.top());
        Path.pop();
        MovementCooldown = 500;
    }
    else
    {
        MovementCooldown -= diff;
    }
}

void Pathfinder::UpdateTarget(Unit* pNewTarget)
{
    this->pTarget = pNewTarget;
}

void Pathfinder::GeneratePath()
{
    sLog.Write("Generating new path");

    // Destroy old path
    Path = std::stack<Vector2i>();

    // Zero out status grid
    std::memset(PathfindingStatusGrid.get(), 0, MAX_MAP_HEIGHT * MAX_MAP_WIDTH * sizeof(uint8));

    // Get Node with same position as creature in pathfinding grid
    PathfinderNode* pOriginNode = &(PathfindingGrid[MAX_MAP_HEIGHT * pOrigin->GetY() + pOrigin->GetX()]);

    // Origin node does not have parent node
    pOriginNode->pParent = nullptr;

    // Origin node does not have G cost
    pOriginNode->G = 0;
    pOriginNode->H = 10 * math::GetManhattanDistance(Vector2i(pOriginNode->Position.x, pOriginNode->Position.y), Target);

    // Add origin node to queue
    OpenList.push(pOriginNode);

    do
    {
        // Grab node with highest priority
        PathfinderNode* pCurrent = OpenList.top();
        OpenList.pop();

        // Move it to "closed" list
        PathfindingStatusGrid[MAX_MAP_HEIGHT * pCurrent->Position.y + pCurrent->Position.x] = CLOSED;

        // If current node has same coordinates as target, we found our path
        if(pCurrent->Position == Target)
        {
            // Create path using parents to track down origin
            while(pCurrent->pParent != nullptr)
            {
                Path.push(pCurrent->Position);
                pCurrent = pCurrent->pParent;
            }
            
            // Destroy open list
            OpenList.clear();
            
            return;
        }

        // For each of adjacent nodes
        // if it is not out of bounds
        // and if tile is 'walkable',
        // check its status

        // Upper
        if(pCurrent->Position.y != 0)
        {
            CheckNode(pCurrent, 0, -1, 10);
        }

        // Lower
        if(pCurrent->Position.y != 49) // Todo pMap->height
        {
            CheckNode(pCurrent, 0, 1, 10);
        }

        // Right
        if(pCurrent->Position.x != 49) // pMap->width-... TODO
        {
            CheckNode(pCurrent, 1, 0, 10);
        }

        // Left
        if(pCurrent->Position.x != 0)
        {
            CheckNode(pCurrent, -1, 0, 10);
        }

        // Upper-right
        if(pCurrent->Position.x != 49 && pCurrent->Position.y != 0)
        {
            CheckNode(pCurrent, 1, -1, 14);
        }

        // Upper-left
        if(pCurrent->Position.x != 0 && pCurrent->Position.y != 0)
        {
            CheckNode(pCurrent, -1, -1, 14);
        }

        // Lower-right
        if(pCurrent->Position.x != 49 && pCurrent->Position.y != 49)
        {
            CheckNode(pCurrent, 1, 1, 14);
        }

        // Lower-left
        if(pCurrent->Position.x != 0 && pCurrent->Position.y != 49)
        {
            CheckNode(pCurrent, -1, 1, 14);
        }

    } while(!OpenList.empty());

    sLog.Write("Could not generate path");
}

void Pathfinder::CheckNode(PathfinderNode* pCurrent, int x, int y, int Cost)
{
    if(!(*pTileGrid)[pCurrent->Position.y+y][pCurrent->Position.x+x])
    {
        PathfinderNode* pAdjacent = &PathfindingGrid[MAX_MAP_HEIGHT * (pCurrent->Position.y+y) + pCurrent->Position.x+x];

        // Check status
        switch(PathfindingStatusGrid[MAX_MAP_HEIGHT * pAdjacent->Position.y + pAdjacent->Position.x])
        {
            // If there is no status yet
        case (uint8)UNKNOWN:
            // Make the current node parent
            pAdjacent->pParent = pCurrent;

            // Calculate costs
            pAdjacent->G = pCurrent->G + Cost;
            pAdjacent->H = 10 * math::GetManhattanDistance(Vector2i(pAdjacent->Position.x, pAdjacent->Position.y), Target);

            // Add it to open list
            PathfindingStatusGrid[MAX_MAP_HEIGHT * pAdjacent->Position.y + pAdjacent->Position.x] = OPEN;
            OpenList.push(pAdjacent);
            
            break;
            // If it is already on open list
        case (uint8)OPEN:
            // Check if G path of pCurrent is better than G path of pAdjacent
            if((pCurrent->G + Cost) < pAdjacent->G)
            {
                // Make pCurrent parent of pAdjacent
                pAdjacent->pParent = pCurrent;

                // Recalculate distance
                pAdjacent->G = pCurrent->G + Cost;
                pAdjacent->H = 10 * math::GetManhattanDistance(Vector2i(pAdjacent->Position.x, pAdjacent->Position.y), Target);

                OpenList.re_insert(pAdjacent);
            }

            // If its closed, ignore it
        case (uint8)CLOSED:
            break;
        }
    }
}
